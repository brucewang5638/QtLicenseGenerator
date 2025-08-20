#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cryptomanager.h"
#include "hardwareinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Tab: Local Info
    void on_fetchLocalInfoButton_clicked();

    // Tab: Key Management
    void on_generateKeysButton_clicked();
    void on_exportPrivateKeyButton_clicked();
    void on_exportPublicKeyButton_clicked();

    // Tab: License Issuing
    void on_loadPrivateKeyButton_clicked();
    void on_generateLicenseButton_clicked();

    // Tab: License Viewer
    void on_loadPublicKeyForVerificationButton_clicked();
    void on_verifyLicenseButton_clicked();

private:
    Ui::MainWindow *ui;
    CryptoManager *cryptoManager;
    bool privateKeyLoadedForSigning = false;
    bool publicKeyLoadedForVerification = false;

    void saveTextToFile(const QString &text, const QString &title, const QString &filter);
};
#endif // MAINWINDOW_H
