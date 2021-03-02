#ifndef CUSTOMPROXYSTYLE_H
#define CUSTOMPROXYSTYLE_H

#include <QProxyStyle>

class QWidget;
class QPainter;
class QStyleOption;

class drawArrowIndicator: public QProxyStyle{
public:
    drawArrowIndicator(Qt::ArrowType arrowType = Qt::ArrowType::DownArrow,QStyle* style = nullptr);

    void drawPrimitive ( PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = nullptr) const;

private:
    Qt::ArrowType _arrowType{Qt::DownArrow};
};




#endif // CUSTOMPROXYSTYLE_H
