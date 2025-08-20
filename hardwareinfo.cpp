#include "hardwareinfo.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>

QString HardwareInfo::executeCommand(const QString &command, const QStringList &args)
{
    QProcess process;
    process.start(command, args);
    process.waitForFinished(3000); // 3 second timeout

    QByteArray error = process.readAllStandardError();
    if (process.exitCode() != 0 || !error.isEmpty()) {
        qWarning() << "Command failed:" << command << args.join(" ");
        qWarning() << "Stderr:" << QString::fromLocal8Bit(error);
        return {};
    }

    return QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed();
}

QString HardwareInfo::getCpuInfo()
{
    // /proc/cpuinfo is readable by all users.
    QString output = executeCommand("cat", QStringList() << "/proc/cpuinfo");
    QRegularExpression re(R"(model name\s+:\s+(.*))");
    QRegularExpressionMatch match = re.match(output);
    if (match.hasMatch()) {
        return match.captured(1).trimmed();
    }
    return "N/A";
}

QString HardwareInfo::getMotherboardSerial()
{
    // Reading the DMI table from /sys is the standard non-root way.
    // This file should contain the motherboard UUID.
    QString output = executeCommand("cat", QStringList() << "/sys/class/dmi/id/product_uuid");
    if (output.isEmpty()) {
        return "N/A (requires root for dmidecode as fallback)";
    }
    return output;
}

QString HardwareInfo::getHardDiskSerial()
{
    // We check common device names. lsblk can give us a list of block devices.
    QStringList devices = executeCommand("lsblk", QStringList() << "-d" << "-n" << "-o" << "NAME").split('\n');
    for (const QString &dev : devices) {
        if (dev.startsWith("sd") || dev.startsWith("hd") || dev.startsWith("nvme")) {
             // /sys/block/DEVICENAME/device/serial is often available
             QString serial = executeCommand("cat", QStringList() << QString("/sys/block/%1/device/serial").arg(dev));
             if (!serial.isEmpty()) {
                 return serial;
             }
        }
    }
    return "N/A (no serial found in /sys)";
}

QStringList HardwareInfo::getMacAddresses()
{
    QStringList macs;
    // `ip link` is the modern tool for this.
    QString output = executeCommand("ip", QStringList() << "link");
    QRegularExpression re("link/ether ([0-9a-fA-F:]{17})");
    auto it = re.globalMatch(output);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        macs.append(match.captured(1));
    }
    return macs;
}
