#include "mainwindow.h"
#include "ui_mainwindow.h" // 包含由 Qt Designer 生成的 UI 定义
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUuid>
#include <QDateTime>

// 构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // 创建 UI 类的实例
    , cryptoManager(new CryptoManager(this)) // 创建加密管理器实例
{
    // ui->setupUi(this) 是一个关键函数，它使用在 Designer 中设计的 UI 布局来初始化这个窗口。
    ui->setupUi(this);
    // 为许可证过期日期选择器设置一个默认值，默认为从今天起一年后。
    ui->expiryDateEdit->setDateTime(QDateTime::currentDateTime().addYears(1));
}

// 析构函数
MainWindow::~MainWindow()
{
    // 清理 UI 对象，这是 C++ 中手动管理内存的必要步骤。
    // cryptoManager 不需要在这里 delete，因为它被指定了 parent (this)，Qt 的父子对象机制会自动处理它的销毁。
    delete ui;
}

//================================================================================
// 辅助方法 (Helper Methods)
//================================================================================

// 将文本保存到文件的通用函数
void MainWindow::saveTextToFile(const QString &text, const QString &title, const QString &filter)
{
    // 检查是否有内容需要保存
    if (text.isEmpty()) {
        QMessageBox::warning(this, title, "内容为空，无法保存。");
        return;
    }
    // 弹出标准的“文件另存为”对话框
    // QFileDialog::getSaveFileName 会让用户选择保存位置和文件名。
    QString fileName = QFileDialog::getSaveFileName(this, title, "", filter);
    // 如果用户取消了对话框，fileName 会是空的。
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    // 以只写和文本模式打开文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file); // 使用 QTextStream 来方便地写入文本
        out << text;
        file.close(); // 完成后关闭文件
        QMessageBox::information(this, title, "文件已成功保存到：\n" + fileName);
    } else {
        // 如果文件无法打开（例如因为权限问题），则显示错误消息。
        QMessageBox::critical(this, title, "无法写入文件：\n" + fileName);
    }
}

//================================================================================
// “本地信息” 标签页 (Tab: Local Info)
//================================================================================

// “获取本地信息”按钮的点击事件处理函数
void MainWindow::on_fetchLocalInfoButton_clicked()
{
    ui->localInfoTextEdit->clear(); // 清空之前的显示

    QString infoText;
    QTextStream stream(&infoText); // 使用 QTextStream 来构建要显示的字符串

    // 调用 HardwareInfo 类的静态方法来获取各种硬件信息
    stream << "CPU 信息:\n" << HardwareInfo::getCpuInfo() << "\n\n";
    stream << "主板序列号 (UUID):\n" << HardwareInfo::getMotherboardSerial() << "\n\n";
    stream << "硬盘序列号:\n" << HardwareInfo::getHardDiskSerial() << "\n\n";

    QStringList macs = HardwareInfo::getMacAddresses();
    stream << "MAC 地址:\n";
    if (macs.isEmpty()) {
        stream << "  N/A\n";
    } else {
        for (const QString &mac : macs) {
            stream << "  " << mac << "\n";
        }
    }

    // 将构建好的信息字符串设置到文本框中显示
    ui->localInfoTextEdit->setText(infoText);
}

//================================================================================
// “密钥管理” 标签页 (Tab: Key Management)
//================================================================================

// “生成密钥对”按钮的点击事件处理函数
void MainWindow::on_generateKeysButton_clicked()
{
    // 弹出文件对话框，让用户选择私钥的保存位置
    QString privateKeyPath = QFileDialog::getSaveFileName(this, "保存私钥文件", "private_key.pem", "PEM Files (*.pem);;All Files (*)");
    if (privateKeyPath.isEmpty()) return; // 用户取消

    // 弹出文件对话框，让用户选择公钥的保存位置
    QString publicKeyPath = QFileDialog::getSaveFileName(this, "保存公钥文件", "public_key.pem", "PEM Files (*.pem);;All Files (*)");
    if (publicKeyPath.isEmpty()) return; // 用户取消

    // 调用加密管理器生成密钥对
    bool success = cryptoManager->generateKeys(privateKeyPath, publicKeyPath);

    if (success) {
        // 如果成功，读取刚生成的文件内容并显示在界面上
        QFile privateFile(privateKeyPath);
        if (privateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ui->privateKeyTextEdit->setText(privateFile.readAll());
            privateFile.close();
        }

        QFile publicFile(publicKeyPath);
        if (publicFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ui->publicKeyTextEdit->setText(publicFile.readAll());
            publicFile.close();
        }

        QMessageBox::information(this, "成功", "密钥对已成功生成并保存。");
    } else {
        QMessageBox::critical(this, "失败", "生成密钥对时发生错误。");
    }
}

// “导出私钥”按钮的点击事件处理函数
void MainWindow::on_exportPrivateKeyButton_clicked()
{
    // 调用通用的保存函数来保存文本框中的私钥内容
    saveTextToFile(ui->privateKeyTextEdit->toPlainText(), "导出私钥", "private_key.pem");
}

// “导出公钥”按钮的点击事件处理函数
void MainWindow::on_exportPublicKeyButton_clicked()
{
    saveTextToFile(ui->publicKeyTextEdit->toPlainText(), "导出公钥", "public_key.pem");
}

//================================================================================
// “签发许可” 标签页 (Tab: License Issuing)
//================================================================================

// “加载私钥”按钮的点击事件处理函数
void MainWindow::on_loadPrivateKeyButton_clicked()
{
    // 弹出文件对话框让用户选择要加载的私钥文件
    QString privateKeyPath = QFileDialog::getOpenFileName(this, "加载私钥文件", "", "PEM Files (*.pem);;All Files (*)");
    if (privateKeyPath.isEmpty()) return;

    // 尝试加载私钥
    privateKeyLoadedForSigning = cryptoManager->loadPrivateKey(privateKeyPath);

    if (privateKeyLoadedForSigning) {
        // 如果成功，更新界面上的状态标签，并设置一个状态标志
        ui->privateKeyStatusLabel->setText("状态：<b style='color:green;'>已加载</b> - " + QFileInfo(privateKeyPath).fileName());
        QMessageBox::information(this, "成功", "私钥已成功加载，可以用于签名。");
    } else {
        ui->privateKeyStatusLabel->setText("状态：<b style='color:red;'>加载失败</b>");
        QMessageBox::critical(this, "失败", "加载私钥时发生错误，请检查文件格式是否正确。");
    }
}

// “生成许可证”按钮的点击事件处理函数
void MainWindow::on_generateLicenseButton_clicked()
{
    // 1. 输入验证
    if (!privateKeyLoadedForSigning) {
        QMessageBox::warning(this, "验证失败", "请先加载用于签名的私钥。");
        return;
    }
    if (ui->customerNameLineEdit->text().isEmpty() || ui->hardwareIdsTextEdit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "验证失败", "客户名称和硬件绑定ID不能为空。");
        return;
    }

    // 2. 构建要签名的许可证数据（JSON 格式）
    //    许可证信息被分为 metadata 和 payload 两部分。
    QJsonObject metadata;
    metadata["kid"] = "key-2025-01"; // Key ID，用于标识使用了哪个密钥
    metadata["notBefore"] = QDateTime::currentSecsSinceEpoch(); // 生效时间（当前时间）
    metadata["notAfter"] = ui->expiryDateEdit->dateTime().toSecsSinceEpoch(); // 过期时间
    metadata["serialNumber"] = QUuid::createUuid().toString(QUuid::WithoutBraces); // 生成一个唯一的许可证序列号

    // 将硬件ID（这里是MAC地址）从文本框读入并转为 JSON 数组
    QJsonArray macsArray = QJsonArray::fromStringList(ui->hardwareIdsTextEdit->toPlainText().split('\n', Qt::SkipEmptyParts));

    QJsonObject payload;
    payload["customerName"] = ui->customerNameLineEdit->text();
    payload["licenseType"] = ui->licenseTypeLineEdit->text();
    payload["macs"] = macsArray;

    QJsonObject licenseData;
    licenseData["metadata"] = metadata;
    licenseData["payload"] = payload;

    // 将 JSON 对象转换为紧凑的（无多余空格）字符串
    QJsonDocument licenseDoc(licenseData);
    QByteArray licenseJson = licenseDoc.toJson(QJsonDocument::Compact);

    // 3. 对 JSON 数据进行签名
    QByteArray signature = cryptoManager->signData(licenseJson);
    if (signature.isEmpty()) {
        QMessageBox::critical(this, "签名失败", "使用私钥签名数据时发生错误。");
        return;
    }

    // 4. 构建最终的许可证文件内容
    //    最终文件包含两部分：原始的许可证数据（字符串）和签名（Base64编码的字符串）。
    QJsonObject finalLicenseObject;
    finalLicenseObject["licenseData"] = QString::fromUtf8(licenseJson);
    finalLicenseObject["signature"] = QString::fromLatin1(signature.toBase64());

    QJsonDocument finalDoc(finalLicenseObject);

    // 5. 保存最终的许可证文件
    QString savePath = QFileDialog::getSaveFileName(this, "保存许可证文件", ui->customerNameLineEdit->text() + ".lic", "License Files (*.lic);;All Files (*)");
    if (savePath.isEmpty()) return;

    QFile outFile(savePath);
    if (outFile.open(QIODevice::WriteOnly)) {
        // 以可读性好的缩进格式写入文件
        outFile.write(finalDoc.toJson(QJsonDocument::Indented));
        outFile.close();
        QMessageBox::information(this, "成功", "许可证文件已成功生成并保存！");
    } else {
        QMessageBox::critical(this, "保存失败", "无法写入许可证文件到指定路径。");
    }
}

//================================================================================
// “许可查看” 标签页 (Tab: License Viewer)
//================================================================================

// “加载公钥”按钮的点击事件处理函数
void MainWindow::on_loadPublicKeyForVerificationButton_clicked()
{
    QString publicKeyPath = QFileDialog::getOpenFileName(this, "加载公钥文件", "", "PEM Files (*.pem);;All Files (*)");
    if (publicKeyPath.isEmpty()) return;

    publicKeyLoadedForVerification = cryptoManager->loadPublicKey(publicKeyPath);

    if (publicKeyLoadedForVerification) {
        ui->publicKeyStatusLabel->setText("状态：<b style='color:green;'>已加载</b> - " + QFileInfo(publicKeyPath).fileName());
    } else {
        ui->publicKeyStatusLabel->setText("状态：<b style='color:red;'>加载失败</b>");
    }
}

// “验证许可证”按钮的点击事件处理函数
void MainWindow::on_verifyLicenseButton_clicked()
{
    // 1. 检查公钥是否已加载
    if (!publicKeyLoadedForVerification) {
        QMessageBox::warning(this, "验证失败", "请先加载用于验证的公钥。");
        return;
    }

    // 2. 加载许可证文件
    QString licensePath = QFileDialog::getOpenFileName(this, "加载许可证文件", "", "License Files (*.lic);;All Files (*)");
    if (licensePath.isEmpty()) return;

    QFile licFile(licensePath);
    if (!licFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "错误", "无法读取许可证文件。");
        return;
    }

    // 3. 解析许可证文件
    QJsonDocument licDoc = QJsonDocument::fromJson(licFile.readAll());
    licFile.close();
    if (licDoc.isNull() || !licDoc.isObject()) {
        QMessageBox::critical(this, "错误", "许可证文件格式无效，不是一个有效的JSON对象。");
        return;
    }

    // 从 JSON 中提取许可证数据和签名
    QJsonObject rootObj = licDoc.object();
    QByteArray signature = QByteArray::fromBase64(rootObj["signature"].toString().toLatin1());
    QByteArray licenseData = rootObj["licenseData"].toString().toUtf8();

    // 4. 调用加密管理器验证签名
    bool isValid = cryptoManager->verifySignature(licenseData, signature);

    // 5. 显示验证结果
    if (isValid) {
        ui->verificationResultLabel->setText("<b style='color:green; font-size:14pt;'>验证成功</b>");
    } else {
        ui->verificationResultLabel->setText("<b style='color:red; font-size:14pt;'>验证失败! 签名不匹配!</b>");
    }

    // 无论验证是否成功，都将许可证的详细内容（格式化后）显示出来，方便查看。
    QJsonDocument prettyDoc = QJsonDocument::fromJson(licenseData);
    ui->licenseDetailsTextEdit->setText(prettyDoc.toJson(QJsonDocument::Indented));
}
