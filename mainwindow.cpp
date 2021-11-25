#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/icon.png"));

    ui->action_deleteItem->setShortcut(tr("Delete"));// Создаётся сигнал для удаления
    ui->action_Open_file->setShortcut(QKeySequence("CTRL+O"));// Создаётся сигнал для открытия файла
    ui->action_Save_File->setShortcut(QKeySequence("CTRL+S"));// Создаётся сигнал для сохранения файла
    ui->action->setShortcut(QKeySequence("F1"));// Создаётся сигнал для открытия справки

    graph = ui->graphicsView;
    tablewindow = new TableWindow();// Cоздание окна представлений

    ui->lineEdit_Source->setValidator(new QIntValidator(0, 99, this));// Добавление валидации для поля ввода начальной вершины
    ui->lineEdit_Dest->setValidator(new QIntValidator(0, 99, this));// Добавление валидации для поля ввода конечной вершины

    graph->setDragMode(QGraphicsView::RubberBandDrag);
    graph->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graph->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graph->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    graph->setInteractive(true);
    graph->setRenderHint(QPainter::Antialiasing);

    connect(ui->action_deleteItem, SIGNAL(triggered(bool)), this, SLOT(deleteItems()));
    connect(ui->actionMenuFileExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(tablewindow, &TableWindow::mainWindow, this, &MainWindow::enable);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enable()
{
    setEnabled(true); // Разблокировка окна

    // Удаление нумерации ребер, если она была
    if (tablewindow->getflag())
        foreach (Edges *edge, graph->getEdges())
        {
            qreal x = (edge->edge->sourcePoint.x() + edge->edge->destPoint.x())/2;// Ищем координату Х индекса ребра
            qreal y = (edge->edge->sourcePoint.y() + edge->edge->destPoint.y())/2;// Ищем координату У индекса ребра

            graph->scene->removeItem(graph->itemAt(QPoint(x,y)));// Удаляем индекс
        }
    update();
}

void MainWindow::deleteItems()
{
    graph->deleteSelectedItems();// Удаляем выбранные элементы
    graph->changeIndecesOfAllVerteces();// Меняем индексы вершин
}

void MainWindow::on_pb_CoordinatesOfAllNode_clicked()
{
    QString s = "Координаты вершин: \n";
    for (int i = 0; i < graph->getListOfNodeSize(); i++)
        s += QString::number(i) + " = {"  +  QString::number(graph->getPosOfNode(i).x()) + ";" + QString::number(graph->getPosOfNode(i).y()) + "}\n";
    ui->textBrowser->setText(s);
}

void MainWindow::on_pb_LengthOfEdges_clicked()
{
    QString s = "Длины рёбер:\n\n";
    QString flag;
    for (int i = 0; i < graph->getEdges().size(); i++)
    {
        if (graph->getEdges().at(i)->oriented)
            flag = "ориентированное";
        else
            flag = "неориентированное";
        s += QString::number(i) + " : ("  +  QString::number(graph->getEdges().at(i)->source) + ";" + QString::number(graph->getEdges().at(i)->dest) + ";" + QString::number(graph->getEdges().at(i)->length) + ";" + flag + ")\n";
    }
    ui->textBrowser->setText(s);
}

void MainWindow::on_pb_OpenButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть граф",InputPath, "Graph file (*.graph)");
    if (!fileName.isEmpty())// Проверка пустой ли файл
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))// Открытие файла для чтения
        {
            QTextStream in(&file);
            graph->deleteAllItems();// Удаляем предыдущий граф

            QFileInfo fi(file);

            QString fn4 = fi.absolutePath();
            InputPath = fn4;// Сохраняем текущее расположение файла

            int vertices_count, edges_count;
            in >> vertices_count;// Считываем количество вершин

            // Добавление вершин
            for (int i = 0; i < vertices_count; i++)
            {
                int id, x, y;
                in >> id >> x >> y;

                Node *node = new Node(graph);// Создаём новую вершину
                node->setIndex(id);          // Устанавливаем id
                node->setPos(x,y);           // Cтавим в нужное место
                graph->scene->addItem(node); // Добавляем её на экран
                graph->putNode(node);        // Добавить эту вершину в список вершин
            }
            graph->update();// Обновяем экран*/

            bool accept = true;
            in >> edges_count;// Считываем количество ребер
            // Добавление ребер
            for (int i = 0; i < edges_count; i++)
            {
                accept = true;
                int uid, vid;
                unsigned length;
                int oriented;
                in >> uid >> vid >> length >> oriented;
                Node *source, *dest;

                source = graph->findNode(uid);// Находим начальную вершину
                dest = graph->findNode(vid);// Находим конечную вершину

                foreach (Edges *edge, graph->getEdges())
                {
                    if (edge->source == uid && edge->dest == vid)
                        accept = false;
                }
                if (accept && source && dest)
                    graph->scene->addItem(new Edge(source, dest, length,oriented));
            }
            graph->update();// Обновляем сцену
            file.close();// Закрываем файл

            if(file.open(QIODevice::ReadOnly | QIODevice::Text))//Выписываем содержимое файла
            {
                QTextStream in(&file);
                ui->textBrowser->setText(in.readAll());
            }
            file.close();// Закрываем файл

        }
        else
        {
            QMessageBox::information(this, "Невозможно открыть файл", file.errorString());
            return;
        }
    }
    else return;
}

void MainWindow::on_pb_SaveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить граф", InputPath, "Graph file (*.graph)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            out <<  graph->getListOfNodeSize() << endl;
            for (int i = 0;i < graph->getListOfNodeSize(); i++)
                out << i << " " << graph->getPosOfNode(i).x() << " " << graph->getPosOfNode(i).y() << endl;

            out <<graph->getEdges().size() << endl;
            foreach (Edges *item, graph->getEdges())
               out << item->source << " " << item->dest << " " << item->length << " " << item->oriented << endl;
            file.close();
        }
        else
        {
            QMessageBox::information(this, "Невозможно открыть файл", file.errorString());
            return;
        }
    }
    else return;
}

void MainWindow::on_action_Save_Inc_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить граф", InputPath, "Graph file (*.incdnc)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            QVector<QVector<int>> matrix = graph->getIncidenceMatrix();
            for (int i = 0;i < matrix.size(); i++)
            {
                out << i << ": ";
                for (int j = 0; j < matrix[i].size(); j++)
                    out << matrix[i][j] << " ";
                out << endl;
            }
        }
        else
        {
            QMessageBox::information(this, "Невозможно открыть файл", file.errorString());
            return;
        }
    }
}

void MainWindow::on_action_Save_Adj_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить граф", InputPath, "Graph file (*.adjcnc)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            QVector<QVector<int>> matrix = graph->getAdjacencyMatrix();
            for (int i = 0;i < matrix.size(); i++)
            {
                out << i << ": ";
                for (int j = 0; j < matrix[i].size(); j++)
                    out << matrix[i][j] << " ";
                out << endl;
            }
        }
        else
        {
            QMessageBox::information(this, "Невозможно открыть файл", file.errorString());
            return;
        }
    }
    else return;
}

void MainWindow::on_action_Save_List_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить граф", InputPath, "Graph file (*.lst)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            QList<QList<int>> list = graph->getlistOfAyliffe();
            for (int i=0; i<list.size();i++)
            {
                out << i << ": ";
                for (int j=0;j<list[i].size();j++)
                    out << list[i][j] << "; ";
                out << endl;
            }
        }
        else
        {
            QMessageBox::information(this, "Невозможно открыть файл", file.errorString());
            return;
        }
    }
    else return;
}

void MainWindow::on_pb_RemoveEdge_clicked() //Удалить рёбра
{
    int Source = (ui->lineEdit_Source->text()).toInt();
    int Dest = (ui->lineEdit_Dest->text()).toInt();

    Node *source = graph->findNode(Source);
    Node *dest = graph->findNode(Dest);

    if (source && dest)
    {
        foreach (Edge *i, source->edges())
            if ((i->destNode()->getIndex() == Dest) || ((!i->getOriented()) && (i->sourceNode()->getIndex() == Dest)))
            {
                graph->deleteEdge(i);
                break;
            }
    }

    ui->lineEdit_Source->clear();
    ui->lineEdit_Dest->clear();
    update();
}

void MainWindow::on_pb_RemoveNode_clicked()  //Удалить вершины
{
   graph->deleteSelectedNodes();
   graph->changeIndecesOfAllVerteces();
}

void MainWindow::on_pb_CreateNode_clicked()//создаем вершину
{
    graph->addNode();
}

void MainWindow::on_pb_CreateEdge_clicked()
{
    int Source1 = (ui->lineEdit_Source->text()).toInt();
    int Dest1 = (ui->lineEdit_Dest->text()).toInt();

    Node *source = graph ->findNode(Source1);
    Node *dest = graph->findNode(Dest1);

    if (dest && source)
        graph->addEdge(source,dest);
    ui->lineEdit_Source->clear();
    ui->lineEdit_Dest->clear();
}

void MainWindow::on_pb_ClearAll_clicked()
{
    graph->deleteAllItems();
    ui->textBrowser->setText("Двойной клик левой кнопкой мыши - добавить новую вершину. \n\n"
                             "Двойной клик правой кнопкой мыши - соединить выбранные вершины с той, на которую кликал.\n");
}

void MainWindow::on_pb_Help_clicked()
{
    HelpBrowser helpBrowser(":/help/", "main.htm");
    helpBrowser.resize(980, 720);
    helpBrowser.exec();
}

void MainWindow::on_action_Open_file_triggered()
{
    on_pb_OpenButton_clicked();
}

void MainWindow::on_action_Save_File_triggered()
{
    on_pb_SaveButton_clicked();
}

void MainWindow::on_action_createNode_triggered()
{
    on_pb_CreateNode_clicked();
}

void MainWindow::on_action_createEdge_triggered()
{
    QMessageBox msgBox;
    QComboBox *source = new QComboBox();
    QComboBox *dest = new QComboBox();
    QSpinBox *length = new QSpinBox();
    QComboBox *type = new QComboBox();
    length->setRange(1, 1000);

    for(int i=0;i<graph->getListOfNodeSize();i++ )
    {
        source->addItem(QString::number(i));
        dest->addItem(QString::number(i));
    }
    QLabel *text1 = new QLabel();
    text1->setText(("Первая вершина:"));
    QLabel *text2 = new QLabel();
    text2->setText(("Вторая вершина:"));
    QLabel *text3 = new QLabel();
    text3->setText(("Вес ребра:"));
    QLabel *text4 = new QLabel();
    text4->setText(("Тип ребра:"));

    type->addItem(QString("Ориентированное"));
    type->addItem(QString("Неориентированное"));

    msgBox.layout()->addWidget(text1);
    msgBox.layout()->addWidget(source);
    msgBox.layout()->addWidget(text2);
    msgBox.layout()->addWidget(dest);
    msgBox.layout()->addWidget(text3);
    msgBox.layout()->addWidget(length);
    msgBox.layout()->addWidget(text4);
    msgBox.layout()->addWidget(type);

    QAbstractButton *okButton = msgBox.addButton(tr("Принять"), QMessageBox::AcceptRole);
    QAbstractButton *canleButton = msgBox.addButton("Отмена",QMessageBox::RejectRole);


    msgBox.setWindowIcon(QIcon(":/icons/icon.png"));
    msgBox.setText("Параметры добавляемого ребра");
    msgBox.setWindowTitle("Параметры ребра");
    msgBox.exec();

    int srcNum = (source->currentText()).toInt();
    int dstNum = (dest->currentText()).toInt();
    int lengthNum = length->value();
    bool flag;

    Node *src = graph->findNode(srcNum);
    Node *dst = graph->findNode(dstNum);

    if (type->currentText() == QString("Ориентированное"))
        flag = true;
    else
        flag = false;

    if (msgBox.clickedButton() == okButton)
        graph->addEdge(src,dst,lengthNum,flag);

    update();
}

void MainWindow::on_action_removeEdge_triggered()
{
    QMessageBox msgBox;
    QComboBox *edge= new QComboBox();

    // Перечисляем все ребра графа
    foreach(Edges *edges, graph->getEdges())
        edge->addItem(QString::number(edges->edge->sourceNode()->getIndex())+" - "+QString::number(edges->edge->destNode()->getIndex()));

    QLabel *text = new QLabel();
    text->setText(("Ребро:"));

    msgBox.layout()->addWidget(text);
    msgBox.layout()->addWidget(edge);

    QAbstractButton *okButton = msgBox.addButton(tr("Удалить"), QMessageBox::AcceptRole);
    QAbstractButton *canleButton = msgBox.addButton("Отмена",QMessageBox::RejectRole);

    msgBox.setWindowIcon(QIcon(":/icons/icon.png"));
    msgBox.setText("Выберите, какое ребро удалить");
    msgBox.setWindowTitle("Удаление ребра");
    msgBox.exec();

    if (msgBox.clickedButton() == okButton)
    {
        QStringList list = edge->currentText().split(QRegExp(" "),QString::SkipEmptyParts);// Разбиваем строку на символы для получения индексов вершин
        int Source = list.at(0).toInt();
        int Dest = list.at(2).toInt();

        Node *source = graph ->findNode(Source);

        // Ищем нужное ребро и удаляем его
        foreach (Edge *i, source->edges())
            if ((i->destNode()->getIndex() == Dest) || ((!i->getOriented()) && (i->sourceNode()->getIndex() == Dest)))
            {
                graph->deleteEdge(i);
                break;
            }
    }
    update();
}

void MainWindow::on_action_removeEdges_triggered()
{
    foreach (Node* Node, graph->getNodes())
        foreach (Edge *edge, Node->edges())
        {
            graph->scene->removeItem(edge); //Убрать ребро из окна
            edge->destNode()->removeEdge(edge); //Убрать ребро из списка рёбер первой вершины
            edge->sourceNode()->removeEdge(edge); //Убрать ребро из списка рёбер второй вершины
            delete edge;
        }
}

void MainWindow::on_action_clear_all_triggered()
{
    on_pb_ClearAll_clicked();
}

void MainWindow::on_action_triggered()
{
    on_pb_Help_clicked();
}

void MainWindow::on_action_About_triggered()
{
     QMessageBox about;
     QLabel *icon = new QLabel;
     QLabel *text = new QLabel;
     text->setText("Программа построения графов\n\nАвтор программы: Малков Дмитрий, группа: ИУ6-51Б\n\n2021");
     icon->setPixmap(QPixmap(":/icons/icon.png"));
     about.layout()->addWidget(icon);
     about.layout()->addWidget(text);
     about.exec();
}

void MainWindow::on_action_adjacencyMatrix_triggered()
{
    QVector<QVector <int>> matrix = graph->getAdjacencyMatrix();

    tablewindow->close();// Закрываем окно представлений
    tablewindow->clear();// Очищаем таблицу
    tablewindow->setCount(matrix.size(),matrix.size());// Устаналвиваем размер таблицы

    for (int i=0;i<matrix.size();i++)
        for (int j=0;j<matrix.size(); j++)
            if (matrix[i][j]!=0)
                tablewindow->setNumber(i,j,matrix[i][j]);

    tablewindow->setflag(false);// Установка флага для удаления дополнительных элементов
    tablewindow->show();// Отображаем окно с таблицей
    setEnabled(false);// Блокируем окно
}
void MainWindow::on_action_incidenceMatrix_triggered()
{
    tablewindow->close();// Закрываем окно представлений
    tablewindow->clear();// Очищаем таблицу
    tablewindow->setCount(graph->getListOfNodeSize(),graph->getEdges().size());// Устаналвиваем размер таблицы

    int ind = 0;
    // Добавляем индекс ребер
    foreach (Edges *edge, graph->getEdges())
    {
              QGraphicsTextItem *textItem = new  QGraphicsTextItem(QString::number(ind));// Создаем индекс ребра
              int x = (edge->edge->sourcePoint.x() + edge->edge->destPoint.x())/2;// Находим координату X для расположения индекса ребра
              int y = (edge->edge->sourcePoint.y() + edge->edge->destPoint.y())/2;// Находим координату У для расположения индекса ребра
              textItem->setFont(QFont("Arial",10,QFont::Bold));// Настраиваем стиль индекса ребра
              textItem->setZValue(1000);// Ставим индекс выше других объектов на сцене
              textItem->setPos(x,y);// Уставливаем коордианты индекса
              textItem->setDefaultTextColor(Qt::red);// Уставливаем цвет индекса
              graph->scene->addItem(textItem);// Добавляем индекс на сцену
              ind++;
    }

    QVector<QVector<int>> matrix = graph->getIncidenceMatrix();
    for (int i=0;i<matrix.size();i++)
        for (int j=0;j<matrix[1].size(); j++)
            if (matrix[i][j]!=0)
                tablewindow->setNumber(i,j,matrix[i][j]);


    tablewindow->setflag(true);// Установка флага для удаления дополнительных элементов
    tablewindow->show();// Отображаем окно с таблицей
    setEnabled(false);// Блокируем окно
}
void MainWindow::on_action_listOfAyliffe_triggered()
{
    QString line = "Cписок Айлиффа \n\n";
    QList<QList<int>> list = graph->getlistOfAyliffe();
    for (int i=0; i<list.size();i++)
    {
        line += QString::number(i)+": ";
        for (int j=0;j<list[i].size();j++)
            line += QString::number(list[i][j]) + ", ";

        line.chop(2);
        line += "\n";
    }
    ui->textBrowser->setText(line);
}

void MainWindow::on_action_Algoritm_triggered()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    tablewindow->close();// Закрываем окно представлений
}

