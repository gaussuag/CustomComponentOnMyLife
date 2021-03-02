#include "CustomLabel.h"
#include <QDebug>
#include <QFont>
#include <QRegularExpressionMatchIterator>


CustomLabel::CustomLabel(QWidget *parent): QLabel(parent)
{
    initWidget();
}

CustomLabel::CustomLabel(QString Text, QWidget *parent): QLabel(parent)
{
    initWidget();
    setText(Text);
}

CustomLabel::~CustomLabel()
{

}

void CustomLabel::setFontPixelSize(int size)
{
    QFont font = this->font();
    font.setPixelSize(size);
    setFont(font);
    adjustSize();
}

void CustomLabel::setTipsTextColor()
{
    setFontColor(QColor("#b99f4f"));
}

void CustomLabel::setFontColor(QColor color)
{
    QString originsheet = styleSheet();
    if(originsheet.isEmpty())
        setStyleSheet(QString("CustomLabel{color:%1}").arg(color.name()));
    else
    {
        QRegularExpression rx("[^background-]\\bcolor\\:\\w*\\b");
        QRegularExpressionMatchIterator iterator = rx.globalMatch(originsheet);
        QStringList matchList;
        while (iterator.hasNext())
        {
            QRegularExpressionMatch match = iterator.next();
            if (match.hasMatch())
                matchList.append(match.captured());

        }
        if(!matchList.isEmpty())
        {
            auto colorStr = matchList.first().replace(QRegularExpression("\\bcolor\\:\\w*\\b"),QString("color:%1").arg(color.name()));
            originsheet.replace(QRegularExpression("[^background-]\\bcolor\\:\\w*\\b"),colorStr);
            setStyleSheet(originsheet);
        }else
        {
            originsheet.append(QString("CustomLabel{color:%1;}").arg(color.name()));
            setStyleSheet(originsheet);
        }

    }

}

void CustomLabel::setLinkTextFontColor(const QString &color)
{
    _LinkTextFontColor = color;
}

void CustomLabel::setLinkTextFontSize(int size)
{
    _LinkTextFontSize = size;
}

void CustomLabel::setBackgroundColor(const QColor &color)
{
    if(!autoFillBackground())
        setAutoFillBackground(true);

    auto Palette = palette();
    Palette.setBrush(QPalette::Background,color);
    setPalette(Palette);
}

void CustomLabel::setAutoFitTextHeightFlag(bool flag)
{
    _autoFitTextHeightFlag = flag;
}
bool CustomLabel::getAutoFitTextHeightFlag()
{
    return _autoFitTextHeightFlag;
}

void CustomLabel::setPixmapByColor(const QColor &color)
{
    QPixmap pixmap(size());
    pixmap.fill(color);
    setPixmap(pixmap);
}


void CustomLabel::setIcon(const QPixmap &pixmap, Qt::WindowFrameSection IconPosition)
{
    _drawIconFlag = true;
    _iconPixmap = pixmap;
    _iconPostion = IconPosition;

    update();
}



void CustomLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit(doubleClickedLeftButton());
    else if (event->button() == Qt::RightButton)
        emit(doubleClickedRightButton());
    return QLabel::mouseDoubleClickEvent(event);
}

bool CustomLabel::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this && event->type() == QEvent::Resize)
    {
        emit(resize_Signal());

        if (_autoFitTextHeightFlag)
            this->setFixedHeight(this->fontMetrics().height() + 2);
    }
    else if (watched == this && event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if (mouseEvent != nullptr)
        {
            if (mouseEvent->button() == Qt::LeftButton)
                emit(clickedLeftButton());
            else if (mouseEvent->button() == Qt::RightButton)
                emit(clickedRightButton());
        }
    }
    else if (watched == this && (event->type() == QEvent::Enter))
        emit(enterLabel());
    else if (watched == this && (event->type() == QEvent::Leave))
        emit(leaveLabel());

    return QLabel::eventFilter(watched,event);
}

void CustomLabel::paintEvent(QPaintEvent * event)
{
    QLabel::paintEvent(event);

    if(_drawIconFlag)
        drawIconPixmap();
    if(_enableSelectFlag)
    {
        if(_isSelectedFlag)
            drawSelectedBorder();
    }
}

void CustomLabel::drawIconPixmap()
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
    else if(_iconPostion == Qt::LeftSection)
    {
        auto topLeft = Rect.topLeft();
        auto iconPixmapSize = _iconPixmap.size();
        topLeft.setX(topLeft.x()+_iconPixmapMargin);
        topLeft.setY(topLeft.y()+ (Rect.height() - iconPixmapSize.height())/2);
        Pos = topLeft;
    }
    painter.drawPixmap(Pos,_iconPixmap);
    painter.restore();
}

void CustomLabel::drawSelectedBorder()
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

    /// 根据位置，将Icon Pixmap 绘制
    QRect Rect = rect();
    Rect.adjust(1,1,-1,-1);

    QPen Pen = painter.pen();
    Pen.setColor(_selectedBorderColor);
    Pen.setWidth(2);

    painter.setPen(Pen);

    painter.drawRect(Rect);

    painter.restore();
}


void CustomLabel::setNecessaryFlag(bool state)
{
    _isNecessary = state;
    setText(text());
}


void CustomLabel::setText(const QString &text)
{
    QString string;

    if(_isNecessary)
        string = QString("<html><head/><body><p><span style=\" color:#c2ac8b;\">*</span> %1</p></body></html>").arg(text);
    else if(_isTextClick)
        string = QString("<style> a {text-decoration: none}</style>"
                         "<a style='color: %1;'"
                         "style='font-size:%2px;'"
                         "href=\"/\">  %3").arg(_LinkTextFontColor).arg(_LinkTextFontSize).arg(text);
    else
        string = text;

    QLabel::setText(string);
}

void CustomLabel::setTextClickedEnable(bool state)
{
    _isTextClick = state;
    this->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

    this->setOpenExternalLinks(false);
    connect(this,SIGNAL(linkActivated(QString)),this,SLOT(linkActivated_slot(QString)));
}

void CustomLabel::startTextAnimation(CustomLabel::TextAnimation AnimationFlag)
{
    animationFlag = AnimationFlag;
    initAnimation();
}

void CustomLabel::stopTextAnimation()
{
    startTextAnimation(CustomLabel::TextAnimation::None);
}

void CustomLabel::setHoverMask(bool flag, const QString &backgroundColor, const QPixmap &pixmap, Qt::WindowFrameSection IconPosition)
{
    if(flag)
    {
        if(_maskLabel == nullptr)
        {
            _maskLabel = new CustomLabel(this);
            _maskLabel->setStyleSheet(QString("QLabel{background-color:%1;}")
                                      .arg(backgroundColor));
            if(!pixmap.isNull())
                _maskLabel->setIcon(pixmap,IconPosition);
            _maskLabel->hide();
        }

        connect(this,&CustomLabel::enterLabel,this,[&](){
            _maskLabel->setFixedSize(size());
            _maskLabel->show();
        });
        connect(this,&CustomLabel::leaveLabel,this,[&](){
            _maskLabel->hide();
        });
    }
    else
    {
        disconnect(this,&CustomLabel::enterLabel,this,nullptr);
        disconnect(this,&CustomLabel::leaveLabel,this,nullptr);

        if(_maskLabel)
        {
            delete _maskLabel;
            _maskLabel = nullptr;
        }
    }
}

void CustomLabel::setSelectable(bool flag, const QColor &selectedBorderColor)
{
    _enableSelectFlag = flag;
    _selectedBorderColor = selectedBorderColor;

    if(_enableSelectFlag)
    {
        connect(this,&CustomLabel::clickedLeftButton,this,[&](){
            _isSelectedFlag = !_isSelectedFlag;
            update();
        });
    }else
    {
        disconnect(this,&CustomLabel::clickedLeftButton,this,nullptr);
        if(_isSelectedFlag)
        {
            _isSelectedFlag = false;
            update();
        }
    }


}

void CustomLabel::setUsageDataInt32(qint32 usageData)
{
    UsageDataInt32 = usageData;
}

void CustomLabel::linkActivated_slot(QString string)
{
    Q_UNUSED(string)
    emit(ClickedText());
}


void CustomLabel::initWidget()
{
    setAttribute(Qt::WA_StyledBackground);
    this->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->installEventFilter(this);

    ///设置默认字体为“Arial”
    auto Font = font();
    Font.setFamily("Arial");
    setFont(Font);
}
void CustomLabel::initAnimation()
{
    if (animationFlag == CustomLabel::TextAnimation::EllipsisAnimation)
        initEllipsisAnimation();
    else if (animationFlag == CustomLabel::TextAnimation::None)
        clearAnimation();
}
void CustomLabel::clearAnimation()
{
    if (animationTimer != nullptr)
    {
        if (animationTimer->isActive())
            animationTimer->stop();
        delete animationTimer;
        animationTimer = nullptr;
    }
}
void CustomLabel::initEllipsisAnimation()
{
    if (animationTimer != nullptr)
    {
        if (animationTimer->isActive())
            animationTimer->stop();
        delete animationTimer;
        animationTimer = nullptr;
    }

    if (animationTimer == nullptr)
    {
        ellipsisAnimationString = text();

        animationTimer = new QTimer(this);
        connect(animationTimer, &QTimer::timeout, this, [=]() {
            if (isVisible())
            {
                if (currentEllipsisCount < ellipsisCount)
                {
                    setText(text().append("."));
                    currentEllipsisCount++;
                }
                else if (currentEllipsisCount >= ellipsisCount)
                {
                    currentEllipsisCount = 0;
                    setText(ellipsisAnimationString);
                }
            }
        });
        animationTimer->start(500);
    }
}
