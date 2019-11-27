#include "sysinfo.h"

#include <DSysInfo>

#include <sys/utsname.h>

SysInfo::SysInfo(QObject *parent)
    : QObject(parent)
{

}

QString SysInfo::arch() const
{
    struct utsname name{};

    if (uname(&name) == -1) {
        return "";
    }

    auto machine = QString::fromLatin1(name.machine);

    // map arch
    auto archMap = QMap<QString, QString>{
        {"x86_64", "amd64"},
        {"aarch64", "arm64"},
        {"mips64", "mips64"},
    };
    auto arch = archMap[machine];
    if (arch.isEmpty()) {
        arch = machine;
    }
    return arch;
}

QString SysInfo::product() const
{
    const auto defaultProduct = "community";
    switch (Dtk::Core::DSysInfo::deepinType()) {
        case Dtk::Core::DSysInfo::DeepinDesktop:
            return "community";
        case Dtk::Core::DSysInfo::DeepinProfessional:
            return "professional";
        default:
            return defaultProduct;
    }
}

// desktop/tablet
QString SysInfo::desktopMode() const
{
    return "desktop";
}
