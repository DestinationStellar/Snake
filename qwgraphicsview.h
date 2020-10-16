#ifndef QWGRAPHICSVIEW_H
#define QWGRAPHICSVIEW_H
#include <QGraphicsView>
#include <QMouseEvent>

class QWGraphicsView : public QGraphicsView
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *);
public:
    QWGraphicsView(QWidget *parent = nullptr);
signals:
    void mouseClicked(QPoint point);
};

#endif // QWGRAPHICSVIEW_H
