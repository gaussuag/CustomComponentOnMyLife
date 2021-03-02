#include "CustomProxyStyle.h"

#include <QPainter>
#include <QStyleOption>

#include "CIconManage.h"

drawArrowIndicator::drawArrowIndicator(Qt::ArrowType arrowType, QStyle *style)
    :QProxyStyle(style),_arrowType(arrowType)
{

}

void drawArrowIndicator::drawPrimitive (PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget) const{

    if (element == QStyle::PE_IndicatorArrowDown && !option->rect.isNull()){

        if(widget)
        {
            QPixmap drawArrowPixmap;
            auto color = painter->pen().color();
            if(drawArrowPixmap.isNull())
            {
                switch (_arrowType) {
                case Qt::DownArrow:
                    drawArrowPixmap = DefaultIcon->icon_Svg_angle_down.getIconPixmap(QSize(10,16),color);
                    break;
                case Qt::UpArrow:
                    drawArrowPixmap = DefaultIcon->icon_Svg_angle_up.getIconPixmap(QSize(10,16),color);
                    break;
                default:
                    drawArrowPixmap = DefaultIcon->icon_Svg_angle_down.getIconPixmap(QSize(10,16),color);
                    break;
                }
            }
            QSize pixmapSize = drawArrowPixmap.size();
            QRect rect = widget->rect();
            QRect drawRect = QRect(rect.width() - pixmapSize.width()*2,
                                   (rect.height() - pixmapSize.height())/2,
                                   pixmapSize.width(),pixmapSize.height());
            painter->drawPixmap(drawRect,drawArrowPixmap);
            return;
        }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
