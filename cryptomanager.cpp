#include "cryptomanager.h"
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <QFile>
#include <QDebug>

// CryptoManager 的构造函数
CryptoManager::CryptoManager(QObject *parent)
    : QObject(parent), privateKey(nullptr), publicKey(nullptr)
{
    // 这是一个好的实践：在使用 OpenSSL 的任何其他部分之前，加载它的错误字符串。
    // 这样，如果 OpenSSL 函数返回错误，我们可以获得有意义的错误信息。
    ERR_load_crypto_strings();
}

// CryptoManager 的析构函数
CryptoManager::~CryptoManager()
{
    // 调用 freeKeys() 来确保所有分配的密钥内存都被释放，防止内存泄漏。
    freeKeys();
    // 清理之前加载的错误字符串，释放资源。
    ERR_free_strings();
}

// 释放密钥内存的私有辅助函数
void CryptoManager::freeKeys()
{
    // 检查私钥指针是否有效
    if (privateKey) {
        // 释放 EVP_PKEY 对象
        EVP_PKEY_free(privateKey);
        // 将指针设为 nullptr，这是一个好习惯，可以防止“悬挂指针”问题。
        privateKey = nullptr;
    }
    // 对公钥也执行相同的操作
    if (publicKey) {
        EVP_PKEY_free(publicKey);
        publicKey = nullptr;
    }
}

// 生成 RSA 密钥对
bool CryptoManager::generateKeys(const QString &privateKeyPath, const QString &publicKeyPath)
{
    // 首先，释放任何已经存在的密钥。
    freeKeys();
    bool success = false;

    // 1. 创建密钥生成上下文（Context）
    // EVP_PKEY_CTX 是 OpenSSL 中用于执行非对称加密操作（如密钥生成、签名等）的上下文。
    // 我们指定算法为 RSA。
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) return false; // 如果创建失败，则直接返回

    // 2. 初始化密钥生成操作
    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx); // 出错时要记得释放上下文
        return false;
    }

    // 3. 设置密钥参数，这里是设置 RSA 密钥的位数。2048 位在当前被认为是安全的。
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    // 4. 执行密钥生成
    EVP_PKEY *pkey = NULL; // 用于接收生成的密钥对
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    // 密钥生成完毕，上下文不再需要，释放它。
    EVP_PKEY_CTX_free(ctx);

    // 5. 将生成的密钥对保存到文件
    // 使用 C 标准库的 fopen 来打开文件，因为 OpenSSL 的 PEM 写入函数需要 FILE* 类型的指针。
    // "wb" 表示以二进制写入模式打开文件。
    FILE *privFile = fopen(privateKeyPath.toStdString().c_str(), "wb");
    if (privFile) {
        // PEM_write_PrivateKey 将私钥以 PEM 格式写入文件。
        // 参数：文件指针，密钥对象，加密算法（NULL表示不加密），密码，密码长度，回调函数，回调参数。
        success = PEM_write_PrivateKey(privFile, pkey, NULL, NULL, 0, NULL, NULL);
        fclose(privFile); // 操作完成后关闭文件
    }

    // 如果保存私钥失败，则释放密钥对象并返回。
    if (!success) {
        EVP_PKEY_free(pkey);
        return false;
    }

    // 6. 保存公钥到文件
    FILE *pubFile = fopen(publicKeyPath.toStdString().c_str(), "wb");
    if (pubFile) {
        // PEM_write_PUBKEY 将公钥以 PEM 格式写入文件。
        success = PEM_write_PUBKEY(pubFile, pkey);
        fclose(pubFile);
    }

    // 密钥对已经写入文件，释放内存中的 pkey 对象。
    EVP_PKEY_free(pkey);
    return success;
}

// 从文件加载私钥
bool CryptoManager::loadPrivateKey(const QString &privateKeyPath)
{
    // 首先释放任何已加载的私钥
    if (privateKey) {
        EVP_PKEY_free(privateKey);
        privateKey = nullptr;
    }
    // 以二进制读取模式("rb")打开文件
    FILE *privFile = fopen(privateKeyPath.toStdString().c_str(), "rb");
    if (!privFile) return false;

    // PEM_read_PrivateKey 从文件中读取 PEM 格式的私钥
    privateKey = PEM_read_PrivateKey(privFile, NULL, NULL, NULL);
    fclose(privFile);

    // 如果 privateKey 不是 nullptr，说明加载成功。
    return privateKey != nullptr;
}

// 从文件加载公钥
bool CryptoManager::loadPublicKey(const QString &publicKeyPath)
{
    // 首先释放任何已加载的公钥
    if (publicKey) {
        EVP_PKEY_free(publicKey);
        publicKey = nullptr;
    }
    FILE *pubFile = fopen(publicKeyPath.toStdString().c_str(), "rb");
    if (!pubFile) return false;

    // PEM_read_PUBKEY 从文件中读取 PEM 格式的公钥
    publicKey = PEM_read_PUBKEY(pubFile, NULL, NULL, NULL);
    fclose(pubFile);

    return publicKey != nullptr;
}

// 对数据进行签名
QByteArray CryptoManager::signData(const QByteArray &data)
{
    // 必须先加载私钥才能签名
    if (!privateKey) return QByteArray();

    // 1. 创建消息摘要上下文（Message Digest Context）
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) return QByteArray();

    // 2. 指定哈希算法，这里使用 SHA-256
    const EVP_MD *md = EVP_sha256();
    QByteArray signature;

    // 3. 初始化签名操作
    // 将哈希算法和私钥关联到上下文中
    if (EVP_DigestSignInit(mdctx, NULL, md, NULL, privateKey) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return QByteArray();
    }

    // 4. 提供要签名的数据
    // 可以多次调用此函数来处理流式数据
    if (EVP_DigestSignUpdate(mdctx, data.constData(), data.size()) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return QByteArray();
    }

    // 5. 获取签名所需的缓冲区大小
    size_t sigLen;
    if (EVP_DigestSignFinal(mdctx, NULL, &sigLen) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return QByteArray();
    }

    // 6. 根据获取的大小调整 QByteArray 的大小
    signature.resize(sigLen);

    // 7. 最终生成签名
    if (EVP_DigestSignFinal(mdctx, (unsigned char*)signature.data(), &sigLen) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return QByteArray();
    }

    // 8. 再次调整大小以匹配实际的签名长度（可能比预估的小）
    signature.resize(sigLen);
    EVP_MD_CTX_free(mdctx); // 释放上下文
    return signature;
}

// 验证签名
bool CryptoManager::verifySignature(const QByteArray &data, const QByteArray &signature)
{
    // 必须先加载公钥才能验证
    if (!publicKey) return false;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) return false;

    // 1. 指定哈希算法，必须与签名时使用的算法一致
    const EVP_MD *md = EVP_sha256();

    // 2. 初始化验证操作
    // 将哈希算法和公钥关联到上下文中
    if (EVP_DigestVerifyInit(mdctx, NULL, md, NULL, publicKey) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    // 3. 提供原始数据
    if (EVP_DigestVerifyUpdate(mdctx, data.constData(), data.size()) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    // 4. 执行验证
    // 将原始数据计算出的哈希值与用公钥从签名中解密出的哈希值进行比较
    int result = EVP_DigestVerifyFinal(mdctx, (const unsigned char*)signature.constData(), signature.size());
    EVP_MD_CTX_free(mdctx);

    // EVP_DigestVerifyFinal 成功时返回 1，失败返回 0，出错返回负值。
    return result == 1;
}
