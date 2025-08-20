#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUuid>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cryptoManager(new CryptoManager(this))
{
    ui->setupUi(this);
    ui->expiryDateEdit->setDateTime(QDateTime::currentDateTime().addYears(1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//================================================================================
// Helper Methods
//================================================================================

void MainWindow::saveTextToFile(const QString &text, const QString &title, const QString &filter)
{
    if (text.isEmpty()) {
        QMessageBox::warning(this, title, "内容为空，无法保存。");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, title, "", filter);
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << text;
        file.close();
        QMessageBox::information(this, title, "文件已成功保存到：\n" + fileName);
    } else {
        QMessageBox::critical(this, title, "无法写入文件：\n" + fileName);
    }
}

//================================================================================
// Tab: Local Info
//================================================================================

void MainWindow::on_fetchLocalInfoButton_clicked()
{
    ui->localInfoTextEdit->clear();

    QString infoText;
    QTextStream stream(&infoText);

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

    ui->localInfoTextEdit->setText(infoText);
}

//================================================================================
// Tab: Key Management
//================================================================================

void MainWindow::on_generateKeysButton_clicked()
{
    QString privateKeyPath = QFileDialog::getSaveFileName(this, "保存私钥文件", "private_key.pem", "PEM Files (*.pem);;All Files (*)");
    if (privateKeyPath.isEmpty()) return;

    QString publicKeyPath = QFileDialog::getSaveFileName(this, "保存公钥文件", "public_key.pem", "PEM Files (*.pem);;All Files (*)");
    if (publicKeyPath.isEmpty()) return;

    bool success = cryptoManager->generateKeys(privateKeyPath, publicKeyPath);

    if (success) {
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

void MainWindow::on_exportPrivateKeyButton_clicked()
{
    saveTextToFile(ui->privateKeyTextEdit->toPlainText(), "导出私钥", "private_key.pem");
}

void MainWindow::on_exportPublicKeyButton_clicked()
{
    saveTextToFile(ui->publicKeyTextEdit->toPlainText(), "导出公钥", "public_key.pem");
}

//================================================================================
// Tab: License Issuing
//================================================================================

void MainWindow::on_loadPrivateKeyButton_clicked()
{
    QString privateKeyPath = QFileDialog::getOpenFileName(this, "加载私钥文件", "", "PEM Files (*.pem);;All Files (*)");
    if (privateKeyPath.isEmpty()) return;

    privateKeyLoadedForSigning = cryptoManager->loadPrivateKey(privateKeyPath);

    if (privateKeyLoadedForSigning) {
        ui->privateKeyStatusLabel->setText("状态：<b style='color:green;'>已加载</b> - " + QFileInfo(privateKeyPath).fileName());
        QMessageBox::information(this, "成功", "私钥已成功加载，可以用于签名。");
    } else {
        ui->privateKeyStatusLabel->setText("状态：<b style='color:red;'>加载失败</b>");
        QMessageBox::critical(this, "失败", "加载私钥时发生错误，请检查文件格式是否正确。");
    }
}

void MainWindow::on_generateLicenseButton_clicked()
{
    // 1. Validate input
    if (!privateKeyLoadedForSigning) {
        QMessageBox::warning(this, "验证失败", "请先加载用于签名的私钥。");
        return;
    }
    if (ui->customerNameLineEdit->text().isEmpty() || ui->hardwareIdsTextEdit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "验证失败", "客户名称和硬件绑定ID不能为空。");
        return;
    }

    // 2. Construct the JSON license data
    QJsonObject metadata;
    metadata["kid"] = "key-2025-01"; // As per spec
    metadata["notBefore"] = QDateTime::currentSecsSinceEpoch();
    metadata["notAfter"] = ui->expiryDateEdit->dateTime().toSecsSinceEpoch();
    metadata["serialNumber"] = QUuid::createUuid().toString(QUuid::WithoutBraces);

    QJsonArray macsArray = QJsonArray::fromStringList(ui->hardwareIdsTextEdit->toPlainText().split('\n', Qt::SkipEmptyParts));

    QJsonObject payload;
    payload["customerName"] = ui->customerNameLineEdit->text();
    payload["licenseType"] = ui->licenseTypeLineEdit->text();
    payload["macs"] = macsArray; // Using MACs as the hardware ID

    QJsonObject licenseData;
    licenseData["metadata"] = metadata;
    licenseData["payload"] = payload;

    QJsonDocument licenseDoc(licenseData);
    QByteArray licenseJson = licenseDoc.toJson(QJsonDocument::Compact);

    // 3. Sign the data
    QByteArray signature = cryptoManager->signData(licenseJson);
    if (signature.isEmpty()) {
        QMessageBox::critical(this, "签名失败", "使用私钥签名数据时发生错误。");
        return;
    }

    // 4. Construct the final license file content
    QJsonObject finalLicenseObject;
    finalLicenseObject["licenseData"] = QString::fromUtf8(licenseJson);
    finalLicenseObject["signature"] = QString::fromLatin1(signature.toBase64());

    QJsonDocument finalDoc(finalLicenseObject);

    // 5. Save the final license file
    QString savePath = QFileDialog::getSaveFileName(this, "保存许可证文件", ui->customerNameLineEdit->text() + ".lic", "License Files (*.lic);;All Files (*)");
    if (savePath.isEmpty()) return;

    QFile outFile(savePath);
    if (outFile.open(QIODevice::WriteOnly)) {
        outFile.write(finalDoc.toJson(QJsonDocument::Indented));
        outFile.close();
        QMessageBox::information(this, "成功", "许可证文件已成功生成并保存！");
    } else {
        QMessageBox::critical(this, "保存失败", "无法写入许可证文件到指定路径。");
    }
}

//================================================================================
// Tab: License Viewer
//================================================================================

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

void MainWindow::on_verifyLicenseButton_clicked()
{
    // 1. Validate public key is loaded
    if (!publicKeyLoadedForVerification) {
        QMessageBox::warning(this, "验证失败", "请先加载用于验证的公钥。");
        return;
    }

    // 2. Load license file
    QString licensePath = QFileDialog::getOpenFileName(this, "加载许可证文件", "", "License Files (*.lic);;All Files (*)");
    if (licensePath.isEmpty()) return;

    QFile licFile(licensePath);
    if (!licFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "错误", "无法读取许可证文件。");
        return;
    }

    // 3. Parse the license file
    QJsonDocument licDoc = QJsonDocument::fromJson(licFile.readAll());
    licFile.close();
    if (licDoc.isNull() || !licDoc.isObject()) {
        QMessageBox::critical(this, "错误", "许可证文件格式无效，不是一个有效的JSON对象。");
        return;
    }

    QJsonObject rootObj = licDoc.object();
    QByteArray signature = QByteArray::fromBase64(rootObj["signature"].toString().toLatin1());
    QByteArray licenseData = rootObj["licenseData"].toString().toUtf8();

    // 4. Verify
    bool isValid = cryptoManager->verifySignature(licenseData, signature);

    // 5. Display results
    if (isValid) {
        ui->verificationResultLabel->setText("<b style='color:green; font-size:14pt;'>验证成功</b>");
    } else {
        ui->verificationResultLabel->setText("<b style='color:red; font-size:14pt;'>验证失败! 签名不匹配!</b>");
    }

    QJsonDocument prettyDoc = QJsonDocument::fromJson(licenseData);
    ui->licenseDetailsTextEdit->setText(prettyDoc.toJson(QJsonDocument::Indented));
}
