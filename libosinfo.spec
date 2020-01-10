# -*- rpm-spec -*-

Summary: A library for managing OS information for virtualization
Name: libosinfo
Version: 1.1.0
Release: 3%{?dist}%{?extra_release}
License: LGPLv2+
Group: Development/Libraries
Source: https://releases.pagure.io/%{name}/%{name}-%{version}.tar.gz
URL: https://libosinfo.org/

### Patches ###
# https://bugzilla.redhat.com/show_bug.cgi?id=1712458
Patch0001: 0001-Use-g_list_free_full-where-easily-possible.patch
Patch0002: 0002-loader-Replace-strcmp-with-g_str_equal.patch
Patch0003: 0003-loader-properly-load-the-treeinfo-attributes.patch
Patch0004: 0004-db-improve-_guess_os_from_tree-checks.patch
Patch0005: 0005-tree-cleanup-load_key_info.patch
Patch0006: 0006-tree-cleanup-non-fatal-errors-in-load_key_info.patch
Patch0007: 0007-tree-Also-check-fore-treeinfo-in-addition-to-.treein.patch
Patch0008: 0008-tree-Avoid-use-of-memory-after-it-s-freed.patch
Patch0009: 0009-tree-Cleanup-_create_from_location_async_helper.patch
Patch0010: 0010-db-improve-_guess_os_from_media-checks.patch

BuildRequires: intltool
BuildRequires: glib2-devel
BuildRequires: check-devel
BuildRequires: libxml2-devel >= 2.6.0
BuildRequires: libxslt-devel >= 1.0.0
BuildRequires: vala
BuildRequires: vala-tools
BuildRequires: libsoup-devel
BuildRequires: /usr/bin/pod2man
BuildRequires: hwdata
BuildRequires: gobject-introspection-devel
BuildRequires: osinfo-db
Requires: hwdata
Requires: osinfo-db
Requires: osinfo-db-tools

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
for p in %patches ; do
    %__patch -p1 -i $p
done

%build
%configure --enable-introspection=yes --enable-vala=yes
%__make %{?_smp_mflags} V=1

chmod a-x examples/*.js examples/*.py

%install
rm -fr %{buildroot}
%__make install DESTDIR=%{buildroot}
rm -f %{buildroot}%{_libdir}/*.a
rm -f %{buildroot}%{_libdir}/*.la

%find_lang %{name}

%clean
rm -fr %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files -f %{name}.lang
%defattr(-, root, root)
%doc AUTHORS ChangeLog COPYING.LIB NEWS README
%{_bindir}/osinfo-detect
%{_bindir}/osinfo-query
%{_bindir}/osinfo-install-script
%{_mandir}/man1/osinfo-detect.1*
%{_mandir}/man1/osinfo-query.1*
%{_mandir}/man1/osinfo-install-script.1*
%{_libdir}/%{name}-1.0.so.*
%{_libdir}/girepository-1.0/Libosinfo-1.0.typelib

%files devel
%defattr(-, root, root)
%doc examples/demo.js
%doc examples/demo.py
%{_libdir}/%{name}-1.0.so
%dir %{_includedir}/%{name}-1.0/
%dir %{_includedir}/%{name}-1.0/osinfo/
%{_includedir}/%{name}-1.0/osinfo/*.h
%{_libdir}/pkgconfig/%{name}-1.0.pc
%{_datadir}/gir-1.0/Libosinfo-1.0.gir
%{_datadir}/gtk-doc/html/Libosinfo

%files vala
%defattr(-, root, root)
%{_datadir}/vala/vapi/libosinfo-1.0.vapi

%changelog
* Thu May 23 2019 Fabiano Fidêncio <fidencio@redhat.com> - 1.1.0-3
- Resolves: rhbz#1712458 - [machines] The function of 'Auto-detect guest
                           operating system' is not available on rhel 7.7

* Wed Jun 06 2018 Richard Hughes <rhughes@redhat.com> 1.1.0-2
- New upstream release 1.1.0
- Resolves: #1584263

* Thu Feb 23 2017 Matthias Clasen <mclasen@redhat.com> 1.0.0-1
- Rebase to 1.0.0
  Resolves: rhbz#1387014

* Fri Jul  1 2016 Daniel P. Berrange <berrange@redhat.com> 0.3.1-1
- New upstream release 0.3.1

* Thu Feb 04 2016 Fedora Release Engineering <releng@fedoraproject.org> - 0.3.0-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_24_Mass_Rebuild

* Fri Jan  8 2016 Zeeshan Ali <zeenix@redhat.com> 0.3.0-1
- New upstream release 0.3.0

* Wed Jun 17 2015 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.12-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_23_Mass_Rebuild

* Thu May 28 2015 Zeeshan Ali <zeenix@redhat.com> 0.2.12-1
- New upstream release 0.2.12

* Mon Sep 22 2014 Cole Robinson <crobinso@redhat.com> - 0.2.11-2
- os: Add Fedora 21

* Tue Aug 26 2014 Christophe Fergeau <cfergeau@redhat.com> 0.2.11-1
- New upstream release 0.2.11

* Sun Aug 17 2014 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.9-4
- Rebuilt for https://fedoraproject.org/wiki/Fedora_21_22_Mass_Rebuild

* Tue Jul 22 2014 Kalev Lember <kalevlember@gmail.com> - 0.2.9-3
- Rebuilt for gobject-introspection 1.41.4

* Sat Jun 07 2014 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.9-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_21_Mass_Rebuild

* Wed Dec 18 2013 Debarshi Ray <rishi@fedoraproject.org> - 0.2.9-1
- New upstream release 0.2.9

* Thu Nov 28 2013 Zeeshan Ali <zeenix@redhat.com> - 0.2.8-1
- New upstream release 0.2.8

* Sat Aug 03 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.7-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_20_Mass_Rebuild

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

