/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *keyManagementTab;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *generateKeysButton;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QTextEdit *privateKeyTextEdit;
    QPushButton *exportPrivateKeyButton;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    QTextEdit *publicKeyTextEdit;
    QPushButton *exportPublicKeyButton;
    QWidget *issueLicenseTab;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *loadPrivateKeyButton;
    QLabel *privateKeyStatusLabel;
    QGroupBox *groupBox_5;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *customerNameLineEdit;
    QLabel *label_2;
    QLineEdit *licenseTypeLineEdit;
    QLabel *label_3;
    QTextEdit *hardwareIdsTextEdit;
    QLabel *label_4;
    QDateEdit *expiryDateEdit;
    QPushButton *generateLicenseButton;
    QWidget *viewLicenseTab;
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBox_6;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *loadPublicKeyForVerificationButton;
    QLabel *publicKeyStatusLabel;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_8;
    QPushButton *verifyLicenseButton;
    QLabel *verificationResultLabel;
    QTextEdit *licenseDetailsTextEdit;
    QWidget *localInfoTab;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *localInfoTextEdit;
    QPushButton *fetchLocalInfoButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        keyManagementTab = new QWidget();
        keyManagementTab->setObjectName(QString::fromUtf8("keyManagementTab"));
        verticalLayout_3 = new QVBoxLayout(keyManagementTab);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox = new QGroupBox(keyManagementTab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        generateKeysButton = new QPushButton(groupBox);
        generateKeysButton->setObjectName(QString::fromUtf8("generateKeysButton"));

        horizontalLayout->addWidget(generateKeysButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(keyManagementTab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        privateKeyTextEdit = new QTextEdit(groupBox_2);
        privateKeyTextEdit->setObjectName(QString::fromUtf8("privateKeyTextEdit"));
        privateKeyTextEdit->setReadOnly(true);

        verticalLayout_4->addWidget(privateKeyTextEdit);

        exportPrivateKeyButton = new QPushButton(groupBox_2);
        exportPrivateKeyButton->setObjectName(QString::fromUtf8("exportPrivateKeyButton"));

        verticalLayout_4->addWidget(exportPrivateKeyButton);


        verticalLayout_3->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(keyManagementTab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        publicKeyTextEdit = new QTextEdit(groupBox_3);
        publicKeyTextEdit->setObjectName(QString::fromUtf8("publicKeyTextEdit"));
        publicKeyTextEdit->setReadOnly(true);

        verticalLayout_5->addWidget(publicKeyTextEdit);

        exportPublicKeyButton = new QPushButton(groupBox_3);
        exportPublicKeyButton->setObjectName(QString::fromUtf8("exportPublicKeyButton"));

        verticalLayout_5->addWidget(exportPublicKeyButton);


        verticalLayout_3->addWidget(groupBox_3);

        tabWidget->addTab(keyManagementTab, QString());
        issueLicenseTab = new QWidget();
        issueLicenseTab->setObjectName(QString::fromUtf8("issueLicenseTab"));
        verticalLayout_6 = new QVBoxLayout(issueLicenseTab);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        groupBox_4 = new QGroupBox(issueLicenseTab);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_4);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        loadPrivateKeyButton = new QPushButton(groupBox_4);
        loadPrivateKeyButton->setObjectName(QString::fromUtf8("loadPrivateKeyButton"));

        horizontalLayout_2->addWidget(loadPrivateKeyButton);

        privateKeyStatusLabel = new QLabel(groupBox_4);
        privateKeyStatusLabel->setObjectName(QString::fromUtf8("privateKeyStatusLabel"));

        horizontalLayout_2->addWidget(privateKeyStatusLabel);


        verticalLayout_6->addWidget(groupBox_4);

        groupBox_5 = new QGroupBox(issueLicenseTab);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        formLayout = new QFormLayout(groupBox_5);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(groupBox_5);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        customerNameLineEdit = new QLineEdit(groupBox_5);
        customerNameLineEdit->setObjectName(QString::fromUtf8("customerNameLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, customerNameLineEdit);

        label_2 = new QLabel(groupBox_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        licenseTypeLineEdit = new QLineEdit(groupBox_5);
        licenseTypeLineEdit->setObjectName(QString::fromUtf8("licenseTypeLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, licenseTypeLineEdit);

        label_3 = new QLabel(groupBox_5);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        hardwareIdsTextEdit = new QTextEdit(groupBox_5);
        hardwareIdsTextEdit->setObjectName(QString::fromUtf8("hardwareIdsTextEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, hardwareIdsTextEdit);

        label_4 = new QLabel(groupBox_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        expiryDateEdit = new QDateEdit(groupBox_5);
        expiryDateEdit->setObjectName(QString::fromUtf8("expiryDateEdit"));
        expiryDateEdit->setCalendarPopup(true);

        formLayout->setWidget(3, QFormLayout::FieldRole, expiryDateEdit);


        verticalLayout_6->addWidget(groupBox_5);

        generateLicenseButton = new QPushButton(issueLicenseTab);
        generateLicenseButton->setObjectName(QString::fromUtf8("generateLicenseButton"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        generateLicenseButton->setFont(font);

        verticalLayout_6->addWidget(generateLicenseButton);

        tabWidget->addTab(issueLicenseTab, QString());
        viewLicenseTab = new QWidget();
        viewLicenseTab->setObjectName(QString::fromUtf8("viewLicenseTab"));
        verticalLayout_7 = new QVBoxLayout(viewLicenseTab);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        groupBox_6 = new QGroupBox(viewLicenseTab);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        loadPublicKeyForVerificationButton = new QPushButton(groupBox_6);
        loadPublicKeyForVerificationButton->setObjectName(QString::fromUtf8("loadPublicKeyForVerificationButton"));

        horizontalLayout_3->addWidget(loadPublicKeyForVerificationButton);

        publicKeyStatusLabel = new QLabel(groupBox_6);
        publicKeyStatusLabel->setObjectName(QString::fromUtf8("publicKeyStatusLabel"));

        horizontalLayout_3->addWidget(publicKeyStatusLabel);


        verticalLayout_7->addWidget(groupBox_6);

        groupBox_7 = new QGroupBox(viewLicenseTab);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        verticalLayout_8 = new QVBoxLayout(groupBox_7);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verifyLicenseButton = new QPushButton(groupBox_7);
        verifyLicenseButton->setObjectName(QString::fromUtf8("verifyLicenseButton"));

        verticalLayout_8->addWidget(verifyLicenseButton);

        verificationResultLabel = new QLabel(groupBox_7);
        verificationResultLabel->setObjectName(QString::fromUtf8("verificationResultLabel"));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        verificationResultLabel->setFont(font1);
        verificationResultLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(verificationResultLabel);

        licenseDetailsTextEdit = new QTextEdit(groupBox_7);
        licenseDetailsTextEdit->setObjectName(QString::fromUtf8("licenseDetailsTextEdit"));
        licenseDetailsTextEdit->setReadOnly(true);

        verticalLayout_8->addWidget(licenseDetailsTextEdit);


        verticalLayout_7->addWidget(groupBox_7);

        tabWidget->addTab(viewLicenseTab, QString());
        localInfoTab = new QWidget();
        localInfoTab->setObjectName(QString::fromUtf8("localInfoTab"));
        verticalLayout_2 = new QVBoxLayout(localInfoTab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        localInfoTextEdit = new QTextEdit(localInfoTab);
        localInfoTextEdit->setObjectName(QString::fromUtf8("localInfoTextEdit"));
        localInfoTextEdit->setReadOnly(true);

        verticalLayout_2->addWidget(localInfoTextEdit);

        fetchLocalInfoButton = new QPushButton(localInfoTab);
        fetchLocalInfoButton->setObjectName(QString::fromUtf8("fetchLocalInfoButton"));

        verticalLayout_2->addWidget(fetchLocalInfoButton);

        tabWidget->addTab(localInfoTab, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\257\201\344\271\246\347\255\276\345\217\221\345\267\245\345\205\267", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\345\257\206\351\222\245\345\257\271\347\224\237\346\210\220", nullptr));
        generateKeysButton->setText(QCoreApplication::translate("MainWindow", "\347\224\237\346\210\220\346\226\260\347\232\204\345\257\206\351\222\245\345\257\271", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\347\247\201\351\222\245", nullptr));
        privateKeyTextEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\277\231\351\207\214\345\260\206\346\230\276\347\244\272\347\224\237\346\210\220\347\232\204\346\210\226\345\212\240\350\275\275\347\232\204\347\247\201\351\222\245...", nullptr));
        exportPrivateKeyButton->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\347\247\201\351\222\245\345\210\260\346\226\207\344\273\266...", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\345\205\254\351\222\245", nullptr));
        publicKeyTextEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\277\231\351\207\214\345\260\206\346\230\276\347\244\272\347\224\237\346\210\220\347\232\204\346\210\226\345\212\240\350\275\275\347\232\204\345\205\254\351\222\245...", nullptr));
        exportPublicKeyButton->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\345\205\254\351\222\245\345\210\260\346\226\207\344\273\266...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(keyManagementTab), QCoreApplication::translate("MainWindow", "\345\257\206\351\222\245\347\256\241\347\220\206", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "\347\255\276\345\220\215\347\247\201\351\222\245", nullptr));
        loadPrivateKeyButton->setText(QCoreApplication::translate("MainWindow", "\345\212\240\350\275\275\347\247\201\351\222\245\346\226\207\344\273\266...", nullptr));
        privateKeyStatusLabel->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201\357\274\232\346\234\252\345\212\240\350\275\275", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "\350\256\270\345\217\257\350\257\201\344\277\241\346\201\257", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\345\256\242\346\210\267\345\220\215\347\247\260:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\350\256\270\345\217\257\350\257\201\347\261\273\345\236\213:", nullptr));
        licenseTypeLineEdit->setText(QCoreApplication::translate("MainWindow", "enterprise", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\347\241\254\344\273\266\347\273\221\345\256\232ID (\345\244\232\350\241\214):", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\346\216\210\346\235\203\345\210\260\346\234\237\346\227\245\346\234\237:", nullptr));
        generateLicenseButton->setText(QCoreApplication::translate("MainWindow", "\347\224\237\346\210\220\345\271\266\344\277\235\345\255\230\350\256\270\345\217\257\350\257\201...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(issueLicenseTab), QCoreApplication::translate("MainWindow", "\350\257\201\344\271\246\347\255\276\345\217\221", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("MainWindow", "1. \345\212\240\350\275\275\351\252\214\350\257\201\345\205\254\351\222\245", nullptr));
        loadPublicKeyForVerificationButton->setText(QCoreApplication::translate("MainWindow", "\345\212\240\350\275\275\345\205\254\351\222\245\346\226\207\344\273\266...", nullptr));
        publicKeyStatusLabel->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201\357\274\232\346\234\252\345\212\240\350\275\275", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "2. \345\212\240\350\275\275\345\271\266\351\252\214\350\257\201\350\256\270\345\217\257\350\257\201", nullptr));
        verifyLicenseButton->setText(QCoreApplication::translate("MainWindow", "\345\212\240\350\275\275\350\256\270\345\217\257\350\257\201\346\226\207\344\273\266...", nullptr));
        verificationResultLabel->setText(QCoreApplication::translate("MainWindow", "\351\252\214\350\257\201\347\212\266\346\200\201\357\274\232\345\276\205\345\212\240\350\275\275", nullptr));
        licenseDetailsTextEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\277\231\351\207\214\345\260\206\346\230\276\347\244\272\345\267\262\351\252\214\350\257\201\347\232\204\350\256\270\345\217\257\350\257\201\347\232\204\350\257\246\347\273\206\345\206\205\345\256\271...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(viewLicenseTab), QCoreApplication::translate("MainWindow", "\350\257\201\344\271\246\346\237\245\347\234\213", nullptr));
        localInfoTextEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\347\202\271\345\207\273\342\200\234\350\216\267\345\217\226\344\277\241\346\201\257\342\200\235\346\214\211\351\222\256\346\235\245\346\230\276\347\244\272\346\234\254\346\234\272\347\232\204\347\241\254\344\273\266\346\240\207\350\257\206\347\254\246\343\200\202", nullptr));
        fetchLocalInfoButton->setText(QCoreApplication::translate("MainWindow", "\350\216\267\345\217\226\346\234\254\346\234\272\344\277\241\346\201\257", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(localInfoTab), QCoreApplication::translate("MainWindow", "\346\234\254\346\234\272\344\277\241\346\201\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
