#ifndef HARDWAREINFO_H
#define HARDWAREINFO_H

#include <QString>
#include <QStringList>

class HardwareInfo
{
public:
    // Gets the CPU serial number. Requires root privileges for a true serial.
    static QString getCpuInfo();

    // Gets the motherboard serial number (UUID). Requires root privileges for a true serial.
    static QString getMotherboardSerial();

    // Gets the primary hard disk serial number. Requires root privileges for a true serial.
    static QString getHardDiskSerial();

    // Gets a list of all MAC addresses.
    static QStringList getMacAddresses();

private:
    // Helper function to execute a command and get its output.
    static QString executeCommand(const QString &command, const QStringList &args);
};

#endif // HARDWAREINFO_H
