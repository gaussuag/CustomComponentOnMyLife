#include "CustomProgressBar.h"

CustomProgressBar::CustomProgressBar(QWidget *parent) : QProgressBar(parent)
{
    QProgressBar::setRange(0,_presetMaximum);
}

CustomProgressBar::~CustomProgressBar()
{

}

void CustomProgressBar::setValue(qint64 value)
{
    _accumulate += value;
    auto percent = _accumulate * QProgressBar::maximum() * 1.00 / _maximum;
    QProgressBar::setValue(percent);
}

void CustomProgressBar::setValue(int value)
{
    return setValue(qint64(value));
}

void CustomProgressBar::setRange(qint64 minimum, qint64 maximum)
{
    _minimum = minimum;
    _maximum = maximum;

    _accumulate = 0;

    ///检查进度范围，若两端均为0，则设置为繁忙样式
    if(minimum == 0 && maximum == 0)
        return QProgressBar::setRange(0,0);

    ///设置进度为预设进度，用于正确显示百分比进度
    else if(QProgressBar::maximum() != _presetMaximum)
    {
        QProgressBar::setRange(0,_presetMaximum);
        QProgressBar::setValue(0);
    }
}

void CustomProgressBar::setRange(int minimum, int maximum)
{
    return setRange(qint64(minimum),qint64(maximum));
}

void CustomProgressBar::setMinimum(qint64 minimum)
{
    _minimum = minimum;

    _accumulate = 0;

    if(QProgressBar::minimum() != 0)
        return QProgressBar::setMinimum(0);
}

void CustomProgressBar::setMinimum(int minimum)
{
    return setMinimum(qint64(minimum));
}

void CustomProgressBar::setMaximum(qint64 maximum)
{
    _maximum = maximum;

    _accumulate = 0;

    if(QProgressBar::maximum() != _presetMaximum)
        return QProgressBar::setMaximum(_presetMaximum);
}

void CustomProgressBar::setMaximum(int maximum)
{
    return setMaximum(qint64(maximum));
}
