#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include "edge.h"
#include "node.h"


struct Edges
{
    Edge *edge;
    int source;
    int dest;
    int length;
    bool oriented;
};

class GraphWidget : public QGraphicsView
{
    Q_OBJECT
private:
    QTextCodec* codec;
    QList<Node*> listOfNode;// Список вершин, отображающихся на окне

public:
    QGraphicsScene *scene;  // Окно, на котором рисуются все элементы

    GraphWidget(QWidget *parent = 0);// Конструктор

    Node *findNode(int val);        // Найти вершину по номеру
    QPointF getPosOfNode(int index);// Возвращает местоположение вершины с номером index
    int getListOfNodeSize();        // Возвращает количество вершин

    void changeIndecesOfAllVerteces(); // Переопределяет индексы всех вершин

    QList<Edges *> getEdges(); // Возвращает список всех ребер
    QList<Node*> getNodes();

    void putNode(Node *node);

    void addNode();                      // Добавить вершину на экран по кнопке
    void addNode(QPointF position,int t);// Добавить вершину в указанном месте
    void deleteSelectedNodes();          // Удаляет выбранные вершины

    void addEdge(Node *source, Node *dest, unsigned length=1, bool type=true);// Добавить ребро
    void deleteEdge(Edge *edge);                              // Удалить ребро

    void deleteAllItems();// Удаляет все

    bool checkSameEdge(Node *source, Node *dest);   // Проверить, есть ли уже такое ребро
    bool checkReverseEdge(Node *source, Node *dest);// Проверить, есть ли обратное ребро0

    QVector<QVector <int>> getAdjacencyMatrix();
    QVector<QVector <int>> getIncidenceMatrix();
    QList<QList<int>> getlistOfAyliffe();

public slots:
    void deleteSelectedItems();// Удаляет выбранные элементы

protected:
    void resizeEvent(QResizeEvent *event);// Изменить размеры окна
    void drawBackground(QPainter *painter, const QRectF &rect);// Отрисовка фона
    void mouseDoubleClickEvent(QMouseEvent *event);// Отслеживание двойного клика мышкой
};
#endif // GRAPHWIDGET_H
