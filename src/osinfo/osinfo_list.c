/*
 * libosinfo:
 *
 * Copyright (C) 2009-2012, 2014 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *   Arjun Roy <arroy@redhat.com>
 *   Christophe Fergeau <cfergeau@redhat.com>
 *   Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <osinfo/osinfo.h>
#include <glib/gi18n-lib.h>

G_DEFINE_ABSTRACT_TYPE(OsinfoList, osinfo_list, G_TYPE_OBJECT);

#define OSINFO_LIST_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), OSINFO_TYPE_LIST, OsinfoListPrivate))

/**
 * SECTION:osinfo_list
 * @short_description: Abstract base class for entity lists
 * @see_also: #OsinfoEntity
 *
 * #OsinfoList provides a way to maintain a list of #OsinfoEntity objects.
 *
 */

struct _OsinfoListPrivate
{
    GPtrArray *array;
    GHashTable *entities;

    GType elementType;
};

enum {
    PROP_O,

    PROP_ELEMENT_TYPE
};


static void
osinfo_list_set_property(GObject      *object,
                         guint         property_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
    OsinfoList *list = OSINFO_LIST(object);

    switch (property_id) {
    case PROP_ELEMENT_TYPE:
        list->priv->elementType = g_value_get_gtype(value);
        break;

    default:
        /* We don't have any other property... */
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

static void
osinfo_list_get_property(GObject    *object,
                         guint       property_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
    OsinfoList *list = OSINFO_LIST(object);

    switch (property_id) {
    case PROP_ELEMENT_TYPE:
        g_value_set_gtype(value, list->priv->elementType);
        break;

    default:
        /* We don't have any other property... */
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}


static void
osinfo_list_finalize(GObject *object)
{
    OsinfoList *list = OSINFO_LIST(object);

    g_ptr_array_free(list->priv->array, TRUE);
    g_hash_table_unref(list->priv->entities);

    /* Chain up to the parent class */
    G_OBJECT_CLASS(osinfo_list_parent_class)->finalize(object);
}

/* Init functions */
static void
osinfo_list_class_init(OsinfoListClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    g_klass->set_property = osinfo_list_set_property;
    g_klass->get_property = osinfo_list_get_property;
    g_klass->finalize = osinfo_list_finalize;

    /**
     * OsinfoList:element-type:
     *
     * The specialization of the list. The list will be
     * restricted to storing #OsinfoEntity objects of
     * the specified type.
     */
    pspec = g_param_spec_gtype("element-type",
                               "Element type",
                               _("List element type"),
                               OSINFO_TYPE_ENTITY,
                               G_PARAM_CONSTRUCT_ONLY |
                               G_PARAM_READWRITE |
                               G_PARAM_STATIC_STRINGS);

    g_object_class_install_property(g_klass,
                                    PROP_ELEMENT_TYPE,
                                    pspec);

    g_type_class_add_private(klass, sizeof(OsinfoListPrivate));
}

static void
osinfo_list_init(OsinfoList *list)
{
    list->priv = OSINFO_LIST_GET_PRIVATE(list);
    list->priv->array = g_ptr_array_new_with_free_func(NULL);
    list->priv->entities = g_hash_table_new_full(g_str_hash,
                                                 g_str_equal,
                                                 NULL,
                                                 g_object_unref);
}

/**
 * osinfo_list_get_element_type:
 * @list: the entity list
 *
 * Retrieves the type of the subclass of #OsinfoEntity
 * that may be stored in the list
 *
 * Returns: the type of entity stored
 */
GType osinfo_list_get_element_type(OsinfoList *list)
{
    return list->priv->elementType;
}


/**
 * osinfo_list_get_length:
 * @list: the entity list
 *
 * Retrieves the number of elements currently stored
 * in the list
 *
 * Returns: the list length
 */
gint osinfo_list_get_length(OsinfoList *list)
{
    return list->priv->array->len;
}

/**
 * osinfo_list_get_nth:
 * @list: the entity list
 * @idx: the list position to fetch
 *
 * Retrieves the element in the list at position @idx. If
 * @idx is less than zero, or greater than the number of
 * elements in the list, the results are undefined.
 *
 * Returns: (transfer none): the list element
 */
OsinfoEntity *osinfo_list_get_nth(OsinfoList *list, gint idx)
{
    return g_ptr_array_index(list->priv->array, idx);
}


/**
 * osinfo_list_get_elements:
 * @list: the entity list
 *
 * Retrieve a linked list of all elements in the list.
 *
 * Returns: (transfer container) (element-type OsinfoEntity): the list elements
 */
GList *osinfo_list_get_elements(OsinfoList *list)
{
    return g_hash_table_get_values(list->priv->entities);
}

/**
 * osinfo_list_find_by_id:
 * @list: the entity list
 * @id: the unique identifier
 *
 * Search the list looking for the entity with a matching
 * unique identifier.
 *
 * Returns: (transfer none): the matching entity, or NULL
 */
OsinfoEntity *osinfo_list_find_by_id(OsinfoList *list, const gchar *id)
{
    return g_hash_table_lookup(list->priv->entities, id);
}


/**
 * osinfo_list_add:
 * @list: the entity list
 * @entity: (transfer none): the entity to add to the list
 *
 * Adds a new entity to the list.
 */
void osinfo_list_add(OsinfoList *list, OsinfoEntity *entity)
{
    OsinfoEntity *preexisting;
    g_return_if_fail(G_TYPE_CHECK_INSTANCE_TYPE(entity, list->priv->elementType));

    g_object_ref(entity);
    preexisting = osinfo_list_find_by_id(list, osinfo_entity_get_id(entity));
    if (preexisting != NULL) {
        g_ptr_array_remove(list->priv->array, preexisting);
    }
    g_ptr_array_add(list->priv->array, entity);
    g_hash_table_replace(list->priv->entities,
                         (gchar *)osinfo_entity_get_id(entity), entity);
}


/**
 * osinfo_list_add_filtered:
 * @list: the entity list
 * @source: (transfer none): the source for elements
 * @filter: (transfer none): filter to process the source with
 *
 * Adds all entities from @source which are matched by @filter. Using one
 * of the constructors in a subclass is preferrable
 * to this method.
 */
void osinfo_list_add_filtered(OsinfoList *list, OsinfoList *source, OsinfoFilter *filter)
{
    int i, len;
    g_return_if_fail(list->priv->elementType == source->priv->elementType);

    len = osinfo_list_get_length(source);
    for (i = 0; i < len; i++) {
        OsinfoEntity *entity = osinfo_list_get_nth(source, i);
        if (osinfo_filter_matches(filter, entity))
            osinfo_list_add(list, entity);
    }
}


/**
 * osinfo_list_add_intersection:
 * @list: the entity list
 * @sourceOne: (transfer none): the first list to add
 * @sourceTwo: (transfer none): the second list to add
 *
 * Computes the intersection between @sourceOne and @sourceTwo and
 * adds the resulting list of entities to the @list. Using one
 * of the constructors in a subclass is preferrable
 * to this method.
 */
void osinfo_list_add_intersection(OsinfoList *list, OsinfoList *sourceOne, OsinfoList *sourceTwo)
{
    int i, len;
    g_return_if_fail(list->priv->elementType == sourceOne->priv->elementType);
    g_return_if_fail(list->priv->elementType == sourceTwo->priv->elementType);

    // Make set representation of otherList and newList
    GHashTable *otherSet = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    GHashTable *newSet = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    // Add all from otherList to otherSet
    len = osinfo_list_get_length(sourceTwo);
    for (i = 0; i < len; i++) {
        OsinfoEntity *entity = osinfo_list_get_nth(sourceTwo, i);
        g_hash_table_insert(otherSet, g_strdup(osinfo_entity_get_id(entity)), entity);
    }

    // If other contains entity, and new list does not, add to new list
    len = osinfo_list_get_length(sourceOne);
    for (i = 0; i < len; i++) {
        OsinfoEntity *entity = osinfo_list_get_nth(sourceOne, i);

        if (g_hash_table_lookup(otherSet, osinfo_entity_get_id(entity)) &&
            !g_hash_table_lookup(newSet, osinfo_entity_get_id(entity))) {
            g_hash_table_insert(newSet, g_strdup(osinfo_entity_get_id(entity)), entity);
            osinfo_list_add(list, entity);
        }
    }

    g_hash_table_destroy(otherSet);
    g_hash_table_destroy(newSet);
}


/**
 * osinfo_list_add_union:
 * @list: the entity list
 * @sourceOne: (transfer none): the first list to add
 * @sourceTwo: (transfer none): the second list to add
 *
 * Computes the union between @sourceOne and @sourceTwo and
 * adds the resulting list of entities to the @list. Using one
 * of the constructors in a subclass is preferrable
 * to this method.
 */
void osinfo_list_add_union(OsinfoList *list, OsinfoList *sourceOne, OsinfoList *sourceTwo)
{
    // Make set version of new list
    GHashTable *newSet;
    g_return_if_fail(list->priv->elementType == sourceOne->priv->elementType);
    g_return_if_fail(list->priv->elementType == sourceTwo->priv->elementType);

    newSet = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    // Add all from first list to new list
    int i, len;
    len = osinfo_list_get_length(sourceOne);
    for (i = 0; i < len; i++) {
        OsinfoEntity *entity = osinfo_list_get_nth(sourceOne, i);
        osinfo_list_add(list, entity);
        g_hash_table_insert(newSet, g_strdup(osinfo_entity_get_id(entity)), entity);
    }

    // Add remaining elements from this list to new list
    len = osinfo_list_get_length(sourceTwo);
    for (i = 0; i < len; i++) {
        OsinfoEntity *entity = osinfo_list_get_nth(sourceTwo, i);
        // If new list does not contain element, add to new list
        if (!g_hash_table_lookup(newSet, osinfo_entity_get_id(entity))) {
            osinfo_list_add(list, entity);
            g_hash_table_insert(newSet, g_strdup(osinfo_entity_get_id(entity)), entity);
        }
    }

    g_hash_table_unref(newSet);
}

/**
 * osinfo_list_add_all:
 * @list: the entity list
 * @source: (transfer none): the list to add
 *
 * Adds all entities from @source to @list. Using one
 * of the constructors in a subclass is preferrable
 * to this method.
 */
void osinfo_list_add_all(OsinfoList *list, OsinfoList *source)
{
    int i, len;
    g_return_if_fail(list->priv->elementType == source->priv->elementType);

    len = osinfo_list_get_length(source);
    for (i = 0; i < len; i++) {
        OsinfoEntity *entity = osinfo_list_get_nth(source, i);
        osinfo_list_add(list, entity);
    }
}

/*
 * Creates a list of the same type as sourceOne and sourceTwo after
 * checking they are the same type. The created list elements are
 * of the same type as the elements of sourceOne and sourceTwo
 */
static OsinfoList *osinfo_list_new_same(OsinfoList *sourceOne,
                                        OsinfoList *sourceTwo)
{
    GType typeOne = G_OBJECT_TYPE(sourceOne);

    if (sourceTwo != NULL) {
        GType typeTwo = G_OBJECT_TYPE(sourceTwo);

        g_return_val_if_fail(typeOne == typeTwo, NULL);
        g_return_val_if_fail(OSINFO_IS_LIST(sourceTwo), NULL);
    }

    g_return_val_if_fail(OSINFO_IS_LIST(sourceOne), NULL);

    return g_object_new(typeOne,
                        "element-type",
                        sourceOne->priv->elementType,
                        NULL);
}

/**
 * osinfo_list_new_copy:
 * @source: the list to copy
 *
 * Construct a new list that is filled with elements from @source
 *
 * Returns: (transfer full): a copy of the list
 */
OsinfoList *osinfo_list_new_copy(OsinfoList *source)
{
    OsinfoList *newList = osinfo_list_new_same(source, NULL);
    g_return_val_if_fail(newList != NULL, NULL);
    osinfo_list_add_all(OSINFO_LIST(newList),
                        OSINFO_LIST(source));
    return newList;
}

/**
 * osinfo_list_new_filtered:
 * @source: the list to copy
 * @filter: the filter to apply
 *
 * Construct a new list that is filled with elements from @source that
 * match @filter
 *
 * Returns: (transfer full): a filtered copy of the list
 */
OsinfoList *osinfo_list_new_filtered(OsinfoList *source, OsinfoFilter *filter)
{
    OsinfoList *newList = osinfo_list_new_same(source, NULL);
    g_return_val_if_fail(newList != NULL, NULL);
    osinfo_list_add_filtered(OSINFO_LIST(newList),
                             OSINFO_LIST(source),
                             filter);
    return newList;
}

/**
 * osinfo_list_new_intersection:
 * @sourceOne: the first list to copy
 * @sourceTwo: the second list to copy
 *
 * Construct a new list that is filled with only the elements
 * that are present in both @sourceOne and @sourceTwo.
 *
 * Returns: (transfer full): an intersection of the two lists
 */
OsinfoList *osinfo_list_new_intersection(OsinfoList *sourceOne,
                                         OsinfoList *sourceTwo)
{
    OsinfoList *newList = osinfo_list_new_same(sourceOne, sourceTwo);
    g_return_val_if_fail(newList != NULL, NULL);
    osinfo_list_add_intersection(OSINFO_LIST(newList),
                                 OSINFO_LIST(sourceOne),
                                 OSINFO_LIST(sourceTwo));
    return newList;
}

/**
 * osinfo_list_new_union:
 * @sourceOne: the first list to copy
 * @sourceTwo: the second list to copy
 *
 * Construct a new list that is filled with all the that are present in
 * either @sourceOne and @sourceTwo. @sourceOne and @sourceTwo must be of
 * the same type.
 *
 * Returns: (transfer full): a union of the two lists
 */
OsinfoList *osinfo_list_new_union(OsinfoList *sourceOne,
                                  OsinfoList *sourceTwo)
{
    OsinfoList *newList = osinfo_list_new_same(sourceOne, sourceTwo);
    g_return_val_if_fail(newList != NULL, NULL);
    osinfo_list_add_union(OSINFO_LIST(newList),
                          OSINFO_LIST(sourceOne),
                          OSINFO_LIST(sourceTwo));
    return newList;
}


/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */
