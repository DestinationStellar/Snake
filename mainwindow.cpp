#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelOver->hide();
    ui->labelOver->raise();
    scene=new QGraphicsScene(-400,-400,800,800);
    ui->graphicsView->setScene(scene);
    for(int i=0;i<40;i++)
        for(int j=0;j<40;j++)
        {
            QGraphicsRectItem *item=new QGraphicsRectItem(i*20-400,j*20-400,mapNodeWidth,mapNodeHeight);
            item->setPen(QColor(255,255,255,50));
            item->setData(rectType,null);
            scene->addItem(item);
            mapNode.push_back(item);
        }
    iniGraphicsSystem();
    stateStart();
    connect(ui->graphicsView,SIGNAL(mouseClicked(QPoint)),this,SLOT(on_mouseClicked(QPoint)));
    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(on_timeOut()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//绘图模块
void MainWindow::paintEvent(QPaintEvent *event)
{
    if(gameState == over){
        if(snake.length()>=1600)QMessageBox::information(this,"成功通关","恭喜你，通关了！");
        else ui->labelOver->show();
        timer->stop();
        stateOver();
        //return;
    }
    for(int i=0;i<mapNode.length();i++)
    {
        mapNode.at(i)->setData(rectType,null);
    }

    reward->setData(rectType,food);

    QPointF pointScene;//可能需要放在循环内
    QGraphicsItem *item=nullptr;
    QGraphicsRectItem *theItem=nullptr;
    for(int i=0;i<snake.length();i++){
        pointScene=ui->graphicsView->mapToScene(snake.at(i));
        item=scene->itemAt(pointScene,ui->graphicsView->transform());
        if(item == nullptr)return;
        theItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);
        theItem->setData(rectType,body);
    }

    for(int i=0;i<Barrier.length();i++)
    {
        Barrier.at(i)->setData(rectType,barrier);
    }

    for(int i=0;i<mapNode.length();i++)
    {
        if(mapNode.at(i)->data(rectType)==barrier)mapNode.at(i)->setBrush(QColor("lightsteelblue"));
        else if(mapNode.at(i)->data(rectType)==body)mapNode.at(i)->setBrush(Qt::darkGreen);
        else if(mapNode.at(i)->data(rectType)==food)mapNode.at(i)->setBrush(Qt::red);
        else if(mapNode.at(i)->data(rectType)==null)mapNode.at(i)->setBrush(QColor("lightyellow"));
    }
    ui->labelScore->setNum(score);
    QMainWindow::paintEvent(event);
}

void MainWindow::iniGraphicsSystem()//初始化
{
    snake.clear();
    Barrier.clear();
    ui->labelOver->hide();
    reward = nullptr;
    moveFlag=Up;
    snake.append(QPoint(410,410));
    addTop();
    reward = mapNode.at(105);//先生成一个奖励节点
    score=0;
    update();
}
//游戏模块
void MainWindow::on_timeOut()
{
    int flag=1;
    bool p=false;
    score++;
    QPointF pointScene=ui->graphicsView->mapToScene(snake.at(0));//可能需要放在循环内
    QGraphicsItem *item=scene->itemAt(pointScene,ui->graphicsView->transform());
    QGraphicsRectItem *theItem=qgraphicsitem_cast<QGraphicsRectItem*>(item);
    if(theItem==reward){
        flag+=3;p=true;
    }
    while (flag--) {
        switch (moveFlag) {
        case Up:
            addTop();break;
        case Down:
            addDown();break;
        case Left:
            addLeft();break;
        case Right:
            addRight();break;
        default:
            break;
        }
    }
    repaint();
    if(p)//这样处理是为了防止随机果实恰好出现在新添加蛇头的位置上
    {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int rand=qrand()%1600;
        while (true) {
            if(mapNode.at(rand)->data(rectType)==null){
                reward=mapNode.at(rand);
                break;
            }
            rand=qrand()%1600;
        }
    }
    SnakeStrike();
    if(gameState!=over)deleteLast();
    update();
}

void MainWindow::addTop()
{
    if(snake.at(0).y()-mapNodeHeight<0)
        //snake.insert(0,QPoint(snake.at(0).x(),ui->graphicsView->height()-(mapNodeHeight/2)));
        gameState=over;
    else
        snake.insert(0,QPoint(snake.at(0).x(),snake.at(0).y()-mapNodeHeight));
}

void MainWindow::addDown()
{
    if(snake.at(0).y()+mapNodeHeight>ui->graphicsView->height())
        //snake.insert(0,QPoint(snake.at(0).x(),mapNodeHeight/2));
        gameState=over;
    else
        snake.insert(0,QPoint(snake.at(0).x(),snake.at(0).y()+mapNodeHeight));
}

void MainWindow::addLeft()
{
    if(snake.at(0).x()-mapNodeWidth<0)
        //snake.insert(0,QPoint(ui->graphicsView->width()-(mapNodeWidth/2),snake.at(0).y()));
        gameState=over;
    else
        snake.insert(0,QPoint(snake.at(0).x()-mapNodeWidth,snake.at(0).y()));
}

void MainWindow::addRight()
{
    if(snake.at(0).x()+mapNodeWidth>scene->width())
        //snake.insert(0,QPoint(mapNodeWidth/2,snake.at(0).y()));
        gameState=over;
    else
        snake.insert(0,QPoint(snake.at(0).x()+mapNodeWidth,snake.at(0).y()));
}

void MainWindow::deleteLast()
{
    snake.removeLast();
}

void MainWindow::SnakeStrike()
{
   for(int i=0;i<snake.length();i++)
   {
       for(int j=i+1;j<snake.length();j++){
           if(snake.at(i)==snake.at(j)){
               gameState = over;break;
           }
       }
   }
   QPointF pointScene=ui->graphicsView->mapToScene(snake.at(0));
   QGraphicsItem *item=scene->itemAt(pointScene,ui->graphicsView->transform());
   QGraphicsRectItem *theItem=qgraphicsitem_cast<QGraphicsRectItem*>(item);
   for(int i=0;i<Barrier.length();i++)
   {
       if(theItem==Barrier.at(i))gameState=over;
   }
}
//键盘鼠标事件及响应模块
void MainWindow::on_mouseClicked(QPoint point)
{
    if(gameState != start)return;
    QPointF pointScene=ui->graphicsView->mapToScene(point);
    QGraphicsItem *item=nullptr;
    item=scene->itemAt(pointScene,ui->graphicsView->transform());
    if(item == nullptr)
        return;
    QGraphicsRectItem *theItem;
    theItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);

    if(theItem->data(rectType)== null){
        theItem->setBrush(QColor("lightsteelblue"));
        theItem->setData(rectType,barrier);
        Barrier.push_back(theItem);
    }
    else if(theItem->data(rectType)== barrier){
        theItem->setBrush(QColor("lightyellow"));
        theItem->setData(rectType,null);
        for(int i=0;i<Barrier.length();i++)
        {
            if(Barrier.at(i)==theItem)Barrier.remove(i);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        if(moveFlag != Down){
            moveFlag = Up;
        }
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        if(moveFlag != Up){
            moveFlag = Down;
        }
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        if(moveFlag != Right){
            moveFlag = Left;
        }
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        if(moveFlag != Left){
            moveFlag = Right;
        }
        break;
    case Qt::Key_Space:
        if(gameState == start||gameState == pause){
            timer->start(time);
            gameState = run;
            stateRun();
        }
        else if(gameState == run){
            timer->stop();
            gameState = pause;
            statePause();
        }
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if(gameState==over||gameState==pause){
            iniGraphicsSystem();
            gameState=start;
            stateStart();
        }
        break;
    default:
        break;
    }
}
//图形界面Action及按钮模块
void MainWindow::stateStart()
{
    ui->actionStart->setEnabled(true);
    ui->btnStart->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->btnPause->setEnabled(false);
    ui->actionLoad->setEnabled(true);
    ui->btnLoad->setEnabled(true);
    ui->actionSave->setEnabled(false);
    ui->btnSave->setEnabled(false);
    ui->actionRestart->setEnabled(false);
    ui->btnRestart->setEnabled(false);
    ui->actionContinue->setEnabled(false);
    ui->btnContinue->setEnabled(false);
}
void MainWindow::stateRun()
{
    ui->actionStart->setEnabled(false);
    ui->btnStart->setEnabled(false);
    ui->actionPause->setEnabled(true);
    ui->btnPause->setEnabled(true);
    ui->actionLoad->setEnabled(false);
    ui->btnLoad->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->btnSave->setEnabled(false);
    ui->actionRestart->setEnabled(false);
    ui->btnRestart->setEnabled(false);
    ui->actionContinue->setEnabled(false);
    ui->btnContinue->setEnabled(false);
}
void MainWindow::statePause()
{
    ui->actionStart->setEnabled(false);
    ui->btnStart->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->btnPause->setEnabled(false);
    ui->actionLoad->setEnabled(false);
    ui->btnLoad->setEnabled(false);
    ui->actionSave->setEnabled(true);
    ui->btnSave->setEnabled(true);
    ui->actionRestart->setEnabled(true);
    ui->btnRestart->setEnabled(true);
    ui->actionContinue->setEnabled(true);
    ui->btnContinue->setEnabled(true);
}
void MainWindow::stateOver()
{
    ui->actionStart->setEnabled(false);
    ui->btnStart->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->btnPause->setEnabled(false);
    ui->actionLoad->setEnabled(false);
    ui->btnLoad->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->btnSave->setEnabled(false);
    ui->actionRestart->setEnabled(true);
    ui->btnRestart->setEnabled(true);
    ui->actionContinue->setEnabled(false);
    ui->btnContinue->setEnabled(false);
}

void MainWindow::on_actionStart_triggered()
{
    if(gameState==start){
        timer->start(time);
        gameState=run;
        stateRun();
    }
}

void MainWindow::on_btnStart_clicked()
{
    if(gameState==start){
        timer->start(time);
        gameState=run;
        stateRun();
    }
}

void MainWindow::on_actionPause_triggered()
{
    if(gameState==run){
        timer->stop();
        gameState=pause;
        statePause();
    }
}

void MainWindow::on_btnPause_clicked()
{
    if(gameState==run){
        timer->stop();
        gameState=pause;
        statePause();
    }
}

void MainWindow::on_actionContinue_triggered()
{
    if(gameState==pause){
        timer->start(time);
        gameState=run;
        stateRun();
    }
}

void MainWindow::on_btnContinue_clicked()
{
    if(gameState==pause){
        timer->start(time);
        gameState=run;
        stateRun();
    }
}

void MainWindow::on_actionRestart_triggered()
{
    if(gameState==pause||gameState==over){
        iniGraphicsSystem();
        gameState=start;
        stateStart();
    }
}

void MainWindow::on_btnRestart_clicked()
{
    if(gameState==pause||gameState==over){
        iniGraphicsSystem();
        gameState=start;
        stateStart();
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString curPath=QDir::currentPath();
    QString aFileName=QFileDialog::getSaveFileName(this,"选择保存文件",curPath,"贪吃蛇存档(*.snake)");
    if(aFileName.isEmpty())
        return;
    if(Save(aFileName))
        QMessageBox::information(this,"提示消息","文件已成功保存！");
}

void MainWindow::on_btnSave_clicked()
{
    QString curPath=QDir::currentPath();
    QString aFileName=QFileDialog::getSaveFileName(this,"选择保存文件",curPath,"贪吃蛇存档(*.snake)");
    if(aFileName.isEmpty())
        return;
    if(Save(aFileName))
        QMessageBox::information(this,"提示消息","文件已成功保存！");
}

void MainWindow::on_actionLoad_triggered()
{
    QString curPath=QDir::currentPath();
    QString aFileName=QFileDialog::getOpenFileName(this,"打开一个文件",curPath,"贪吃蛇存档(*.snake)");
    if(aFileName.isEmpty())
        return;
    if(Load(aFileName)){
        QMessageBox::information(this,"提示消息","文件已经打开！");
    }
}

void MainWindow::on_btnLoad_clicked()
{
    QString curPath=QDir::currentPath();
    QString aFileName=QFileDialog::getOpenFileName(this,"打开一个文件",curPath,"贪吃蛇存档(*.snake)");
    if(aFileName.isEmpty())
        return;
    if(Load(aFileName)){
        QMessageBox::information(this,"提示消息","文件已经打开！");
    }
}

bool MainWindow::Save(QString aFileName)
{
    QFile aFile(aFileName);
    if(!(aFile.open(QIODevice::WriteOnly | QIODevice::Truncate)))
        return  false;
    QDataStream aStream(&aFile);
    aStream.setVersion(QDataStream::Qt_5_13);
    aStream<<score;
    aStream<<gameState;
    aStream<<moveFlag;
    aStream<<snake;
    aStream<<Barrier.length();
    for (int i=0;i<Barrier.length();i++) {
        for (int j=0;j<mapNode.length();j++) {
            if(Barrier.at(i)==mapNode.at(j)){
                aStream<<j;break;
            }
        }
    }
    for(int i = 0;i<mapNode.length();i++)
    {
        if(reward==mapNode.at(i))
        {
            aStream<<i;break;
        }
    }
    aFile.close();
    return true;
}

bool MainWindow::Load(QString aFileName)
{
    QFile aFile(aFileName);
    if(!(aFile.open(QIODevice::ReadOnly)))
        return false;
    QDataStream aStream(&aFile);
    aStream.setVersion(QDataStream::Qt_5_13);
    aStream>>score;
    aStream>>gameState;
    aStream>>moveFlag;
    Barrier.clear();
    snake.clear();
    aStream>>snake;
    int barrierLength;
    int barrierId;
    aStream>>barrierLength;
    for(int j=0;j<barrierLength;j++){
        aStream>>barrierId;
        Barrier.push_back(mapNode.at(barrierId));
    }
    reward=nullptr;
    int i;
    aStream>>i;
    reward=mapNode.at(i);
    update();
    aFile.close();
    return true;
}
