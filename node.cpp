#include "graphwidget.h"

Node::Node(GraphWidget *graphWidget): graph(graphWidget)
{
    codec = QTextCodec::codecForName("Windows-1251");
    setFlags(ItemIsSelectable | ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(+1);
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

void Node::removeEdge(Edge *deletedEdge)
{
    edgeList.removeAll(deletedEdge);
}

QList<Edge*> Node::edges()
{
    return edgeList;
}

int Node::getIndex()
{
    return index;
}

void Node::setIndex(int index)
{
    this->index = index;
}

QRectF Node::boundingRect() const
{
    return QRectF( -30, -30, 60, 60);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-15, -15, 30, 30);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if (option->state & QStyle::State_Selected) //Проверка на выделение вершины
        painter->setBrush(QColor(255, 255, 30, 255));
    else
        painter->setBrush(QColor(190, 255, 140, 255));

    painter->setPen(QPen(QColor(130, 180, 130, 255), 3));
    painter->drawEllipse(-15, -15, 30, 30);
    painter->setPen(QPen(Qt::black));
    painter->setFont(QFont("Roboto", 9));

    QString text = QString::number(index);
    if (index < 10)
        painter->drawText(-4, 5, text);
    else
        painter->drawText(-8, 5, text);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    }

    return QGraphicsItem::itemChange(change, value);
}

void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsItem::contextMenuEvent(event);
    if (!this->isSelected())
        return;

    QMenu menu;
    menu.addSeparator();

    QAction *new_chose = menu.addAction(codec->toUnicode("Настройки"));
    QAction *selected_action = menu.exec(event->screenPos());

    if(selected_action == new_chose)
    {
        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(":/icons/icon.png"));
        msgBox.setText(codec->toUnicode("Настройки вершины"));
        msgBox.setWindowTitle(codec->toUnicode("Настройки"));

        QComboBox *dest = new QComboBox();
        QSpinBox *length = new QSpinBox();
        length->setRange(1, 1000);

        QLabel *text1 = new QLabel();
        text1->setText(codec->toUnicode("Ребро:"));
        QLabel *text2 = new QLabel();
        text2->setText(codec->toUnicode("Вес ребра:"));

        foreach (Edge *i, edgeList)
        if (i->destNode()->getIndex() != this->getIndex())
            dest->addItem(QString::number(i->destNode()->getIndex()));
        else if (!i->getOriented())
            dest->addItem(QString::number(i->sourceNode()->getIndex()));

        msgBox.layout()->addWidget(text1);
        msgBox.layout()->addWidget(dest);
        msgBox.layout()->addWidget(text2);
        msgBox.layout()->addWidget(length);
        QAbstractButton *okButton = msgBox.addButton(codec->toUnicode("Изменить"), QMessageBox::AcceptRole);
        QAbstractButton *canleButton = msgBox.addButton(codec->toUnicode("Отмена"),QMessageBox::RejectRole);
        msgBox.exec();

        int destNum = (dest->currentText()).toInt();
        int lengthNum = length->value();

        if (msgBox.clickedButton() == okButton)
            foreach (Edge *i, edgeList)
                if ((i->destNode()->getIndex() == destNum) || ((!i->getOriented()) && (i->sourceNode()->getIndex() == destNum)))
                {
                    i->setLength(static_cast<unsigned>(lengthNum));
                    break;
                }
        update();
     }
     setSelected(false);
}

