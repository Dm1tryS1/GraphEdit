#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QtWidgets>
#include <edge.h>

class Edge;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class Node : public QGraphicsItem // Вершина графа
{
    QTextCodec* codec;

private:
    int index;             // Номер вершины
    QList<Edge *> edgeList;// Список ссылок рёбер, с которыми соединена вершина
    GraphWidget *graph;    // Ссылка на виждет, на котором должна отображаться вершина

public:
    Node(GraphWidget *graphWidget);// Конструктор

    void addEdge(Edge *edge);          // Добавить к этой вершине ребро
    QList<Edge*> edges();              // Вернуть список рёбер, к которыми соединена эта вершина
    void removeEdge(Edge *deletedEdge);// Удалить ребро от этой вершины

    int getIndex();          // Возвращает номер вершины
    void setIndex(int index);// Нумерует вершину

protected:
    QRectF boundingRect() const override;// Просчитывает нужные координаты прямоугольника, внутри которого будет вершина и все надписи
    QPainterPath shape() const override; // Делает кликабельную область, в которой находится вершина
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;// Рисует вершину

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;// Регистрирует изменение положения вершины

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;// Измнение параметров вершины
};

#endif // NODE_H
