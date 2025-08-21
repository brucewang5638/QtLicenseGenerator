#include "hardwareinfo.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>

// 执行外部命令的辅助函数
QString HardwareInfo::executeCommand(const QString &command, const QStringList &args)
{
    // 创建一个 QProcess 对象来运行外部程序
    QProcess process;
    // 启动命令，并传入参数
    process.start(command, args);
    // 等待命令执行完成，设置了 3000 毫秒（3秒）的超时时间
    process.waitForFinished(3000); // 3 second timeout

    // 读取标准错误输出
    QByteArray error = process.readAllStandardError();
    // 检查命令是否成功执行（退出码为0）以及是否有错误输出
    if (process.exitCode() != 0 || !error.isEmpty()) {
        // 如果失败，使用 qWarning 打印错误信息，这在调试时很有用
        qWarning() << "Command failed:" << command << args.join(" ");
        qWarning() << "Stderr:" << QString::fromLocal8Bit(error);
        // 返回一个空的 QString 表示失败
        return {};
    }

    // 返回标准输出，并使用 trimmed() 删除开头和结尾的空白字符
    return QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed();
}

// 获取 CPU 信息
QString HardwareInfo::getCpuInfo()
{
    // 在 Linux 中，/proc/cpuinfo 文件包含了 CPU 的详细信息，且所有用户都可读。
    // 我们使用 "cat" 命令来读取这个文件。
    QString output = executeCommand("cat", QStringList() << "/proc/cpuinfo");
    // 使用正则表达式来从输出中匹配 "model name" 这一行。
    // R"(...)" 是 C++11 的原始字符串字面量，可以避免转义反斜杠。
    // \s+ 匹配一个或多个空白字符，(.*) 是一个捕获组，用于捕获该行冒号后面的所有内容。
    QRegularExpression re(R"(model name\s+:\s+(.*))");
    QRegularExpressionMatch match = re.match(output);
    // 如果匹配成功
    if (match.hasMatch()) {
        // captured(1) 返回第一个捕获组的内容，即 CPU 型号字符串。
        return match.captured(1).trimmed();
    }
    return "N/A"; // 如果没有匹配到，返回 "N/A"
}

// 获取主板序列号
QString HardwareInfo::getMotherboardSerial()
{
    // 在现代 Linux 系统中，DMI (Desktop Management Interface) 信息被暴露在 /sys 文件系统中。
    // /sys/class/dmi/id/product_uuid 文件通常包含了主板的唯一 UUID。
    // 这是在非 root 用户下获取主板信息的标准方法。
    QString output = executeCommand("cat", QStringList() << "/sys/class/dmi/id/product_uuid");
    if (output.isEmpty()) {
        // 如果文件不存在或为空，说明无法获取。
        // 另一种方法是使用 dmidecode 命令，但那通常需要 root 权限。
        return "N/A (requires root for dmidecode as fallback)";
    }
    return output;
}

// 获取硬盘序列号
QString HardwareInfo::getHardDiskSerial()
{
    // 1. 首先，使用 lsblk 命令列出所有的块设备（硬盘、SSD等）。
    //    -d: 只显示设备信息，不显示分区信息。
    //    -n: 无标题输出。
    //    -o NAME: 只输出设备名称。
    QStringList devices = executeCommand("lsblk", QStringList() << "-d" << "-n" << "-o" << "NAME").split('\n');

    // 2. 遍历所有找到的设备
    for (const QString &dev : devices) {
        // 我们只关心常见的硬盘设备名称
        if (dev.startsWith("sd") || dev.startsWith("hd") || dev.startsWith("nvme")) {
             // 对于每个设备，尝试从 /sys/block/设备名/device/serial 文件读取序列号。
             // 这也是非 root 用户可以尝试的方法。
             QString serial = executeCommand("cat", QStringList() << QString("/sys/block/%1/device/serial").arg(dev));
             if (!serial.isEmpty()) {
                 // 如果成功读取到序列号，就立即返回它。
                 return serial;
             }
        }
    }
    // 如果遍历完所有设备都找不到序列号，则返回 "N/A"。
    return "N/A (no serial found in /sys)";
}

// 获取 MAC 地址
QStringList HardwareInfo::getMacAddresses()
{
    QStringList macs;
    // `ip link` 是现代 Linux 系统中用于查看和管理网络接口的命令。
    QString output = executeCommand("ip", QStringList() << "link");
    // 使用正则表达式匹配 MAC 地址的格式。
    // MAC 地址通常显示在 "link/ether" 后面，格式为 6 组由冒号分隔的两位十六进制数。
    // ([0-9a-fA-F:]{17}) 是一个捕获组，用于捕获这个 17 个字符长的字符串。
    QRegularExpression re("link/ether ([0-9a-fA-F:]{17})");
    // 使用 globalMatch 进行全局匹配，以找到所有网络接口的 MAC 地址。
    auto it = re.globalMatch(output);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        // 将找到的 MAC 地址添加到列表中
        macs.append(match.captured(1));
    }
    return macs;
}
