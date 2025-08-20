#include "cryptomanager.h"
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <QFile>
#include <QDebug>

CryptoManager::CryptoManager(QObject *parent)
    : QObject(parent), privateKey(nullptr), publicKey(nullptr)
{
    // It's good practice to initialize the OpenSSL error strings
    ERR_load_crypto_strings();
}

CryptoManager::~CryptoManager()
{
    freeKeys();
    // Clean up OpenSSL error strings
    ERR_free_strings();
}

void CryptoManager::freeKeys()
{
    if (privateKey) {
        EVP_PKEY_free(privateKey);
        privateKey = nullptr;
    }
    if (publicKey) {
        EVP_PKEY_free(publicKey);
        publicKey = nullptr;
    }
}

bool CryptoManager::generateKeys(const QString &privateKeyPath, const QString &publicKeyPath)
{
    freeKeys();
    bool success = false;
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) return false;

    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    EVP_PKEY *pkey = NULL;
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    EVP_PKEY_CTX_free(ctx);

    // Save private key
    FILE *privFile = fopen(privateKeyPath.toStdString().c_str(), "wb");
    if (privFile) {
        success = PEM_write_PrivateKey(privFile, pkey, NULL, NULL, 0, NULL, NULL);
        fclose(privFile);
    }

    if (!success) {
        EVP_PKEY_free(pkey);
        return false;
    }

    // Save public key
    FILE *pubFile = fopen(publicKeyPath.toStdString().c_str(), "wb");
    if (pubFile) {
        success = PEM_write_PUBKEY(pubFile, pkey);
        fclose(pubFile);
    }

    EVP_PKEY_free(pkey);
    return success;
}

bool CryptoManager::loadPrivateKey(const QString &privateKeyPath)
{
    if (privateKey) {
        EVP_PKEY_free(privateKey);
        privateKey = nullptr;
    }
    FILE *privFile = fopen(privateKeyPath.toStdString().c_str(), "rb");
    if (!privFile) return false;

    privateKey = PEM_read_PrivateKey(privFile, NULL, NULL, NULL);
    fclose(privFile);

    return privateKey != nullptr;
}

bool CryptoManager::loadPublicKey(const QString &publicKeyPath)
{
    if (publicKey) {
        EVP_PKEY_free(publicKey);
        publicKey = nullptr;
    }
    FILE *pubFile = fopen(publicKeyPath.toStdString().c_str(), "rb");
    if (!pubFile) return false;

    publicKey = PEM_read_PUBKEY(pubFile, NULL, NULL, NULL);
    fclose(pubFile);

    return publicKey != nullptr;
}

QByteArray CryptoManager::signData(const QByteArray &data)
{
    if (!privateKey) return QByteArray();

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) return QByteArray();

    const EVP_MD *md = EVP_sha256();
    QByteArray signature;

    if (EVP_DigestSignInit(mdctx, NULL, md, NULL, privateKey) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return QByteArray();
    }

    if (EVP_DigestSignUpdate(mdctx, data.constData(), data.size()) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return QByteArray();
    }

    size_t sigLen;
    if (EVP_DigestSignFinal(mdctx, NULL, &sigLen) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return QByteArray();
    }

    signature.resize(sigLen);

    if (EVP_DigestSignFinal(mdctx, (unsigned char*)signature.data(), &sigLen) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return QByteArray();
    }

    signature.resize(sigLen);
    EVP_MD_CTX_free(mdctx);
    return signature;
}

bool CryptoManager::verifySignature(const QByteArray &data, const QByteArray &signature)
{
    if (!publicKey) return false;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) return false;

    const EVP_MD *md = EVP_sha256();
    if (EVP_DigestVerifyInit(mdctx, NULL, md, NULL, publicKey) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    if (EVP_DigestVerifyUpdate(mdctx, data.constData(), data.size()) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    int result = EVP_DigestVerifyFinal(mdctx, (const unsigned char*)signature.constData(), signature.size());
    EVP_MD_CTX_free(mdctx);

    return result == 1;
}
