#include "tablewindow.h"
#include "ui_tablewindow.h"

TableWindow::TableWindow(QWidget *parent) : QWidget(parent), ui(new Ui::TableWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(15);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(15);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWindowIcon(QIcon(":/icons/icon.png"));
}

TableWindow::~TableWindow()
{
    delete ui;
}

void TableWindow::setCount(int x, int y)
{
    QStringList listRow;
    QStringList listColumn;

    for (int i=0;i<x;i++)
        listRow << QString::number(i);

    for (int j=0;j<y;j++)
        listColumn << QString::number(j);

    ui->tableWidget->setRowCount(x);
    ui->tableWidget->setColumnCount(y);

    ui->tableWidget->setHorizontalHeaderLabels(listColumn);
    ui->tableWidget->setVerticalHeaderLabels(listRow);
}

void TableWindow::clear()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
}

void TableWindow::setNumber(int i, int j, short x)
{
    QTableWidgetItem* item = new QTableWidgetItem(QString::number(x));
    ui->tableWidget->setItem(i,j,item);
}

void TableWindow::on_pb_close_clicked()
{
    this->close();
    emit mainWindow();
}

void TableWindow::setflag(bool flag)
{
    additionalDeletion = flag;
}
bool TableWindow::getflag()
{
    return additionalDeletion;
}
