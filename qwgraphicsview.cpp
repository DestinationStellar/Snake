 #include "qwgraphicsview.h"

QWGraphicsView::QWGraphicsView(QWidget *parent):QGraphicsView(parent)
{

}

void QWGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        QPoint point=event->pos();
        emit mouseClicked(point);
    }
    QGraphicsView::mousePressEvent(event);
}
