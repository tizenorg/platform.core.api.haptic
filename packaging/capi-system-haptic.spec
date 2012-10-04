#sbs-git:slp/api/haptic capi-system-haptic 0.1.0 0092e2c55dbf60717ae8f43aee931f2dd1c57157
Name:       capi-system-haptic
Summary:    A Haptic library in Tizen C API
Version:    0.1.0
Release:    21
Group:      TO_BE/FILLED_IN
License:    TO BE FILLED IN
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(devman)
BuildRequires:  pkgconfig(devman_haptic)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(devman)
BuildRequires:  pkgconfig(dlog)

Requires(post): /sbin/ldconfig  
Requires(postun): /sbin/ldconfig

%description


%package devel
Summary:  A Haptic library in Tizen C API (Development)
Group:    TO_BE/FILLED_IN
Requires: %{name} = %{version}-%{release}

%description devel



%prep
%setup -q


%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%{_libdir}/libcapi-system-haptic.so.*

%files devel
%{_includedir}/system/*.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-haptic.so

%changelog
* Thu Oct 04 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- fix the bug regarding haptic_level_auto
- Tag : capi-system-haptic_0.1.0-21

* Fri Sep 21 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- Add smack label
- Tag : capi-system-haptic_0.1.0-20

* Thu Sep 06 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- fix TC error
- Tag : capi-system-haptic_0.1.0-19

* Thu Aug 16 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- modify the function parameter to control detail feedback level
- Tag : capi-system-haptic_0.1.0-17

* Thu Aug 09 2012 - Jiyoung Yun <jy910.yun@samsung.com>
- Revert "add level parameter to monotone vibration function."
- Tag : capi-system-haptic_0.1.0-16
