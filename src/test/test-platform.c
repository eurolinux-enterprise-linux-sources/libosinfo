/*
 * Copyright (C) 2009-2012, 2014 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Authors:
 *   Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <stdlib.h>
#include <osinfo/osinfo.h>
#include <check.h>



START_TEST(test_basic)
{
    OsinfoPlatform *platform = osinfo_platform_new("awesome");

    fail_unless(OSINFO_IS_PLATFORM(platform), "Platform is a platform object");
    fail_unless(g_strcmp0(osinfo_entity_get_id(OSINFO_ENTITY(platform)), "awesome") == 0, "Platform ID was awesome");

    g_object_unref(platform);
}
END_TEST

START_TEST(test_devices)
{
    OsinfoPlatform *hv = osinfo_platform_new("awesome");
    OsinfoDevice *dev1 = osinfo_device_new("e1000");
    OsinfoDevice *dev2 = osinfo_device_new("rtl8139");

    OsinfoDeviceLink *link1 = osinfo_platform_add_device(hv, dev1);
    osinfo_entity_add_param(OSINFO_ENTITY(link1), "device", "pci-e1000");
    OsinfoDeviceLink *link2 = osinfo_platform_add_device(hv, dev2);
    osinfo_entity_add_param(OSINFO_ENTITY(link2), "device", "pci-8139");

    OsinfoDeviceList *devices = osinfo_platform_get_devices(hv, NULL);

    fail_unless(osinfo_list_get_length(OSINFO_LIST(devices)) == 2, "Platform has two devices");

    gboolean hasDev1 = FALSE;
    gboolean hasDev2 = FALSE;
    gboolean hasBad = FALSE;
    int i;
    for (i = 0; i < osinfo_list_get_length(OSINFO_LIST(devices)); i++) {
        OsinfoEntity *ent = osinfo_list_get_nth(OSINFO_LIST(devices), i);
        fail_unless(OSINFO_IS_DEVICE(ent), "entity is a device");
        if (OSINFO_DEVICE(ent) == dev1)
            hasDev1 = TRUE;
        else if (OSINFO_DEVICE(ent) == dev2)
            hasDev2 = TRUE;
        else
            hasBad = TRUE;
    }
    fail_unless(hasDev1, "Device 1 is missing");
    fail_unless(hasDev2, "Device 2 is missing");
    fail_unless(!hasBad, "Device was not expected");

    g_object_unref(devices);
    g_object_unref(dev1);
    g_object_unref(dev2);
    g_object_unref(hv);
}
END_TEST


START_TEST(test_devices_filter)
{
    OsinfoPlatform *hv = osinfo_platform_new("awesome");
    OsinfoDevice *dev1 = osinfo_device_new("e1000");
    OsinfoDevice *dev2 = osinfo_device_new("sb16");
    OsinfoFilter *filter = osinfo_filter_new();

    osinfo_entity_add_param(OSINFO_ENTITY(dev1), "class", "network");
    osinfo_entity_add_param(OSINFO_ENTITY(dev2), "class", "audio");

    OsinfoDeviceLink *link1 = osinfo_platform_add_device(hv, dev1);
    osinfo_entity_add_param(OSINFO_ENTITY(link1), "device", "pci-e1000");
    OsinfoDeviceLink *link2 = osinfo_platform_add_device(hv, dev2);
    osinfo_entity_add_param(OSINFO_ENTITY(link2), "device", "isa-sb16");

    osinfo_filter_add_constraint(filter, "class", "network");

    OsinfoDeviceList *devices = osinfo_platform_get_devices(hv, filter);

    fail_unless(osinfo_list_get_length(OSINFO_LIST(devices)) == 1, "Platform has one devices");
    OsinfoEntity *ent = osinfo_list_get_nth(OSINFO_LIST(devices), 0);
    fail_unless(OSINFO_IS_DEVICE(ent), "entity is a device");
    fail_unless(OSINFO_DEVICE(ent) == dev1, "device is e1000");

    g_object_unref(devices);
    g_object_unref(filter);
    g_object_unref(dev1);
    g_object_unref(dev2);
    g_object_unref(hv);
}
END_TEST



static Suite *
platform_suite(void)
{
    Suite *s = suite_create("Platform");
    TCase *tc = tcase_create("Core");
    tcase_add_test(tc, test_basic);
    tcase_add_test(tc, test_devices);
    tcase_add_test(tc, test_devices_filter);
    suite_add_tcase(s, tc);
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s = platform_suite();
    SRunner *sr = srunner_create(s);

#if !GLIB_CHECK_VERSION(2,35,1)
    g_type_init();
#endif

    /* Upfront so we don't confuse valgrind */
    osinfo_platform_get_type();
    osinfo_device_get_type();
    osinfo_devicelist_get_type();
    osinfo_filter_get_type();

    srunner_run_all(sr, CK_ENV);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */
