Name:       capi-system-haptic
Summary:    A Haptic library in Tizen Native API
Version:    0.1.0
Release:    11
Group:      TO_BE/FILLED_IN
License:    Apache-2.0
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
Summary:  A Haptic library in Tizen Native API (Development)
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


