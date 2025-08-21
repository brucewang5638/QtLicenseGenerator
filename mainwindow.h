#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cryptomanager.h"
#include "hardwareinfo.h"

// QT_BEGIN_NAMESPACE 和 QT_END_NAMESPACE 是 Qt 的宏，
// 用于在 Qt 自己生成的代码中（比如这里的 Ui::MainWindow）避免命名冲突。
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief 应用程序的主窗口类。
 *
 * 这个类负责管理图形用户界面（GUI）的所有元素和用户交互。
 * 它包含了四个主要功能标签页的逻辑：
 * 1. 获取本地硬件信息
 * 2. 生成和管理密钥对
 * 3. 签发许可证
 * 4. 查看和验证许可证
 * 它通过组合 CryptoManager 和 HardwareInfo 类的功能来完成其任务。
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT // 这个宏必须出现在任何定义了信号或槽的类中，是 Qt 元对象系统的核心。

public:
    /**
     * @brief 构造函数。
     * @param parent 父 QWidget 对象，用于 Qt 的窗口所有权管理。
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief 析构函数。
     *        负责清理 UI 和其他在构造函数中分配的资源。
     */
    ~MainWindow();

// private slots: 是 Qt 的一个特性，这里的函数是“槽”，用于响应“信号”（例如按钮点击）。
// 命名约定 on_<widgetName>_<signalName> 是 Qt Designer 自动连接功能使用的标准格式。
private slots:
    // --- “本地信息” 标签页的槽函数 ---
    /**
     * @brief 当“获取本地信息”按钮被点击时调用。
     */
    void on_fetchLocalInfoButton_clicked();

    // --- “密钥管理” 标签页的槽函数 ---
    /**
     * @brief 当“生成密钥对”按钮被点击时调用。
     */
    void on_generateKeysButton_clicked();
    /**
     * @brief 当“导出私钥”按钮被点击时调用。
     */
    void on_exportPrivateKeyButton_clicked();
    /**
     * @brief 当“导出公钥”按钮被点击时调用。
     */
    void on_exportPublicKeyButton_clicked();

    // --- “签发许可” 标签页的槽函数 ---
    /**
     * @brief 当“加载私钥”按钮被点击时调用。
     */
    void on_loadPrivateKeyButton_clicked();
    /**
     * @brief 当“生成许可证”按钮被点击时调用。
     */
    void on_generateLicenseButton_clicked();

    // --- “许可查看” 标签页的槽函数 ---
    /**
     * @brief 当“加载公钥（用于验证）”按钮被点击时调用。
     */
    void on_loadPublicKeyForVerificationButton_clicked();
    /**
     * @brief 当“验证许可证”按钮被点击时调用。
     */
    void on_verifyLicenseButton_clicked();

private:
    Ui::MainWindow *ui; ///< 指向由 Qt Designer 自动生成的 UI 类的指针，包含了所有界面控件。
    CryptoManager *cryptoManager; ///< 指向加密管理器的实例，用于执行所有加密操作。

    // --- 状态标志 ---
    ///< 标记是否已成功加载用于签发许可证的私钥。
    bool privateKeyLoadedForSigning = false;
    ///< 标记是否已成功加载用于验证许可证的公钥。
    bool publicKeyLoadedForVerification = false;

    /**
     * @brief 一个通用的辅助函数，用于将文本内容保存到文件中。
     * @param text 要保存的文本。
     * @param title 文件保存对话框的标题。
     * @param filter 文件类型过滤器 (例如 "Text files (*.txt)")。
     */
    void saveTextToFile(const QString &text, const QString &title, const QString &filter);
};
#endif // MAINWINDOW_H
