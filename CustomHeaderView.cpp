#include "CustomHeaderView.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

CustomHeaderView::CustomHeaderView(Qt::Orientation orientation, QWidget *parent):QHeaderView(orientation,parent)
{
}

CustomHeaderView::~CustomHeaderView()
{

}

void CustomHeaderView::updateGeometries()
{
    _checkBoxGeometry = QRect(10,(height() - 15)/2,15,15);
}

void CustomHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();
    if (logicalIndex == 0)
    {
        QStyleOptionButton option;
        option.rect = _checkBoxGeometry;
        if (_checkBoxCheckedFlag)
            option.state = QStyle::State_On;
        else
            option.state = QStyle::State_Off;
        this->style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter);
    }
}
void CustomHeaderView::mousePressEvent(QMouseEvent *event)
{
    if(_checkBoxGeometry.contains(event->pos()))
    {
        if (_checkBoxCheckedFlag)
            _checkBoxCheckedFlag = false;
        else
            _checkBoxCheckedFlag = true;

        emit(clickedCheckBox(_checkBoxCheckedFlag));
        this->update();
    }
    QHeaderView::mousePressEvent(event);
}
void CustomHeaderView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(_checkBoxGeometry.contains(event->pos()))
    {
        if (_checkBoxCheckedFlag)
            _checkBoxCheckedFlag = false;
        else
            _checkBoxCheckedFlag = true;

        emit(clickedCheckBox(_checkBoxCheckedFlag));
        this->update();
    }
    QHeaderView::mouseDoubleClickEvent(event);

}
