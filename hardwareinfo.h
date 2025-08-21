#ifndef HARDWAREINFO_H
#define HARDWAREINFO_H

#include <QString>
#include <QStringList>

/**
 * @class HardwareInfo
 * @brief 提供用于获取本地硬件信息的静态方法的工具类。
 *
 * 这个类完全由静态方法组成，因此不需要创建它的实例。
 * 它封装了在 Linux 系统上通过执行命令行工具来获取硬件信息的逻辑，
 * 例如 CPU 型号、主板序列号、硬盘序列号和 MAC 地址。
 * 这些信息通常用于生成与特定机器绑定的许可证。
 *
 * @note 此类中的许多函数在普通用户权限下获取的信息可能不唯一或不完整。
 *       为了获得最准确和唯一的硬件序列号，通常需要 root 权限。
 *       但为了应用的通用性，这里的实现尝试使用普通用户即可访问的 /sys 和 /proc 文件系统。
 */
class HardwareInfo
{
public:
    /**
     * @brief 获取 CPU 的型号信息。
     * @return 返回一个包含 CPU 型号的 QString。如果无法获取，则返回 "N/A"。
     */
    static QString getCpuInfo();

    /**
     * @brief 获取主板的产品 UUID (通用唯一识别码)。
     *
     * 这通常被用作主板的唯一序列号。
     * @return 返回主板的 UUID 字符串。如果无法获取，则返回 "N/A"。
     */
    static QString getMotherboardSerial();

    /**
     * @brief 获取主硬盘的序列号。
     *
     * 尝试查找第一个可用的块设备（如 sda, hda, nvme0n1）并读取其序列号。
     * @return 返回找到的第一个硬盘的序列号。如果找不到，则返回 "N/A"。
     */
    static QString getHardDiskSerial();

    /**
     * @brief 获取本机所有网络接口的 MAC 地址列表。
     * @return 返回一个包含所有找到的 MAC 地址的 QStringList。
     */
    static QStringList getMacAddresses();

private:
    /**
     * @brief 执行一个外部 shell 命令并返回其标准输出。
     *
     * 这是一个私有的辅助函数，被此类中的其他公共方法用来与操作系统交互。
     * @param command 要执行的命令 (例如 "cat", "ip")。
     * @param args 传递给命令的参数列表。
     * @return 返回命令的标准输出内容，并移除前后的空白字符。如果命令执行失败，则返回空字符串。
     */
    static QString executeCommand(const QString &command, const QStringList &args);
};

#endif // HARDWAREINFO_H
