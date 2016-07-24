/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLabel *capturelbl;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *subNameLbl;
    QPushButton *addImgBtn;
    QVBoxLayout *verticalLayout;
    QComboBox *comboBox;
    QPushButton *trainBtn;
    QPushButton *recBtn;
    QPlainTextEdit *consuletxt;
    QLabel *faceROI;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1018, 771);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        capturelbl = new QLabel(centralWidget);
        capturelbl->setObjectName(QStringLiteral("capturelbl"));
        capturelbl->setAutoFillBackground(true);
        capturelbl->setFrameShape(QFrame::Box);

        gridLayout->addWidget(capturelbl, 0, 0, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        subNameLbl = new QLineEdit(centralWidget);
        subNameLbl->setObjectName(QStringLiteral("subNameLbl"));
        QFont font;
        font.setPointSize(12);
        subNameLbl->setFont(font);

        verticalLayout_2->addWidget(subNameLbl);

        addImgBtn = new QPushButton(centralWidget);
        addImgBtn->setObjectName(QStringLiteral("addImgBtn"));
        addImgBtn->setFont(font);

        verticalLayout_2->addWidget(addImgBtn);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setFont(font);

        verticalLayout->addWidget(comboBox);

        trainBtn = new QPushButton(centralWidget);
        trainBtn->setObjectName(QStringLiteral("trainBtn"));
        trainBtn->setFont(font);

        verticalLayout->addWidget(trainBtn);


        horizontalLayout->addLayout(verticalLayout);

        recBtn = new QPushButton(centralWidget);
        recBtn->setObjectName(QStringLiteral("recBtn"));
        recBtn->setFont(font);

        horizontalLayout->addWidget(recBtn);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 2);
        horizontalLayout->setStretch(2, 2);

        verticalLayout_3->addLayout(horizontalLayout);

        consuletxt = new QPlainTextEdit(centralWidget);
        consuletxt->setObjectName(QStringLiteral("consuletxt"));

        verticalLayout_3->addWidget(consuletxt);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 2);

        gridLayout->addLayout(verticalLayout_3, 1, 0, 1, 2);

        faceROI = new QLabel(centralWidget);
        faceROI->setObjectName(QStringLiteral("faceROI"));
        faceROI->setAutoFillBackground(true);
        faceROI->setFrameShape(QFrame::Box);

        gridLayout->addWidget(faceROI, 0, 1, 1, 1);

        gridLayout->setRowStretch(0, 2);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1018, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        capturelbl->setText(QString());
        subNameLbl->setInputMask(QString());
        subNameLbl->setText(QApplication::translate("MainWindow", "Enter your name", 0));
        addImgBtn->setText(QApplication::translate("MainWindow", "add image", 0));
        trainBtn->setText(QApplication::translate("MainWindow", "train", 0));
        recBtn->setText(QApplication::translate("MainWindow", "recognize faces", 0));
        faceROI->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
