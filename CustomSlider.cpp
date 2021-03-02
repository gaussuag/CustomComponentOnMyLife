#include "CustomSlider.h"

#include <QStyle>


CustomSlider::CustomSlider(Qt::Orientation SliderType,int SliderThickness,QWidget *parent)
    :QSlider(parent)
{
    init_SliderButton(SliderType,SliderThickness);
    setMouseTracking(true);
}

CustomSlider::~CustomSlider()
{

}

void CustomSlider::setIgnoreDirectionKeyFlag(bool flag)
{
    _ignoreDirectionKeyFlag = flag;
}

int CustomSlider::sliderPositionFromValue(int Value)
{
    const int o = style()->pixelMetric(QStyle::PM_SliderLength) - 1;
    int position = QStyle::sliderPositionFromValue(minimum(),maximum(),Value,width()-o,false);
    return position;
}

void CustomSlider::mousePressEvent(QMouseEvent *ev)
{
    _mousePressedFlag = true;
    return QSlider::mousePressEvent(ev);
}

void CustomSlider::mouseMoveEvent(QMouseEvent *event)
{
    ///鼠标未按下 悬浮移动则发送信号
    if(!_mousePressedFlag)
    {
        const int o = style()->pixelMetric(QStyle::PM_SliderLength) - 1;
        int value = QStyle::sliderValueFromPosition(minimum(),maximum(), (event->pos().x() - o/2) ,width() - o , false);
        emit(hoverValueChanged(event->globalPos(),value));
    }
    return QSlider::mouseMoveEvent(event);
}

void CustomSlider::mouseReleaseEvent(QMouseEvent *ev)
{
    _mousePressedFlag = false;

    const int o = style()->pixelMetric(QStyle::PM_SliderLength) - 1;
    int value = QStyle::sliderValueFromPosition(minimum(),maximum(), (ev->pos().x() - o/2) ,width() - o , false);
    emit(clickedSlider(value));

    return QSlider::mouseReleaseEvent(ev);
}

void CustomSlider::leaveEvent(QEvent *event)
{
    emit(leaveSlider());
    return QSlider::leaveEvent(event);
}

void CustomSlider::keyPressEvent(QKeyEvent *event)
{
	if (_ignoreDirectionKeyFlag && ((event->key() == Qt::Key_Left) || (event->key() == Qt::Key_Right) || (event->key() == Qt::Key_Up) || (event->key() == Qt::Key_Down)))
		return;

	return QSlider::keyPressEvent(event);
}
void CustomSlider::keyReleaseEvent(QKeyEvent *event)
{
	if (_ignoreDirectionKeyFlag && ((event->key() == Qt::Key_Left) || (event->key() == Qt::Key_Right) || (event->key() == Qt::Key_Up) || (event->key() == Qt::Key_Down)))
		return;

	return QSlider::keyReleaseEvent(event);
}

void CustomSlider::init_SliderButton(Qt::Orientation SliderType,int SliderThickness)
{
    if(SliderType == Qt::Horizontal)
    {
        DefaultHeight = SliderThickness;
        setHorizontalSliderQss(DefaultHeight);
    }
    else if(SliderType == Qt::Vertical)
    {
        DefaultWidth = SliderThickness;
        setVerticalSliderQss(DefaultWidth);
    }
}

void CustomSlider::setHorizontalSliderQss(int height,
                                    const QString &backgroundColor,
                                    const QString &backgroundimage_url,
                                    const QString &normalColor,const QString &handleColor)
{
    this->setOrientation(Qt::Horizontal);

    int sliderHeight = height;
    int _sliderHeight = sliderHeight/8;
    int sliderRadius = _sliderHeight / 2;
    int handleWidth = _sliderHeight*10;
    int handleOffset = (handleWidth-_sliderHeight) / 2;
    int handleRadius = (handleOffset*2+_sliderHeight)/2;

    QStringList qss;
    qss.append(QString("QSlider{background-color: %2;border-radius:%1px;}")
               .arg(height).arg(backgroundColor));

    qss.append(QString("QSlider::groove:horizontal{background:%1;height:%2px;border-radius:%3px;}")
               .arg(normalColor).arg(_sliderHeight).arg(sliderRadius));
    qss.append(QString("QSlider::add-page:horizontal{background:%1;height:%2px;border-radius:%3px;}")
               .arg(normalColor).arg(_sliderHeight).arg(sliderRadius));
    qss.append(QString("QSlider::sub-page:horizontal{background:%1;height:%2px;border-radius:%3px;}")
               .arg(normalColor).arg(_sliderHeight).arg(sliderRadius));
    qss.append(QString("QSlider::handle:horizontal{"
                       "background:%1;"
                       "width:%2px;margin-top:-%3px;margin-bottom:-%3px;border-radius:%4px;"
                       "background-image:%5;}")
               .arg(handleColor).arg(handleWidth).arg(handleOffset).arg(handleRadius).arg(backgroundimage_url));
    this->setStyleSheet(qss.join(""));

    handlewidth = handleWidth;
    handleheight = handleOffset*2+_sliderHeight;
}

void CustomSlider::setVerticalSliderQss(int width,
                                    const QString &backgroundColor,
                                    const QString &backgroundimage_url,
                                    const QString &normalColor,const QString &handleColor)
{
    int sliderWidth = width;
    int _sliderWidth = sliderWidth/3;
    int sliderRadius = _sliderWidth / 2;
    int handleHeight = sliderWidth-5;//控制Handle的大小-高度
    int handleOffset = (handleHeight-_sliderWidth) / 2;
    int handleRadius = (handleOffset*2+_sliderWidth)/2;

    this->setOrientation(Qt::Vertical);

    QStringList qss;
    qss.append(QString("QSlider{background-color: %2;border-radius:%1px;}")
               .arg(width/3).arg(backgroundColor));

    qss.append(QString("QSlider::groove:vertical{background:%1;width:%2px;border-radius:%3px;}")
               .arg(normalColor).arg(_sliderWidth).arg(sliderRadius));
    qss.append(QString("QSlider::add-page:vertical{background:%1;width:%2px;border-radius:%3px;}")
               .arg(normalColor).arg(_sliderWidth).arg(sliderRadius));
    qss.append(QString("QSlider::sub-page:vertical{background:%1;width:%2px;border-radius:%3px;}")
               .arg(normalColor).arg(_sliderWidth).arg(sliderRadius));
    qss.append(QString("QSlider::handle:vertical{"
                       "background:%1;"
                       "height:%2px;margin-left:-%3px;margin-right:-%3px;border-radius:%4px;"
                       "background-image:%5;}")
               .arg(handleColor).arg(handleHeight).arg(handleOffset).arg(handleRadius).arg(backgroundimage_url));

    this->setStyleSheet(qss.join(""));

    handleheight = handleHeight;
    handlewidth = handleheight;

}


void CustomSlider::setSliderMaxRange(qint64 MaxRange)
{
    this->setRange(0,int(MaxRange));
}
