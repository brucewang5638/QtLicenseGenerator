#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <QObject>
#include <QString>
#include <QByteArray>

// 使用前向声明（Forward Declaration）来避免在头文件中包含完整的 OpenSSL 头文件。
// 这样可以加快编译速度，并减少头文件依赖。
// EVP_PKEY 是 OpenSSL 中用于表示非对称密钥（公钥或私钥）的通用结构体。
struct evp_pkey_st;
typedef struct evp_pkey_st EVP_PKEY;

/**
 * @class CryptoManager
 * @brief 管理所有加密操作的类。
 *
 * 这个类封装了 OpenSSL 库的功能，用于生成 RSA 密钥对、加载密钥、
 * 对数据进行数字签名以及验证签名。它是本许可证生成器应用的核心加密模块。
 * 它继承自 QObject 以便利用 Qt 的元对象系统（例如信号和槽），尽管在这个版本中未使用。
 */
class CryptoManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数。
     * @param parent 父 QObject 对象，用于 Qt 的对象所有权管理。
     */
    explicit CryptoManager(QObject *parent = nullptr);

    /**
     * @brief 析构函数。
     *        负责释放在对象生命周期内分配的 OpenSSL 密钥资源。
     */
    ~CryptoManager();

    /**
     * @brief 生成一个新的 RSA 密钥对（公钥和私钥）。
     *
     * 创建一个 2048 位的 RSA 密钥对，并将它们以 PEM 格式分别保存到指定的文件路径。
     * PEM 是一种常见的用于存储和传输加密密钥的格式。
     * @param privateKeyPath 用于保存私钥的完整文件路径。
     * @param publicKeyPath 用于保存公钥的完整文件路径。
     * @return 如果密钥对成功生成并保存，返回 true；否则返回 false。
     */
    bool generateKeys(const QString &privateKeyPath, const QString &publicKeyPath);

    /**
     * @brief 从文件加载私钥。
     *
     * 从一个 PEM 格式的文件中读取私钥，并将其加载到内存中以备后用（例如用于签名）。
     * @param privateKeyPath 包含私钥的 PEM 文件的路径。
     * @return 如果私钥成功加载，返回 true；否则返回 false。
     */
    bool loadPrivateKey(const QString &privateKeyPath);

    /**
     * @brief 从文件加载公钥。
     *
     * 从一个 PEM 格式的文件中读取公钥，并将其加载到内存中以备后用（例如用于验证签名）。
     * @param publicKeyPath 包含公钥的 PEM 文件的路径。
     * @return 如果公钥成功加载，返回 true；否则返回 false。
     */
    bool loadPublicKey(const QString &publicKeyPath);

    /**
     * @brief 使用已加载的私钥对数据进行数字签名。
     *
     * 此函数使用 SHA-256 算法计算输入数据的哈希值，然后用加载的私钥对该哈希值进行加密，
     * 从而生成数字签名。
     * @param data 需要被签名的数据。
     * @return 返回生成的数字签名（QByteArray格式）。如果私钥未加载或签名过程中发生错误，则返回一个空的 QByteArray。
     */
    QByteArray signData(const QByteArray &data);

    /**
     * @brief 使用已加载的公钥验证数字签名。
     *
     * 此函数使用公钥解密签名，得到原始的哈希值，同时计算输入数据的哈希值，
     * 然后比较这两个哈希值是否一致，以判断签名是否有效且数据未被篡改。
     * @param data 原始的、未经签名的数据。
     * @param signature 需要被验证的数字签名。
     * @return 如果签名有效，返回 true；否则返回 false。
     */
    bool verifySignature(const QByteArray &data, const QByteArray &signature);

private:
    /**
     * @brief 释放存储的密钥对以防止内存泄漏。
     *
     * 这是一个辅助函数，用于安全地释放由 OpenSSL 分配的 EVP_PKEY 对象。
     * 在加载新密钥或在析构函数中时调用。
     */
    void freeKeys();

    EVP_PKEY *privateKey; ///< 指向 OpenSSL 私钥对象的指针。
    EVP_PKEY *publicKey;  ///< 指向 OpenSSL 公钥对象的指针。
};

#endif // CRYPTOMANAGER_H
