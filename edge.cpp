#include "edge.h"

Edge::Edge(Node *sourceNode, Node *destNode, unsigned len, bool flag)
{
    source = sourceNode;
    dest = destNode;

    source->addEdge(this);
    dest->addEdge(this);
    adjust();

    arrowSize = 15;
    length = len;

    oriented = flag;
    color = Qt::black;

    codec = QTextCodec::codecForName("Windows-1251");
    setFlags(ItemIsSelectable);
    setZValue(+1);
}

int Edge::getLength()
{
    return length;
}

void Edge::setColor(QBrush newColor)
{
    color = newColor;
    update();
}

void Edge::setStyle(Qt::PenStyle st)
{
    style = st;
    update();
}

Node *Edge::sourceNode()
{
    return source;
}

void Edge::setLength(unsigned value)
{
    length = value;
    update();
}

Node *Edge::destNode()
{
    return dest;
}

void Edge::setOriented(bool flag)
{
    oriented = flag;
}

bool Edge::getOriented()
{
    return oriented;
}


void Edge::adjust()
{
    if (!source || !dest) return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(30))
    {
        QPointF edgeOffset((line.dx() * 15) / length, (line.dy() * 15) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    }
    else
        sourcePoint = destPoint = line.p1();
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest) return QRectF();
    qreal extra = arrowSize+20;
    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),destPoint.y() - sourcePoint.y())).normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath Edge::shape() const
{
    QPolygonF nPolygon;
    QLineF line(sourcePoint.x(), sourcePoint.y(),destPoint.x(),destPoint.y());
    qreal radAngle = line.angle()* M_PI / 180;
    qreal dx = 10 * sin(radAngle);
    qreal dy = 10 * cos(radAngle);
    QPointF offset1 = QPointF(dx, dy);
    QPointF offset2 = QPointF(-dx, -dy);
    nPolygon << line.p1() + offset1 << line.p1() + offset2 << line.p2() + offset2 << line.p2() + offset1;

    QPainterPath path;
    path.addPolygon(nPolygon);
    return path;
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if (!source || !dest) return;

    QLineF line(sourcePoint, destPoint);
    if (line.length() == 0) return;

    // Рисуем линию
    if (option->state & QStyle::State_Selected) //Проверка на выделение вершины
        painter->setPen(QPen(QColor(254, 0, 0, 255), 2));
    else
        painter->setPen(QPen(color, 2, Qt::SolidLine));

    painter->drawLine(line);
    QFont font;
    font.setPixelSize(15);
    painter->setFont(font);


    // Рисуем стрелку
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = 2*M_PI - angle;

    QPointF destArrowP1 = destPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                              cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                              cos(angle - M_PI + M_PI / 3) * arrowSize);
    // Рисуем надпись
    int weight = length;
    QString output;
    output += "L:" + QString::number(weight);

    painter->drawText((sourcePoint+destPoint)/2, output);

    painter->setBrush(Qt::black);

    if (oriented)
        painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}

void Edge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        event->accept();
        return;
    }
    QGraphicsItem::mousePressEvent(event);
}

void Edge::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (!this->isSelected())
        return;
    QGraphicsItem::contextMenuEvent(event);

    // Создание меню
    QMenu menu;
    menu.addSeparator();
    QAction *  new_val = menu.addAction(codec->toUnicode("Настройки"));

    // Выбор пункта меню
    QAction * selected_action = menu.exec(event->screenPos());
    // Выполнение действий
    if(selected_action == new_val)
    {
         QMessageBox msgBox;
         msgBox.setWindowIcon(QIcon(":/icons/icon.png"));
         msgBox.setText(codec->toUnicode("Настройки ребра"));
         msgBox.setWindowTitle(codec->toUnicode("Настройки"));
         QLabel *text1 = new QLabel();
         text1->setText(codec->toUnicode("Вес ребра:"));
         QLabel *text2 = new QLabel();
         text2->setText(codec->toUnicode("Тип ребра:"));
         QSpinBox *spin = new QSpinBox();
         spin->setRange(1, 1000);
         spin->setValue(this->length);
         QComboBox *type = new QComboBox();
         if (this->getOriented())
         {
            type->addItem(QString(codec->toUnicode("Ориентированное")));
            type->addItem(QString(codec->toUnicode("Неориентированное")));
         }
         else
         {
            type->addItem(QString(codec->toUnicode("Неориентированное")));
            type->addItem(QString(codec->toUnicode("Ориентированное")));
         }

         msgBox.layout()->addWidget(text1);
         msgBox.layout()->addWidget(spin);
         msgBox.layout()->addWidget(text2);
         msgBox.layout()->addWidget(type);

         QAbstractButton *okButton = msgBox.addButton(codec->toUnicode("Изменить"), QMessageBox::AcceptRole);
         QAbstractButton *canleButton = msgBox.addButton(codec->toUnicode("Отмена"),QMessageBox::RejectRole);
         msgBox.exec();
         if (msgBox.clickedButton() == okButton)
         {
            length = spin->value();
            if (type->currentText() == QString(codec->toUnicode("Ориентированное")))
                this->setOriented(true);
            else
                this->setOriented(false);
         }
         update();
    }
    setSelected(false);
}
