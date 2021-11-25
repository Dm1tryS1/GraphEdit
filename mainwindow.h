#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "HelpBrowser.h"
#include "graphwidget.h"
#include "tablewindow.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    GraphWidget *graph;
    TableWindow *tablewindow;
    Ui::MainWindow *ui;

public:
     MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QString InputPath ="C:\\";

private slots:        
    void deleteItems();// Удаляет выбранные элементы графа

    void on_pb_CreateNode_clicked();// Создать вершину
    void on_pb_RemoveNode_clicked();// Удалить выбранные вершины

    void on_pb_CreateEdge_clicked();// Создать ребро
    void on_pb_RemoveEdge_clicked();// Удалить ребра

    void on_pb_OpenButton_clicked();// Открыть файл .grahp
    void on_pb_SaveButton_clicked();// Сохранить файл .grahp

    void on_pb_ClearAll_clicked();// Очистить всё
    void on_pb_Help_clicked();    // Открыть справку

    void on_pb_CoordinatesOfAllNode_clicked();// Выписывает координаты всех вершин графа
    void on_pb_LengthOfEdges_clicked();// Выписывает длину всех рёбер графа
//----------------------------------------------------------------------------------------//
    void on_action_Open_file_triggered();
    void on_action_Save_File_triggered();

    void on_action_Save_Inc_triggered();
    void on_action_Save_Adj_triggered();
    void on_action_Save_List_triggered();

    void on_action_createNode_triggered();
    void on_action_createEdge_triggered();

    void on_action_adjacencyMatrix_triggered();
    void on_action_incidenceMatrix_triggered();
    void on_action_listOfAyliffe_triggered();

    void on_action_Algoritm_triggered();

    void on_action_removeEdge_triggered();
    void on_action_removeEdges_triggered();
    void on_action_clear_all_triggered();

    void on_action_triggered();
    void on_action_About_triggered();

    void enable();// Разблокирует главное окно
    void closeEvent(QCloseEvent *event); // Закрывает главное окно и внутренние окна
};

#endif // MAINWINDOW_H
