#include "CustomMaskWidget.h"

#include <QPainter>

CustomMaskLabel::CustomMaskLabel(QWidget *parent) : QLabel(parent)
{
    initWidget();
}

CustomMaskLabel::~CustomMaskLabel()
{

}

void CustomMaskLabel::setIcon(const QPixmap &pixmap, Qt::WindowFrameSection IconPosition)
{
    _drawIconFlag = true;
    _iconPixmap = pixmap;
    _iconPostion = IconPosition;

    update();
}

void CustomMaskLabel::initWidget()
{
    setAutoFillBackground(true);
    auto Palette = palette();
    Palette.setBrush(QPalette::Background,_backgroundColor);
    setPalette(Palette);
}

void CustomMaskLabel::paintEvent(QPaintEvent * event)
{
    QLabel::paintEvent(event);

    if(_drawIconFlag)
        drawIconPixmap();
}

void CustomMaskLabel::drawIconPixmap()
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

    /// 根据位置，将Icon Pixmap 绘制
    QRect Rect = rect();

    QPoint Pos;
    if(_iconPostion == Qt::TopLeftSection)
    {
        auto topLeft = Rect.topLeft();
        topLeft.setX(topLeft.x()+_iconPixmapMargin);
        topLeft.setY(topLeft.y()+_iconPixmapMargin);
        Pos = topLeft;
    }
    painter.drawPixmap(Pos,_iconPixmap);

    painter.restore();
}
