#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <QObject>
#include <QString>
#include <QByteArray>

// Forward declarations for OpenSSL types
typedef struct rsa_st RSA;
typedef struct evp_pkey_st EVP_PKEY;

class CryptoManager : public QObject
{
    Q_OBJECT
public:
    explicit CryptoManager(QObject *parent = nullptr);
    ~CryptoManager();

    // Generates a new RSA-2048 key pair
    bool generateKeys(const QString &privateKeyPath, const QString &publicKeyPath);

    // Loads a private key from a PEM file
    bool loadPrivateKey(const QString &privateKeyPath);

    // Loads a public key from a PEM file
    bool loadPublicKey(const QString &publicKeyPath);

    // Signs data with the loaded private key
    QByteArray signData(const QByteArray &data);

    // Verifies a signature with the loaded public key
    bool verifySignature(const QByteArray &data, const QByteArray &signature);

private:
    EVP_PKEY *privateKey;
    EVP_PKEY *publicKey;

    void freeKeys();
};

#endif // CRYPTOMANAGER_H
