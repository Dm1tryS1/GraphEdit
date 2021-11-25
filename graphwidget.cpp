#include "graphwidget.h"

GraphWidget::GraphWidget(QWidget *parent): QGraphicsView(parent)//�����������
{
    codec = QTextCodec::codecForName("Windows-1251");
    scene = new QGraphicsScene(this);
    setScene(scene);
    scene->setSceneRect(0, 0, width(), height());
}

QPointF GraphWidget::getPosOfNode(int index)
{
    return listOfNode.at(index)->pos();
}

QList<Node*> GraphWidget::getNodes()
{
    return listOfNode;
}

void GraphWidget::putNode(Node *node)
{
    listOfNode<<node;
}
int GraphWidget::getListOfNodeSize()
{
    return listOfNode.size();
}

void GraphWidget::resizeEvent(QResizeEvent *event)
//���������� ��� ��������� ������� ����
{
    scene->setSceneRect(0, 0, width(), height());
    QGraphicsView::resizeEvent(event);
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &)
// ��������� ����
{
    QRectF sceneRect = this->sceneRect();
    painter->fillRect(sceneRect, QColor(255, 255, 255, 255));//���������� ������� � ���� ���������
}

bool GraphWidget::checkSameEdge(Node *source, Node *dest)
{
    foreach (Edge *compareEdge, source->edges())
        if ((compareEdge->sourceNode() == source && compareEdge->destNode() == dest))
            return true;
    return false;
}

bool GraphWidget::checkReverseEdge(Node *source, Node *dest)
{
    foreach (Edge *compareEdge, source->edges())
        if ((compareEdge->sourceNode() == dest && compareEdge->destNode() == source))
            return true;
    return false;
}

void GraphWidget::addNode()//��������� �������
{
    if (getListOfNodeSize() < 100)
    {
        Node *node = new Node(this);// ������ ����� �������
        scene->addItem(node);       // ��������� � �� �����

        listOfNode << node;// �������� ��� ������� � ������ ������

        changeIndecesOfAllVerteces(); // ��������� ����� ������ �������

    //  ��������� ����� ����������� ������ ������������ ����������
        int i = node->getIndex();
        node->setPos(QPointF(50+i*10, 50+i*10));
        update(); // �������� �����
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(":/icons/icon.png"));
        msgBox.setText(codec->toUnicode("������������ ���������� ������: 100"));
        msgBox.setWindowTitle(codec->toUnicode("��������������!"));
        msgBox.exec();
    }
}

void GraphWidget::addNode(QPointF position,int t)
{
    if (getListOfNodeSize() < 100)
    {
        Node *node = new Node(this); // ������ ����� �������
        node->setIndex(t);
        scene->addItem(node); // ��������� � �� �����
        listOfNode << node; // �������� ��� ������� � ������ ������
        node->setPos(position);
        update(); // �������� �����
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(":/icons/icon.png"));
        msgBox.setText(codec->toUnicode("������������ ���������� ������: 100"));
        msgBox.setWindowTitle(codec->toUnicode("��������������!"));
        msgBox.exec();
    }

}

void GraphWidget::addEdge(Node *source,Node *dest, unsigned length, bool type)
{
    if (source == dest) return;
    if ((!checkSameEdge(source,dest))&&(!checkReverseEdge(source,dest)))
    {
        Edge *edge = new Edge(source, dest, length,type);
        scene->addItem(edge);
    }
}

void GraphWidget::deleteEdge(Edge *edge)
{
    scene->removeItem(edge);
    edge->destNode()->removeEdge(edge);
    edge->sourceNode()->removeEdge(edge);
    delete edge;
}

void GraphWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)      // ���� ��� ��������� ������� ���� ����� ������� ����
        addNode(QPointF(event->pos().x(), event->pos().y()),listOfNode.size());
    else if (event->button() == Qt::RightButton)// ���� ��� ��������� ������� ���� ������ ������� ����
    {
        Node *dest = dynamic_cast<Node*>(itemAt(event->pos()));// �������, � ������� ���� �������� ��� ���� �� ��������� ������

        QList<Node*> NodeList;// ������ ��������� ������

        foreach (QGraphicsItem *item, scene->selectedItems())// ���� �� ��������� ��������� �������
            if (dynamic_cast<Node*>(item))
                NodeList << dynamic_cast<Node*>(item);

        foreach (Node *item, NodeList)// ��� ���� ��������� ������
        {
            Node *source = item; // �������� ������� ������� � ���� Node

            if (source == dest) continue;

            if ((!checkReverseEdge(source,dest))&&(!checkSameEdge(source,dest)))
            {
                QMessageBox msgBox;
                QLabel *text1 = new QLabel();
                text1->setText(codec->toUnicode("��� �����:"));
                QLabel *text2 = new QLabel();
                text2->setText(codec->toUnicode("��� �����:"));
                QSpinBox *length = new QSpinBox();
                QComboBox *type = new QComboBox();
                type->addItem(QString(codec->toUnicode("���������������")));
                type->addItem(QString(codec->toUnicode("�����������������")));

                length->setRange(1, 1000);
                msgBox.layout()->addWidget(text1);
                msgBox.layout()->addWidget(length);
                msgBox.layout()->addWidget(text2);
                msgBox.layout()->addWidget(type);


                msgBox.setWindowIcon(QIcon(":/icons/icon.png"));
                msgBox.setText(codec->toUnicode("��������� ������������ �����"));
                msgBox.setWindowTitle(codec->toUnicode("��������� �����"));
                QAbstractButton *okButton = msgBox.addButton(codec->toUnicode("��������"), QMessageBox::AcceptRole);
                QAbstractButton *canleButton = msgBox.addButton(codec->toUnicode("������"),QMessageBox::RejectRole);
                msgBox.exec();

                unsigned lengthNum = length->value();
                bool flag;

                if (type->currentText() == QString(codec->toUnicode("���������������")))
                    flag = true;
                else
                    flag = false;

                if (msgBox.clickedButton() == okButton)
                    scene->addItem(new Edge(source, dest, lengthNum,flag));
                update();
            }

        }
    }
}

void GraphWidget::changeIndecesOfAllVerteces()
{
    foreach (Node *itemNode, listOfNode)
    {
        itemNode->setIndex(listOfNode.indexOf(itemNode));
        itemNode->update();
    }
}

QList<Edges*> GraphWidget::getEdges()
{
    QList<Edges*> EdgesList;
    foreach (Node *Node, listOfNode)
    {
        foreach (Edge *edge, Node->edges())
        {
            if (Node->getIndex() == edge->destNode()->getIndex()) continue;
            Edges *curr = new Edges;
            curr->source = Node->getIndex();          // ������ ������� �����
            curr->dest = edge->destNode()->getIndex();// ������ ������� �����
            curr->length = edge->getLength();         // ����� �����
            curr->oriented = edge->getOriented();     // ����������
            curr->edge = edge;                        // ������ �� ���� �����
            EdgesList << curr;
        }
    }
    return EdgesList;
}

void GraphWidget::deleteSelectedItems()
{
    QList<Node*> NodeList;// ������ ��������� ������

    foreach (QGraphicsItem *item, scene->selectedItems())// ���� �� ��������� ��������� �������
        if (dynamic_cast<Node*>(item))
            NodeList << dynamic_cast<Node*>(item);


    foreach (Node *node, NodeList)// ��� ������ ��������� �������
    {
        scene->removeItem(node);   // ������ � �� ����
        listOfNode.removeAll(node);// ������ �� ������ ������

        foreach (Edge *itemEdge, node->edges())// ��� ������� �����, ���������� � ������ ��������
        {
            scene->removeItem(itemEdge);                 // ������ ����� �� ����
            itemEdge->destNode()->removeEdge(itemEdge);  // ������ ����� �� ������ ���� ������ �������
            itemEdge->sourceNode()->removeEdge(itemEdge);// ������ ����� �� ������ ���� ������ �������
            delete itemEdge;                             // ������� ���� �����
         }
        delete node;// ������� ���� �������
    }

    foreach (QGraphicsItem *item, scene->selectedItems())// ��� ������ ���������� �����
    {
        Edge *edge = dynamic_cast<Edge*>(item);
        if (edge)
        {
            scene->removeItem(edge);             // ������ ����� �� ����
            edge->destNode()->removeEdge(edge);  // ������ ����� �� ������ ���� ������ �������
            edge->sourceNode()->removeEdge(edge);// ������ ����� �� ������ ���� ������ �������
            delete edge;                         // ������� ���� �����
        }
    }
}

void GraphWidget::deleteSelectedNodes()
{
    QList<Node*> NodeList; //������ ��������� ������

    foreach (QGraphicsItem *item, scene->selectedItems()) //���� �� ��������� ��������� �������
        if (dynamic_cast<Node*>(item))
            NodeList << dynamic_cast<Node*>(item);


    foreach (Node *node, NodeList)
    {
        scene->removeItem(node);   // ������ � �� ����
        listOfNode.removeAll(node);// ������ �� ������ ������

        foreach (Edge *itemEdge, node->edges())// ��� ������� �����, ���������� � ������ ��������
        {
            scene->removeItem(itemEdge);                 // ������ ����� �� ����
            itemEdge->destNode()->removeEdge(itemEdge);  // ������ ����� �� ������ ���� ������ �������
            itemEdge->sourceNode()->removeEdge(itemEdge);// ������ ����� �� ������ ���� ������ �������
            delete itemEdge;                             // ������� ���� �����
        }
        delete node;// ������� ���� �������
    }
}



void GraphWidget::deleteAllItems() //������� ��� �������
{
   foreach (Node *itemNode, listOfNode)
   {
        scene->removeItem(itemNode);//������ � �� ����

        foreach (Edge *itemEdge, itemNode->edges())
        {
            scene->removeItem(itemEdge);                 // ������ ����� �� ����
            itemEdge->destNode()->removeEdge(itemEdge);  // ������ ����� �� ������ ���� ������ �������
            itemEdge->sourceNode()->removeEdge(itemEdge);// ������ ����� �� ������ ���� ������ �������
            delete itemEdge;                             // ������� ���� �����
        }
        itemNode->edges().clear();
        delete itemNode;//������� ���� �������
   }
   listOfNode.clear();
   scene->clear();
}

Node* GraphWidget::findNode(int val)// ����� ������� �� �� ������ �������
{
    foreach (Node *itemNode, listOfNode)
    {
        if (itemNode->getIndex() == val)
            return itemNode;

    }
    return nullptr;
}

QVector<QVector <int>> GraphWidget::getAdjacencyMatrix()
{
    QVector<QVector<int>> matrix;

    for (int i = 0; i<getListOfNodeSize();i++)
    {
       QVector<int> temp;
       for (int j = 0; j<getListOfNodeSize();j++)
           temp.push_back(0);
       matrix.push_back(temp);
    }

    foreach(Node *node, listOfNode)
    {
        int i = node->getIndex();
        foreach(Edge *edge, node->edges())
        {
            if (edge->getOriented())
            {
                if (edge->destNode()->getIndex() != i)
                    matrix[i][edge->destNode()->getIndex()] = edge->getLength();
            }
            else
                if (edge->destNode()->getIndex() != i)
                     matrix[i][edge->destNode()->getIndex()] = edge->getLength();
                else
                    matrix[i][edge->sourceNode()->getIndex()] = edge->getLength();

        }
    }
    return matrix;

}

QVector<QVector <int>>  GraphWidget::getIncidenceMatrix()
{
    QVector<QVector<int>> matrix;

    for (int i=0; i<getListOfNodeSize();i++)
    {
        QVector<int> temp;
        for (int j = 0; j<getEdges().size();j++)
            temp.push_back(0);
        matrix.push_back(temp);
    }


    int ind=0;

    foreach (Edges *edge, getEdges())
    {
        if (!edge->edge->getOriented())
        {
            matrix[edge->source][ind] = edge->length;
            matrix[ edge->dest][ind] = edge->length;

        }
        else
        {
            matrix[edge->source][ind] = edge->length;
            matrix[edge->dest][ind] = edge->length * (-1);
        }
        ind++;
    }

    return matrix;
}

QList<QList<int>> GraphWidget::getlistOfAyliffe()
{
    QList<QList<int>> list;
    foreach(Node *node, getNodes())
    {
        QList<int> temp;
        int i = node->getIndex();

        foreach(Edge *edge, node->edges())
        {
            if (edge->getOriented())
            {
                if (edge->destNode()->getIndex() != i)
                    temp << edge->destNode()->getIndex();
            }
            else
                if (edge->destNode()->getIndex() != i)
                    temp << edge->destNode()->getIndex();
                else
                    temp << edge->sourceNode()->getIndex();

        }
        list << temp;
    }

    return list;
}
