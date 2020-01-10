# -*- rpm-spec -*-

# Plugin isn't ready for real world use yet - it needs
# a security audit at very least
%define with_plugin 0

%define with_gir 0

%if 0%{?fedora} >= 15 || 0%{?rhel} >= 7
%define with_gir 1
%endif

%define with_udev 1
%if 0%{?fedora} >= 19 || 0%{?rhel} >= 7
%define with_udev 0
%endif

Summary: A library for managing OS information for virtualization
Name: libosinfo
Version: 0.2.12
Release: 3%{?dist}%{?extra_release}
License: LGPLv2+
Group: Development/Libraries
Source: https://fedorahosted.org/releases/l/i/%{name}/%{name}-%{version}.tar.gz

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
URL: http://libosinfo.org/
BuildRequires: intltool
BuildRequires: glib2-devel
BuildRequires: check-devel
BuildRequires: libxml2-devel >= 2.6.0
BuildRequires: libxslt-devel >= 1.0.0
BuildRequires: vala
BuildRequires: vala-tools
BuildRequires: libsoup-devel
BuildRequires: /usr/bin/pod2man
%if %{with_gir}
BuildRequires: gobject-introspection-devel
%endif
Requires: hwdata
%if %{with_udev}
Requires: udev
%endif

# https://bugzilla.redhat.com/show_bug.cgi?id=1032524
# Don't advertise unsigned drivers from unsecure locations
Patch0: rm-unsigned-drivers-from-unsecure-locations.patch

# https://bugzilla.redhat.com/show_bug.cgi?id=1197236
# libosinfo support for RHELSA (7.2)
Patch1: rhel-Add-data-about-RHEL-7.1-7.2.patch

%description
libosinfo is a library that allows virtualization provisioning tools to
determine the optimal device settings for a hypervisor/operating system
combination.

%package devel
Summary: Libraries, includes, etc. to compile with the libosinfo library
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
Requires: pkgconfig
Requires: glib2-devel

%description devel
libosinfo is a library that allows virtualization provisioning tools to
determine the optimal device settings for a hypervisor/operating system
combination.

Libraries, includes, etc. to compile with the libosinfo library

%package vala
Summary: Vala bindings
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description vala
libosinfo is a library that allows virtualization provisioning tools to
determine the optimal device settings for a hypervisor/operating system
combination.

This package provides the Vala bindings for libosinfo library.

%prep
%setup -q

%patch0 -p1 -b .rm-unsigned-drivers-from-unsecure-locations
%patch1 -p1 -b .rhel-Add-data-about-RHEL-7.1-7.2

%build
%if %{with_gir}
%define gir_arg --enable-introspection=yes
%else
%define gir_arg --enable-introspection=no
%endif

%if %{with_udev}
%define udev_arg --enable-udev=yes
%else
%define udev_arg --enable-udev=no
%endif

%configure %{gir_arg} %{udev_arg} --enable-vala=yes --with-usb-ids-path=/usr/share/hwdata/usb.ids --with-pci-ids-path=/usr/share/hwdata/pci.ids
%__make %{?_smp_mflags} V=1

chmod a-x examples/*.js examples/*.py

%install
rm -fr %{buildroot}
%__make install DESTDIR=%{buildroot}
rm -f %{buildroot}%{_libdir}/*.a
rm -f %{buildroot}%{_libdir}/*.la

%find_lang %{name}

%check
make check

%clean
rm -fr %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files -f %{name}.lang
%defattr(-, root, root)
%doc AUTHORS ChangeLog COPYING.LIB NEWS README
%{_bindir}/osinfo-detect
%{_bindir}/osinfo-db-validate
%{_bindir}/osinfo-query
%{_bindir}/osinfo-install-script
%dir %{_datadir}/libosinfo/
%dir %{_datadir}/libosinfo/db/
%dir %{_datadir}/libosinfo/schemas/
%{_datadir}/libosinfo/db/usb.ids
%{_datadir}/libosinfo/db/pci.ids
%{_datadir}/libosinfo/db/datamaps
%{_datadir}/libosinfo/db/devices
%{_datadir}/libosinfo/db/oses
%{_datadir}/libosinfo/db/hypervisors
%{_datadir}/libosinfo/db/install-scripts
%{_datadir}/libosinfo/schemas/libosinfo.rng
%{_mandir}/man1/osinfo-db-validate.1*
%{_mandir}/man1/osinfo-detect.1*
%{_mandir}/man1/osinfo-query.1*
%{_mandir}/man1/osinfo-install-script.1*
%{_libdir}/%{name}-1.0.so.*
%if %{with_udev}
/lib/udev/rules.d/95-osinfo.rules
%endif
%if %{with_gir}
%{_libdir}/girepository-1.0/Libosinfo-1.0.typelib
%endif

%files devel
%defattr(-, root, root)
%doc examples/demo.js
%doc examples/demo.py
%{_libdir}/%{name}-1.0.so
%dir %{_includedir}/%{name}-1.0/
%dir %{_includedir}/%{name}-1.0/osinfo/
%{_includedir}/%{name}-1.0/osinfo/*.h
%{_libdir}/pkgconfig/%{name}-1.0.pc
%if %{with_gir}
%{_datadir}/gir-1.0/Libosinfo-1.0.gir
%endif
%{_datadir}/gtk-doc/html/Libosinfo

%files vala
%defattr(-, root, root)
%{_datadir}/vala/vapi/libosinfo-1.0.vapi

%changelog
* Wed Aug 19 2015 Zeeshan Ali <zeenix@redhat.com> - 0.2.12-3
- Bump relase to get the new build out. (#1197236).

* Wed Aug 19 2015 Zeeshan Ali <zeenix@redhat.com> - 0.2.12-2
- Add data about RHEL 7.1 & 7.2 (#1197236).

* Thu May 28 2015 Zeeshan Ali <zeenix@redhat.com> - 0.2.12-1
- Update to 0.2.12 (#1174552).
- Remove now redundant patches.

* Sat May 16 2015 Zeeshan Ali <zeenix@redhat.com> - 0.2.11-5
- Merge RHELSA branch (#1197236).

* Mon Jan 26 2015 Zeeshan Ali <zeenix@redhat.com> - 0.2.11-4
- Add/update F21 info (#1169415).

* Mon Jan 26 2015 Zeeshan Ali <zeenix@redhat.com> - 0.2.11-3
- Don't specify network interface in fedora script (#1169415).

* Thu Jan  8 2015 Zeeshan Ali <zeenix@redhat.com> - 0.2.11-2
- Add rhel-5.9..11 and rhel-6.6 info (#1168199).

* Wed Sep  3 2014 Zeeshan Ali <zeenix@redhat.com> - 0.2.11-1
- Update to latest libosinfo release (#1128621).

* Fri Feb 28 2014 Zeeshan Ali <zeenix@redhat.com> - 0.2.7-10
- Rebuild with -fstack-protector-strong (#1070773).

* Fri Feb 28 2014 Zeeshan Ali <zeenix@redhat.com> - 0.2.7-9
- Remove unsigned drivers from unsecure locations (#1032524).

* Mon Feb 24 2014 Zeeshan Ali <zeenix@redhat.com> - 0.2.7-8
- Disable unattended installation scripts for Windows 8.1 (#1067431).

* Tue Feb 18 2014 Zeeshan Ali <zeenix@redhat.com> - 0.2.7-7
- Support windows 8.1 (#1066042).

* Fri Jan 24 2014 Daniel Mach <dmach@redhat.com> - 0.2.7-6
- Mass rebuild 2014-01-24

* Tue Jan  7 2014 Zeeshan Ali <zeenix@redhat.com> - 0.2.7-5
- fedora,win7: Improve & add data (#1047774).

* Fri Dec 27 2013 Daniel Mach <dmach@redhat.com> - 0.2.7-4
- Mass rebuild 2013-12-27

* Thu Dec  5 2013 Zeeshan Ali <zeenix@redhat.com> - 0.2.7-3
- rhel: Add info on 6.5 and 7.0 (#1015055).
- install-script,rhel: Fix for 'Server' & 7.0 (#1033007).

* Fri Jul 19 2013 Zeeshan Ali <zeenix@redhat.com> - 0.2.7-2
- Don't install udev rule in RHEL7+ either (#983169).

* Tue May 14 2013 Zeeshan Ali <zeenix@redhat.com> - 0.2.7-1
- New upstream release 0.2.7

* Thu Mar 21 2013 Zeeshan Ali <zeenix@redhat.com> - 0.2.6-1
- New upstream release 0.2.6

* Wed Mar 06 2013 Christophe Fergeau <cfergeau@redhat.com> - 0.2.5-2
- BuildRequires /usr/bin/pod2man as this will automatically pick the right
  package rather than conditionally requiring a package that is only
  available in f19+
- Do not Requires: udev when building libosinfo without its udev rule
  (which is done on f19+)

* Tue Mar 05 2013 Christophe Fergeau <cfergeau@redhat.com> 0.2.5-1
- New upstream release 0.2.5
- Disable udev rule as it's no longer required with newer
  systemd/util-linux

* Tue Feb 12 2013 Cole Robinson <crobinso@redhat.com> - 0.2.3-2
- Fix osinfo-detect crash with non-bootable media (bz #901910)

* Mon Jan 14 2013 Zeeshan Ali <zeenix@redhat.com> - 0.2.3-1
- New upstream release 0.2.3

* Thu Dec 20 2012 Christophe Fergeau <cfergeau@redhat.com> - 0.2.2-1
- New upstream release 0.2.2

* Fri Oct 12 2012 Zeeshan Ali <zeenix@redhat.com> - 0.2.1-1
- Fix and simplify udev rule.
- Fedora:
  - Fix minimum RAM requirements for F16 and F17.
- Add data on:
  - Fedora 18
  - GNOME 3.6
  - Ubuntu 12.10
- Fixes to doc build.
- Install script:
  - Add get_config_param method.
  - Differenciate between expected/output script names.
  - Add more utility functions.
- Add 'installer-reboots' parameter to medias.
- osinfo-detect does not die of DB loading errors anymore.
- More type-specific entity value getters/setters.
- Fixe and update RNG file.
- Add 'subsystem' property/attribute to devices.

* Mon Sep 03 2012 Christophe Fergeau <cfergeau@redhat.com> - 0.2.0-1
- Update to 0.2.0 release.

* Thu Jul 19 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.1.2-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_18_Mass_Rebuild

* Tue Jun 12 2012 Zeeshan Ali <zeenix@redhat.com> - 0.1.2-1
- Update to 0.1.2 release.

* Thu Apr 12 2012 Zeeshan Ali <zeenix@redhat.com> - 0.1.1-1
- Update to 0.1.1 release.

* Wed Mar 14 2012 Daniel P. Berrange <berrange@redhat.com> - 0.1.0-2
- Remove obsolete perl based scripts (rhbz #803086)

* Wed Feb 08 2012 Christophe Fergeau <cfergeau@redhat.com> - 0.1.0-1
- Update to 0.1.0 release

* Tue Jan  17 2012 Zeeshan Ali <zeenix@redhat.com> - 0.0.5-1
- Update to 0.0.5 release

* Tue Jan  3 2012 Daniel P. Berrange <berrange@redhat.com> - 0.0.4-2
- Remove pointless gir conditionals

* Wed Dec 21 2011 Daniel P. Berrange <berrange@redhat.com> - 0.0.4-1
- Update to 0.0.4 release

* Thu Nov 24 2011 Daniel P. Berrange <berrange@redhat.com> - 0.0.2-1
- Initial package

