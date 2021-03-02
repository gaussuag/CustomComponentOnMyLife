#include "CustomLine.h"
#include <QPainter>
#include <QPen>
#include <QDebug>

CustomLine::CustomLine(QWidget *parent, int LineWidth, Qt::Orientation Orientation, QColor Color, Qt::PenStyle PenStyle)
    :QWidget(parent),_lineWidth(LineWidth),_lineColor(QColor(Color)),_penStyle(PenStyle)
{
    setLineOrientation(Orientation);
}

CustomLine::~CustomLine()
{

}

void CustomLine::setLineOrientation(Qt::Orientation Orientation)
{
    _orientation = Orientation;
    ///update fixed size
    setLineWidth(_lineWidth);
}

void CustomLine::setDashStepLenth(qreal StepLenth, qreal StepSpaceLenth)
{
    _dashStepLenth = StepLenth;
    _dashStepSpaceLenth = StepSpaceLenth;
    update();
}

void CustomLine::setLineColor(const QString &Color)
{
    _lineColor = QColor(Color);
    update();
}

void CustomLine::setLineWidth(int w)
{
    _lineWidth = w;
    if(_orientation == Qt::Vertical)
        setFixedWidth(w);
    else if(_orientation == Qt::Horizontal)
        setFixedHeight(w);
}

void CustomLine::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setStyle(_penStyle);
    pen.setCapStyle(Qt::RoundCap);
    pen.setBrush(_lineColor);
    pen.setColor(_lineColor);
    pen.setWidth(0);

    if(_penStyle == Qt::PenStyle::CustomDashLine)
        pen.setDashPattern({_dashStepLenth,_dashStepSpaceLenth});

    painter.setPen(pen);


    auto currentRect = rect();
    QPointF s,e;
    if(_orientation == Qt::Orientation::Vertical)
    {
        qreal offset = currentRect.width()/2.0;
        s = currentRect.topLeft() + QPointF(offset,0);
        e = currentRect.bottomLeft() + QPointF(offset,0);
    }
    else if(_orientation == Qt::Orientation::Horizontal)
    {
        qreal offset = currentRect.height()/2;
        s = currentRect.topLeft() + QPointF(0,offset);
        e = currentRect.topRight() + QPointF(0,offset);
    }


    painter.drawLine(s,e);

    return ;
}
