%define pkgrelease  1
%if 0%{?openeuler}
%define specrelease %{pkgrelease}
%else
## allow specrelease to have configurable %%{?dist} tag in other distribution
%define specrelease %{pkgrelease}%{?dist}
%endif


Name:           dde-calendar
Version:        5.6.32
Release:        %{specrelease}
Summary:        Manual is designed to help users learn the operating system and its applications, providing specific instructions and function descriptions
License:        GPLv3+
URL:            https://github.com/linuxdeepin/%{name}
Source0:        %{url}/archive/%{version}/%{name}-%{version}.tar.gz


BuildRequires: gcc-c++
BuildRequires: cmake
BuildRequires: qt5-devel

BuildRequires: pkgconfig(dtkwidget)
BuildRequires: pkgconfig(dtkgui)
BuildRequires: pkgconfig(dtkcore)


%description
%{Summary}.


%prep
%autosetup

%build
# help find (and prefer) qt5 utilities, e.g. qmake, lrelease
export PATH=%{_qt5_bindir}:$PATH
# cmake_minimum_required version is too high
sed -i "s|^cmake_minimum_required.*|cmake_minimum_required(VERSION 3.0)|" $(find . -name "CMakeLists.txt")
mkdir build && pushd build
%cmake -DCMAKE_BUILD_TYPE=Release ../
%make_build
popd

%install
%make_install -C build INSTALL_ROOT="%buildroot"

%files
%{_bindir}/dman
%{_bindir}/dman-search
%{_datadir}/applications/deepin-manual.desktop
%{_datadir}/dbus-1/services/com.deepin.Manual.Open.service
%{_datadir}/dbus-1/services/com.deepin.Manual.Search.service
%{_datadir}/deepin-manual/dman
%{_datadir}/deepin-manual/web_dist/index.css
%{_datadir}/deepin-manual/web_dist/index.html
%{_datadir}/deepin-manual/web_dist/index.js
%{_datadir}/deepin-manual/web_dist/qwebchannel.js
%{_datadir}/icons/hicolor/scalable/apps/deepin-manual.svg
%{_datadir}/deepin-manual/manual-assets/professional/*
%{_datadir}/%{name}/translations/*.qm
%{_datadir}/deepin-manual/manual-assets/server/*
%doc README.md
%license LICENSE
%{_bindir}/%{name}


%changelog
* Thu May 28 2020 uoser <uoser@uniontech.com> - 5.6.7-3
- Project init.
