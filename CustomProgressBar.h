#ifndef CUSTOMPROGRESSBAR_H
#define CUSTOMPROGRESSBAR_H

#include <QProgressBar>

class CustomProgressBar : public QProgressBar
{
public:
    CustomProgressBar(QWidget *parent = nullptr);
    ~CustomProgressBar();

public slots:
    /*!
     * \brief setValue 更新当前进度值，此Value为进度单次变化值，类中会自动累积进度，并计算百分比设置到进度上。
     * \param value
     */
    void setValue(qint64 value);
    void setValue(int value);

    /*!
     * \brief setRange 设置进度条范围
     * \param minimum
     * \param maximum
     * \note 若最大值和最小值均为0,则进度条显示繁忙样式
     */
    void setRange(qint64 minimum,qint64 maximum);
    void setRange(int minimum,int maximum);

    /*!
     * \brief setMinimum 设置进度条的最小值
     * \param minimum
     */
    void setMinimum(qint64 minimum);
    void setMinimum(int minimum);

    /*!
     * \brief setMaximum 设置进度条的最大值，为进度操作总数值；将会把原生进度最大值设置为预设值。
     * \param maximum
     */
    void setMaximum(qint64 maximum);
    void setMaximum(int maximum);
private:
    qint64 _minimum{0};
    qint64 _maximum{100};
    qint64 _accumulate{0};

    int _presetMaximum{10000};
};

#endif // CUSTOMPROGRESSBAR_H
