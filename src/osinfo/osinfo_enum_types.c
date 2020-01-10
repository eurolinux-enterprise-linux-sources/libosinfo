
/* Generated data (by glib-mkenums) */

#include <osinfo/osinfo.h>

/* enumerations from "./osinfo_install_config_param.h" */
GType
osinfo_install_config_param_policy_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { OSINFO_INSTALL_CONFIG_PARAM_POLICY_NONE, "OSINFO_INSTALL_CONFIG_PARAM_POLICY_NONE", "none" },
        { OSINFO_INSTALL_CONFIG_PARAM_POLICY_REQUIRED, "OSINFO_INSTALL_CONFIG_PARAM_POLICY_REQUIRED", "required" },
        { OSINFO_INSTALL_CONFIG_PARAM_POLICY_OPTIONAL, "OSINFO_INSTALL_CONFIG_PARAM_POLICY_OPTIONAL", "optional" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("OsinfoInstallConfigParamPolicy"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
osinfo_path_format_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { OSINFO_PATH_FORMAT_UNIX, "OSINFO_PATH_FORMAT_UNIX", "unix" },
        { OSINFO_PATH_FORMAT_DOS, "OSINFO_PATH_FORMAT_DOS", "dos" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("OsinfoPathFormat"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
osinfo_device_driver_signing_req_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { OSINFO_DEVICE_DRIVER_SIGNING_REQ_NONE, "OSINFO_DEVICE_DRIVER_SIGNING_REQ_NONE", "none" },
        { OSINFO_DEVICE_DRIVER_SIGNING_REQ_STRICT, "OSINFO_DEVICE_DRIVER_SIGNING_REQ_STRICT", "strict" },
        { OSINFO_DEVICE_DRIVER_SIGNING_REQ_WARN, "OSINFO_DEVICE_DRIVER_SIGNING_REQ_WARN", "warn" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("OsinfoDeviceDriverSigningReq"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
osinfo_install_script_injection_method_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { OSINFO_INSTALL_SCRIPT_INJECTION_METHOD_CDROM, "OSINFO_INSTALL_SCRIPT_INJECTION_METHOD_CDROM", "cdrom" },
        { OSINFO_INSTALL_SCRIPT_INJECTION_METHOD_DISK, "OSINFO_INSTALL_SCRIPT_INJECTION_METHOD_DISK", "disk" },
        { OSINFO_INSTALL_SCRIPT_INJECTION_METHOD_FLOPPY, "OSINFO_INSTALL_SCRIPT_INJECTION_METHOD_FLOPPY", "floppy" },
        { OSINFO_INSTALL_SCRIPT_INJECTION_METHOD_INITRD, "OSINFO_INSTALL_SCRIPT_INJECTION_METHOD_INITRD", "initrd" },
        { OSINFO_INSTALL_SCRIPT_INJECTION_METHOD_WEB, "OSINFO_INSTALL_SCRIPT_INJECTION_METHOD_WEB", "web" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("OsinfoInstallScriptInjectionMethod"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
osinfo_product_relationship_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { OSINFO_PRODUCT_RELATIONSHIP_DERIVES_FROM, "OSINFO_PRODUCT_RELATIONSHIP_DERIVES_FROM", "derives-from" },
        { OSINFO_PRODUCT_RELATIONSHIP_UPGRADES, "OSINFO_PRODUCT_RELATIONSHIP_UPGRADES", "upgrades" },
        { OSINFO_PRODUCT_RELATIONSHIP_CLONES, "OSINFO_PRODUCT_RELATIONSHIP_CLONES", "clones" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("OsinfoProductRelationship"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
osinfo_release_status_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { OSINFO_RELEASE_STATUS_RELEASED, "OSINFO_RELEASE_STATUS_RELEASED", "released" },
        { OSINFO_RELEASE_STATUS_SNAPSHOT, "OSINFO_RELEASE_STATUS_SNAPSHOT", "snapshot" },
        { OSINFO_RELEASE_STATUS_PRERELEASE, "OSINFO_RELEASE_STATUS_PRERELEASE", "prerelease" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("OsinfoReleaseStatus"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}

GType
osinfo_media_error_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GEnumValue values[] = {
        { OSINFO_MEDIA_ERROR_NO_DESCRIPTORS, "OSINFO_MEDIA_ERROR_NO_DESCRIPTORS", "no-descriptors" },
        { OSINFO_MEDIA_ERROR_NO_PVD, "OSINFO_MEDIA_ERROR_NO_PVD", "no-pvd" },
        { OSINFO_MEDIA_ERROR_NO_SVD, "OSINFO_MEDIA_ERROR_NO_SVD", "no-svd" },
        { OSINFO_MEDIA_ERROR_INSUFFICIENT_METADATA, "OSINFO_MEDIA_ERROR_INSUFFICIENT_METADATA", "insufficient-metadata" },
        { OSINFO_MEDIA_ERROR_NOT_BOOTABLE, "OSINFO_MEDIA_ERROR_NOT_BOOTABLE", "not-bootable" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_enum_register_static (g_intern_static_string ("OsinfoMediaError"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}


/* Generated data ends here */

