#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QList>
#include <QVector>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_mouseClicked(QPoint point);

    void on_timeOut();

    void on_actionStart_triggered();

    void on_btnStart_clicked();

    void on_actionPause_triggered();

    void on_btnPause_clicked();

    void on_actionContinue_triggered();

    void on_btnContinue_clicked();

    void on_actionRestart_triggered();

    void on_btnRestart_clicked();

    void on_actionSave_triggered();

    void on_btnSave_clicked();

    void on_actionLoad_triggered();

    void on_btnLoad_clicked();

private:
    void iniGraphicsSystem();
    void addTop();
    void addDown();
    void addLeft();
    void addRight();
    void deleteLast();
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void stateStart();
    void stateRun();
    void statePause();
    void stateOver();
    void SnakeStrike();
    bool Save(QString aFileName);
    bool Load(QString aFileName);
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    const int mapNodeWidth=20;
    const int mapNodeHeight=20;
    QTimer *timer;
    int time=100;
    //地图信息
    QVector<QGraphicsRectItem*> mapNode;
    QVector<QGraphicsRectItem*> Barrier;
    QGraphicsRectItem* reward;//奖励\食物
    static const int rectType=1;
    enum type{null,body,barrier,food};//地图格子类型
    //游戏信息
    QList<QPoint> snake;//贪吃蛇本体
    enum move{Left,Right,Up,Down};
    int moveFlag = Up;
    enum state{start,run,pause,over}; //游戏的状态
    int gameState = start;
    int score=0;
};

#endif // MAINWINDOW_H
