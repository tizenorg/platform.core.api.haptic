Name:       capi-system-haptic
Summary:    A Haptic library in Tizen C API
Version:    0.1.0
Release:    23
Group:      System/API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1:	capi-system-haptic.manifest
BuildRequires:  cmake
BuildRequires:  pkgconfig(devman)
BuildRequires:  pkgconfig(devman_haptic)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(devman)
BuildRequires:  pkgconfig(dlog)

%description
A Haptic library in Tizen C API.


%package devel
Summary:  A Haptic library in Tizen C API (Development)
Group:    Development/System
Requires: %{name} = %{version}-%{release}

%description devel
%devel_desc

%prep
%setup -q


%build
cp %{SOURCE1} .
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER}


make %{?jobs:-j%jobs}

%install
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest %{name}.manifest
%license LICENSE
%manifest capi-system-haptic.manifest
%{_libdir}/libcapi-system-haptic.so.*

%files devel
%manifest %{name}.manifest
%{_includedir}/system/*.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-haptic.so

%changelog
