#include "CustomNavigationWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QFontMetrics>

#include <QDebug>

CustomNavigationWidget::CustomNavigationWidget(QWidget *parent) : QWidget(parent)
{
    initWidget();
}

void CustomNavigationWidget::setText(const QString &text)
{
    _text = text;

    update();
}

void CustomNavigationWidget::setLeftButtonFlag(bool flag)
{
    _leftEnableFlag = flag;
    update();
}

void CustomNavigationWidget::setRightButtonFlag(bool flag)
{
    _rightEnableFlag = flag;
    update();
}

void CustomNavigationWidget::setFont(const QFont &font)
{
    _font = font;

    update();
}

void CustomNavigationWidget::setTextColor(const QColor &color)
{
    _textColor = color;

    update();
}

void CustomNavigationWidget::setEnableColor(const QColor &color)
{
    _enableColor = color;
    update();
}

void CustomNavigationWidget::setEnableArrowColor(const QColor &color)
{
    _enableArrowColor = color;
    update();
}

void CustomNavigationWidget::setDisableColor(const QColor &color)
{
    _disableColor = color;
    update();
}

bool CustomNavigationWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this && event->type() == QEvent::Leave)
        handleHoverEvent(QPoint(-1,-1));

    return QWidget::eventFilter(watched,event);
}

void CustomNavigationWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        handleLeftButtonPressedEvent(event->pos());

    return QWidget::mousePressEvent(event);
}

void CustomNavigationWidget::mouseMoveEvent(QMouseEvent *event)
{
    handleHoverEvent(event->pos());

    return QWidget::mouseMoveEvent(event);
}

void CustomNavigationWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        handleLeftButtonClickedEvent(event->pos());

    return QWidget::mouseReleaseEvent(event);
}

void CustomNavigationWidget::handleHoverEvent(const QPoint & pos)
{
    if(_leftRect.contains(pos))
        _horverLeftRectFlag = true;
    else if(_rightRect.contains(pos))
        _horverRightRectFlag = true;
    else
    {
        _horverLeftRectFlag = false;
        _horverRightRectFlag = false;
    }

    update();
}

void CustomNavigationWidget::handleLeftButtonPressedEvent(const QPoint & pos)
{
    if(_leftRect.contains(pos))
        _pressedLeftRectFlag = true;
    else if(_rightRect.contains(pos))
        _pressedRightRectFlag = true;

    update();
}
void CustomNavigationWidget::handleLeftButtonClickedEvent(const QPoint & pos)
{
    _pressedLeftRectFlag = false;
    _pressedRightRectFlag = false;

    if(_leftRect.contains(pos) && _leftEnableFlag)
        emit(clickedLeftButton());
    else if(_rightRect.contains(pos) && _rightEnableFlag)
        emit(clickedRightButton());

    update();
}

void CustomNavigationWidget::initWidget()
{
    installEventFilter(this);
    setMouseTracking(true);

    _font = font();
    _font.setPointSize(12);
}

void CustomNavigationWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(_font);
    painter.setPen(Qt::transparent);

    QRect Rect = rect();
    int Rect_Height = Rect.height();


    QRect textRect(0,0,painter.fontMetrics().width(_text),painter.fontMetrics().height());
    QRect drawTextRect = QRect((Rect.width()-textRect.width())/2,(Rect.height()-textRect.height())/2,
                               textRect.width(),textRect.height());
    QPen Pen = painter.pen();
    Pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    Pen.setJoinStyle(Qt::PenJoinStyle::RoundJoin);
    Pen.setColor(_textColor);
    Pen.setWidth(2);

    painter.setPen(Pen);
    painter.drawText(drawTextRect,_text);

    ///draw left
    _leftRect = QRect(0,0,Rect_Height,Rect_Height);
    QRect leftArrowRect = _leftRect.adjusted(_leftRect.height()/3,_leftRect.height()/4,
                                             -_leftRect.height()/3,-_leftRect.height()/4);
    QColor tempColor = _enableColor;
    QColor tempArrowColor = _enableArrowColor;

    /// draw left rect
    if(!_leftEnableFlag)
    {
        tempColor = _disableColor;
        tempArrowColor = _disableColor.light();
    }else if(_pressedLeftRectFlag)
    {
        tempColor = tempColor.dark(120);
        tempArrowColor = tempArrowColor.dark(120);
    }else if(_horverLeftRectFlag)
    {
        tempColor = tempColor.light();
        tempArrowColor = tempArrowColor.light();
    }

    painter.setPen(Qt::transparent);
    painter.setBrush(tempColor);
    painter.drawRoundedRect(_leftRect,3,3);
    /// draw left arrow
    Pen.setColor(tempArrowColor);
    painter.setPen(Pen);
    painter.drawLine(leftArrowRect.topRight(),QPoint(leftArrowRect.x(),leftArrowRect.center().y()));
    painter.drawLine(QPoint(leftArrowRect.x(),leftArrowRect.center().y()),leftArrowRect.bottomRight());

    /// reset color
    tempColor = _enableColor;
    tempArrowColor = _enableArrowColor;

    /// draw right
    _rightRect = QRect(Rect.width()-Rect_Height,0,Rect_Height,Rect_Height);
    QRect rightArrowRect = _rightRect.adjusted(_rightRect.height()/3,_rightRect.height()/4,
                                              -_rightRect.height()/3,-_rightRect.height()/4);

    /// draw right rect
    if(!_rightEnableFlag)
    {
        tempColor = _disableColor;
        tempArrowColor = _disableColor.light();
    }else if(_pressedRightRectFlag)
    {
        tempColor = tempColor.dark(120);
        tempArrowColor = tempArrowColor.dark(120);
    }else if(_horverRightRectFlag)
    {
        tempColor = tempColor.light();
        tempArrowColor = tempArrowColor.light();
    }

    painter.setPen(Qt::transparent);
    painter.setBrush(tempColor);
    painter.drawRoundedRect(_rightRect,3,3);
    /// draw right arrow
    Pen.setColor(tempArrowColor);
    painter.setPen(Pen);
    painter.drawLine(rightArrowRect.topLeft(),QPoint(rightArrowRect.right(),rightArrowRect.center().y()));
    painter.drawLine(QPoint(rightArrowRect.right(),rightArrowRect.center().y()),rightArrowRect.bottomLeft());

    return QWidget::paintEvent(event);
}
