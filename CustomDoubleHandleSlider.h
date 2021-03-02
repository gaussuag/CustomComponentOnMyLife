#ifndef CUSTOMDOUBLEHANDLESLIDER_H
#define CUSTOMDOUBLEHANDLESLIDER_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QEvent>
#include <QDebug>
#include <QLabel>


class CustomDoubleHandleSlider : public QWidget
{
    Q_OBJECT
public:
    enum State{
        PressedHandle,
        HoverHandle,
        None};
    explicit CustomDoubleHandleSlider(QWidget* parent = 0);
    ~CustomDoubleHandleSlider();
    void setRange(float min, float max);
    void setSingleStep(float step);

    float minValue() const;
    float maxValue() const;

    float minRange() const;
    float maxRange() const;

    float effectRange(){return maxValue() - minValue();}

    void setgrooveEffectColor(QColor color);
public slots:
    void setMaxValue(float val);
    void setMinValue(float val);
protected:
    void paintEvent(QPaintEvent* event);
    void paintColoredRect(QRect rect, QColor color, QPainter* painter);
    void paintWidget(QPainter* painter);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void leaveEvent(QEvent* event);


private:
    float m_min;
    float m_max;
    float m_singleStep;

    float m_minValue;
    float m_maxValue;

    QRect _MinHandleRegion;
    QRect _MaxHandleRegion;
    QRect _GrooveEffectRegion;

    State _MaxHandlestate;
    State _MinHandlestate;
    State _GrooveEffectState;
    //Slider style parameter
    int _grooveHeight=8;
    int _handleHeight = 16;
    int _ValueStringSpaceHeight;

    QColor _grooveDefaultColor = Qt::gray;
    QColor _grooveEffectColor = Qt::green;


    QColor getHandleColor(State state);
    QString DurationToTimeformat(qint64 duration);
signals:
    void minValueChanged(float);
    void maxValueChanged(float);
    void minValueChanged(qint64);
    void maxValueChanged(qint64);


};

#endif // CUSTOMDOUBLEHANDLESLIDER_H
