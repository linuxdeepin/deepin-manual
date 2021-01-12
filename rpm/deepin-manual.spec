%define specrelease 1%{?dist}
%if 0%{?openeuler}
%define specrelease 1
%endif


Name:           deepin-manual
Version:        5.7.0.41
Release:        %{specrelease}
Summary:        Manual is designed to help users learn the operating system and its applications, providing specific instructions and function descriptions
License:        GPLv3+
URL:            https://github.com/linuxdeepin/%{name}
Source0:        %{url}/archive/%{version}/%{name}-%{version}.tar.gz


BuildRequires: gcc-c++
BuildRequires: cmake3
BuildRequires: qt5-devel

BuildRequires: pkgconfig(dtkwidget)
BuildRequires: pkgconfig(dtkgui)
BuildRequires: pkgconfig(dtkcore)
BuildRequires: qt5-qtwebengine-devel
BuildRequires: qt5-qtwebchannel-devel
BuildRequires: qt5-linguist
BuildRequires: qt5-qtx11extras-devel


%description
%{Summary}.


%prep
%autosetup 
# %autosetup -c -n %{name}-%{version}

%build
# help find (and prefer) qt5 utilities, e.g. qmake, lrelease
export PATH=%{_qt5_bindir}:$PATH
# cmake_minimum_required version is too high
sed -i "s|^cmake_minimum_required.*|cmake_minimum_required(VERSION 3.0)|" $(find . -name "CMakeLists.txt")
mkdir build && pushd build
pwd
%cmake -DCMAKE_BUILD_TYPE=Release -DAPP_VERSION=%{version} -DVERSION=%{version}  ../CMakeLists.txt 
pwd
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
%{_datadir}/%{name}/manual-assets/deepin-manual.svg
%{_datadir}/%{name}/translations/*.qm
%{_datadir}/deepin-manual/manual-assets/server/*
%doc README.md
%license LICENSE


%changelog
* Fri Nov 06 2020 guoqinglan <guoqinglan@uniontech.com> - 5.6.45-1
- update.

* Wed Nov 04 2020 guoqinglan <guoqinglan@uniontech.com> - 5.6.43-1
- update.

* Wed Oct 28 2020 panchenbo <panchenbo@uniontech.com> - 5.6.42-1
- update.

* Thu May 28 2020 uoser <uoser@uniontech.com> - 5.6.7-3
- Project init.
