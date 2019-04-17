# norootforbuild
%{!?python_sitearch: %global python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(1))")}

%define __cmake %{_bindir}/cmake
%define _cmake_lib_suffix64 -DLIB_SUFFIX=64
%define cmake \
CFLAGS="${CFLAGS:-%optflags}" ; export CFLAGS ; \
CXXFLAGS="${CXXFLAGS:-%optflags}" ; export CXXFLAGS ; \
FFLAGS="${FFLAGS:-%optflags}" ; export FFLAGS ; \
%__cmake \\\
-DCMAKE_INSTALL_PREFIX:PATH=%{_prefix} \\\
%if "%{?_lib}" == "lib64" \
%{?_cmake_lib_suffix64} \\\
%endif \
-DBUILD_SHARED_LIBS:BOOL=ON

Name:           otgui
Version:        6.1
Release:        0%{?dist}
Summary:        GUI for OpenTURNS
Group:          System Environment/Libraries
License:        LGPLv3+
URL:            http://www.openturns.org/
Source0:        http://downloads.sourceforge.net/openturns-modules/otgui/otgui-%{version}.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:  gcc-c++, cmake, swig
BuildRequires:  openturns-devel
BuildRequires:  python-openturns
BuildRequires:  python-devel
%if 0%{?suse_version}
BuildRequires:  qwt6-devel
%else
BuildRequires:  qwt-devel >= 6
%endif
Requires:       libotgui0

%description
GUI module for OpenTURNS library.

%package -n libotgui0
Summary:        OTGui development files
Group:          Development/Libraries/C and C++

%description -n libotgui0
Dynamic libraries for OTGui.

%package devel
Summary:        OTGui development files
Group:          Development/Libraries/C and C++
Requires:       libotgui0 = %{version}
Requires:       openturns-devel

%description devel
Development files for OTGui library.

%package examples
Summary:        OTGui examples
Group:          Productivity/Scientific/Math

%description examples
Example files for OTGui

%package -n python-%{name}
Summary:        OTGui library
Group:          Productivity/Scientific/Math
Requires:       python-openturns
%description -n python-%{name}
Python textual interface to OTGui uncertainty library

%prep
%setup -q

%build
%cmake -DCMAKE_INSTALL_PREFIX=/usr \
       -DINSTALL_DESTDIR:PATH=%{buildroot} \
       -DUSE_SPHINX=OFF \
       -DUSE_SALOME=OFF .
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

%check
make tests %{?_smp_mflags}
ctest %{?_smp_mflags} || cat Testing/Temporary/LastTest.log
rm %{buildroot}%{python_sitearch}/%{name}base/*.pyc

%clean
rm -rf %{buildroot}

%post -n libotgui0 -p /sbin/ldconfig 
%postun -n libotgui0 -p /sbin/ldconfig 

%files -n libotgui0
%defattr(-,root,root,-)
%{_libdir}/*.so.*
%{_bindir}/otgui

%files devel
%defattr(-,root,root,-)
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*.h*
%{_includedir}/%{name}/swig/
%{_libdir}/*.so
%{_libdir}/cmake/

%files examples
%defattr(-,root,root,-)
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/examples/

%files -n python-%{name}
%defattr(-,root,root,-)
%{python_sitearch}/%{name}base


%changelog
* Wed Nov 28 2012 Julien Schueller <schueller at phimeca dot com> 0.0-1
- Initial package creation

