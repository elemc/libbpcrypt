Name:           libbpcrypt
Version:        0.1.1
Release:        1%{?dist}
Summary:        Library for basket passwords file read and write

License:        GPLv3
URL:            https://bitbucket.org/elemc
Source0:        https://repo.elemc.name/sources/%{name}-%{version}.tar.xz

BuildRequires:  libxml2-devel openssl-devel cmake

%description
Library for Basket of passwords file read and write

%package        devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.


%prep
%autosetup

%build
%cmake -DWITH_OLD_CRYPT=ON -DCMAKE_BUILD_TYPE=Release -DWITH_XML=ON .
%make_build


%install
rm -rf $RPM_BUILD_ROOT
%make_install
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'

%check
ctest -V %{?_smp_mflags}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%license LICENSE
%doc README.md
%{_libdir}/*.so.*

%files devel
%doc README.md
%{_includedir}/*
%{_libdir}/*.so


%changelog
* Wed Jun 14 2017 Alexei Panov <me AT elemc DOT name> 0.1.1-1
- Fixed openssl struct to pointer for gcc 7

* Tue Sep 27 2016 Alexei Panov <me@elemc.name>
- Initial build
