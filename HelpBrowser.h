#pragma once

#include <QtWidgets>

class HelpBrowser : public QDialog
{
    Q_OBJECT

public:
    HelpBrowser(const QString& strPath, const QString& strFileName)
    {
        QPushButton*  pcmdHome    = new QPushButton("Главная страница");
        QTextBrowser* ptxtBrowser = new QTextBrowser;

        connect(pcmdHome, SIGNAL(clicked()), ptxtBrowser, SLOT(home()));

        ptxtBrowser->setSearchPaths(QStringList() << strPath);
        ptxtBrowser->setSource(QString(strFileName));

        QVBoxLayout* pvbxLayout = new QVBoxLayout;
        QHBoxLayout* phbxLayout = new QHBoxLayout;   
        phbxLayout->addWidget(pcmdHome);    
        pvbxLayout->addLayout(phbxLayout);    
        pvbxLayout->addWidget(ptxtBrowser);
        setLayout(pvbxLayout);
    }
};

