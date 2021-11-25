#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QWidget>

namespace Ui {
class TableWindow;
}

class TableWindow : public QWidget
{
    Q_OBJECT
    int count;
    bool additionalDeletion;

public:
    TableWindow(QWidget *parent = 0);
    ~TableWindow();

    void setCount(int x, int y);
    void clear();
    void setNumber(int i, int j, short x);
    void setflag(bool flag);
    bool getflag();


signals:
    void mainWindow();

private:
    Ui::TableWindow *ui;

private slots:
    void on_pb_close_clicked();

};

#endif // TABLEWINDOW_H
