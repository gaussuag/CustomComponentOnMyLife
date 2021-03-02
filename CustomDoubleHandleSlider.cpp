#include "CustomDoubleHandleSlider.h"

CustomDoubleHandleSlider::CustomDoubleHandleSlider(QWidget* parent)
    : QWidget(parent)
    , m_min(0.0f)
    , m_max(100.0f)
    , m_singleStep(0.5f)
    , m_minValue(0.0f)
    , m_maxValue(100.0f)
    , _MaxHandlestate(None)
    , _MinHandlestate(None)
    , _GrooveEffectState(None)
{
    setAttribute(Qt::WA_StyledBackground);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

CustomDoubleHandleSlider::~CustomDoubleHandleSlider()
{

}

void CustomDoubleHandleSlider::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    paintWidget(&painter);
}
void CustomDoubleHandleSlider::paintWidget(QPainter* painter)
{
    _ValueStringSpaceHeight = height()*2/5;
    _grooveHeight = (height() - _ValueStringSpaceHeight)/4;
    _handleHeight = height() - _ValueStringSpaceHeight;

    int fontsize;
    fontsize =  _ValueStringSpaceHeight >= 12 ? 9:7;

    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial",fontsize));

    QString ValueString;
    if(_MinHandlestate == PressedHandle)
        ValueString = QString::number(m_minValue,'f',3);
    else if(_MaxHandlestate == PressedHandle)
        ValueString  = QString::number(m_maxValue,'f',3);
    else if(_GrooveEffectState == PressedHandle)
        ValueString = QString::number(effectRange(),'f',3);
    else
        ValueString = QString("");

    QFontMetrics metrics = painter->fontMetrics();
    int textWidth = metrics.width(ValueString);
    int textHeight = metrics.height();
    //---- paint text
    painter->drawText(QRectF((this->width()-textWidth)/2,this->height()-_ValueStringSpaceHeight,textWidth,textHeight),ValueString);

    int minPos = ( m_minValue - m_min ) * width() / (m_max - m_min);
    int maxPos = (m_maxValue - m_min ) * width() /  (m_max - m_min);

    if(minPos <= 4){
        minPos = 4;
    }else if(minPos >= width() - 8){
        minPos = width() - 8;
    }else{}

    if(maxPos <= 4){
        maxPos = 4;
    }else if(maxPos >= width() -8){
        maxPos = width() - 8;
    }else{}


    //----- paint groove
    _GrooveEffectRegion = QRect(minPos + 4,(_handleHeight - _grooveHeight)/2,maxPos - minPos,_grooveHeight);
    paintColoredRect(QRect(4,(_handleHeight - _grooveHeight)/2,width() - 8,_grooveHeight),_grooveDefaultColor,painter);
    paintColoredRect(_GrooveEffectRegion,_grooveEffectColor,painter);

    //----- handle
    _MinHandleRegion = QRect(minPos ,0,8,_handleHeight);
    _MaxHandleRegion = QRect(maxPos ,0,8,_handleHeight);

    //-----paint Handle
    QColor minColor  = getHandleColor(_MinHandlestate);
    QColor maxColor  = getHandleColor(_MaxHandlestate);
    paintColoredRect(_MinHandleRegion,minColor,painter);
    paintColoredRect(_MaxHandleRegion,maxColor,painter);
}

void CustomDoubleHandleSlider::paintColoredRect(QRect rect, QColor color ,QPainter* painter)
{
    painter->fillRect(rect,QBrush(color));
}

QColor CustomDoubleHandleSlider::getHandleColor(State state)
{
    switch (state) {
    case PressedHandle:
        return QColor(38, 255, 248);
    case HoverHandle:
        return QColor(47,47,47);
    default:
        return QColor(51,153,155);
    }
}

inline float getValidValue(float val, float min, float max)
{
    float tmp = std::max(val, min);
    return std::min(tmp, max);
}


void CustomDoubleHandleSlider::setgrooveEffectColor(QColor color)
{
    _grooveEffectColor = color;
}

void CustomDoubleHandleSlider::setRange(float min, float max)
{
    m_min = min;
    m_max = max;

    if(minValue() < m_min)
        setMinValue(m_min);

    if(maxValue() > m_max){
        setMaxValue(m_max);
    }
}

void CustomDoubleHandleSlider::setSingleStep(float step)
{
    m_singleStep = step;
}

float CustomDoubleHandleSlider::minValue() const
{
    return m_minValue;
}
void CustomDoubleHandleSlider::setMinValue(float val)
{
    if(qAbs( m_minValue - val ) > 0.0001 ){
        m_minValue = val;
        emit minValueChanged(qint64(val*1000));
    }
}

float CustomDoubleHandleSlider::maxValue() const
{
    return m_maxValue;
}
void CustomDoubleHandleSlider::setMaxValue(float val)
{
    if(qAbs(m_maxValue - val) > 0.0001){
        m_maxValue = val;
        emit maxValueChanged(qint64(val*1000));
    }
}


float CustomDoubleHandleSlider::minRange() const
{
    return m_min;
}
float CustomDoubleHandleSlider::maxRange() const
{
    return m_max;
}


void CustomDoubleHandleSlider::mousePressEvent(QMouseEvent* event)
{
    if(_MaxHandleRegion.contains(event->pos())){
            _MaxHandlestate = PressedHandle;
    }else if(_MinHandleRegion.contains(event->pos())){
        _MinHandlestate = PressedHandle;
    }else if(_GrooveEffectRegion.contains(event->pos())){
        _GrooveEffectState = PressedHandle;
    }else{
        _MaxHandlestate = None;
        _MinHandlestate = None;
        _GrooveEffectState = None;
    }

    if(qAbs(minValue() - maxValue()) < 0.001){
        if(qAbs(minValue() - m_min) < 0.001){
            _MaxHandlestate = PressedHandle;
        }else if(qAbs(minValue() - m_max) < 0.001){
            _MinHandlestate = PressedHandle;
        }else{}
    }

    update();
    return QWidget::mousePressEvent(event);
}
void CustomDoubleHandleSlider::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons() == Qt::NoButton)
    {
        if(_MaxHandleRegion.contains(event->pos())){
            _MaxHandlestate = HoverHandle;
        }else if(_MinHandleRegion.contains(event->pos())){
            _MinHandlestate = HoverHandle;
        }else{
            _MaxHandlestate = None;
            _MinHandlestate = None;
        }
    }
    else if(event->buttons() & Qt::LeftButton)
    {
        float move = event->x() * (m_max - m_min) * 1.0/ width() + m_min;

        if(_MaxHandlestate == PressedHandle)
        {
            float val = getValidValue(move, minValue(), m_max);
            setMaxValue(val);
        }
        else if(_MinHandlestate == PressedHandle)
        {
            float val = getValidValue( move, m_min,maxValue());
            setMinValue(val);
        }
    }
    update();
}
void CustomDoubleHandleSlider::mouseReleaseEvent(QMouseEvent *event)
{
    _MaxHandlestate = None;
    _MinHandlestate = None;
    update();
    return QWidget::mouseReleaseEvent(event);
}
void CustomDoubleHandleSlider::leaveEvent(QEvent* event)
{
    _MaxHandlestate = None;
    _MinHandlestate = None;
    update();
    return QWidget::leaveEvent(event);

}
void CustomDoubleHandleSlider::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Left)
    {
        if(_MinHandlestate == PressedHandle){
            float val = getValidValue(minValue()- m_singleStep, m_min ,maxValue());
            setMinValue(val);
        }else if(_MaxHandlestate == PressedHandle) {
            float val = getValidValue(maxValue()- m_singleStep, minValue() ,m_max);
            setMaxValue(val);
        }else{}
    }else if(event->key() == Qt::Key_Right){
        if(_MinHandlestate == PressedHandle){
            float val = getValidValue(minValue() + m_singleStep, m_min ,maxValue());
            setMinValue(val);
        }else if(_MaxHandlestate == PressedHandle) {
            float val = getValidValue(maxValue() + m_singleStep, minValue() ,m_max);
            setMaxValue(val);
        }else{}
    }

    update();
    return  QWidget::keyPressEvent(event);
}



/*MediaItem Parameter Option*/
QString CustomDoubleHandleSlider::DurationToTimeformat(qint64 duration)
{
    duration += 100; //time compensate
    int totalsec = duration/1000;
    int totalmin = totalsec/60;

    int second = totalsec%60;
    int minute = totalmin%60;
    int hour = totalmin/60;

    QString Timeformat;
    if(hour < 10)
        Timeformat.append(QString("0%1").arg(hour));
    else
        Timeformat.append(hour);

    if(minute < 10)
        Timeformat.append(QString(":0%1").arg(minute));
    else
        Timeformat.append(QString(":%1").arg(minute));

    if(second < 10)
        Timeformat.append(QString(":0%1").arg(second));
    else
        Timeformat.append(QString(":%1").arg(second));

    return Timeformat;
}
