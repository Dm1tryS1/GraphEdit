#ifndef EDGE_H
#define EDGE_H
#define _USE_MATH_DEFINES
#include <QGraphicsItem>
#include <QtWidgets>
#include <node.h>

class Node;
class Edge : public QGraphicsItem
{
private:
    QTextCodec* codec;

    Node *source, *dest;// Вершины начала и конца
    bool oriented;

    qreal arrowSize;   // Вес дуги
    QBrush color;      // Цвет дуги
    Qt::PenStyle style;// Стиль текста

    QGraphicsScene *scene;

    unsigned length;// Вес ребра

public:
    QPointF sourcePoint;// Координаты вершины начала
    QPointF destPoint;  // Координаты вершины конца

    Edge(Node *sourceNode, Node *destNode, unsigned len=1, bool flag = true);//Коструктор

    Node *sourceNode();// Вернуть начальную вершину
    Node *destNode();  // Вернуть конечную вершину

    void adjust();// Определяем местоположение ребра

    void setColor(QBrush newColor);// Установить цвет
    void setStyle(Qt::PenStyle st);// Установить стиль

    void setOriented(bool flag);// Установить тип ребра
    bool getOriented();         // Получить тип ребра

    int getLength();// Возвращает вес ребра
    void setLength(unsigned value);// Установить вес

protected:
    QRectF boundingRect() const override;// Просчитывает нужные координаты прямоугольника, внутри которого будет ребро и все надписи
    QPainterPath shape() const override; // Делает кликабельную область, в которой находится ребро
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;// Рисует ребро

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;// Событие нажатие на ребро
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;// Событие вызов меню ребра
};

#endif // EDGE_H
