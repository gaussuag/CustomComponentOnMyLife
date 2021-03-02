#include "CustomToolButton.h"
#include <QCursor>
#include <QApplication>
#include <QDebug>



CustomToolButton::CustomToolButton(QWidget *parent) : QToolButton(parent)
  , _movie(nullptr)
{

}
CustomToolButton::CustomToolButton(const QString& text, QWidget *parent):QToolButton(parent)
  , _movie(nullptr)
{
    this->setText(text);
}

CustomToolButton::~CustomToolButton()
{

}

void CustomToolButton::setIconGif(QString GifPath)
{
    if (_movie != nullptr)
    {
        if (_movie->state() == QMovie::Running)
            _movie->stop();

        delete  _movie;
        _movie = nullptr;
        this->setIcon(QIcon());
    }

    _movie = new QMovie(GifPath, QByteArray(), this);
    connect(_movie, &QMovie::frameChanged, this, [&]() {
        this->setIcon(_movie->currentPixmap());
    }, Qt::UniqueConnection);

}
void CustomToolButton::startGif()
{
    if (_movie != nullptr)
    {
        if (_movie->state() != QMovie::Running)
            _movie->start();
    }
}
void CustomToolButton::stopGif()
{
    if (_movie != nullptr)
    {
        if (_movie->state() == QMovie::Running)
            _movie->stop();
    }
}

void CustomToolButton::setIconGifenable(bool Flag, QString IconPath)
{
    if (Flag)
    {
        if (_movie == nullptr)
        {
            _movie = new QMovie(IconPath,QByteArray(),this);
            connect(_movie, &QMovie::frameChanged, this, [&]() {
                this->setIcon(_movie->currentPixmap());
            },Qt::UniqueConnection);
            _movie->start();
        }
    }
    else
    {
        if(_movie != nullptr)
        {
            if (_movie->state() == QMovie::Running)
                _movie->stop();

            delete  _movie;
            _movie = nullptr;
            this->setIcon(QIcon(IconPath));
        }
    }
}

void CustomToolButton::setEnterButtonCursorOnButtonUnable(Qt::CursorShape CursorShape)
{
    _EnterButtonCursorShapeOnButtonUnable = CursorShape;
    connect(this, &CustomToolButton::enabledChange, this, [=]() {
        if (isEnabled())
            setEnterButtonSwitchCursorEnable(false);
        else
            setEnterButtonCursor(_EnterButtonCursorShapeOnButtonUnable);
    },Qt::UniqueConnection);
}

void CustomToolButton::setEnterButtonSwitchCursorEnable(bool Flag)
{
    _enterSwitchCursorFlag = Flag;
}

void CustomToolButton::setEnterButtonCursor(Qt::CursorShape CursorShape)
{
    _enterSwitchCursorFlag = true;
    _enterCursorShape = CursorShape;
}

void CustomToolButton::setIcon(const QIcon & defaultIcon)
{
    if(defaultIcon.isNull())
        return ;

    _defaultIcon = defaultIcon;
    return QToolButton::setIcon(_defaultIcon);
}

void CustomToolButton::setCheckedIcon(const QIcon & checkedIcon)
{
    if(checkedIcon.isNull())
        return ;

    _checkedIcon = checkedIcon;

    connect(this, &CustomToolButton::clicked, this, [=](bool state) {
        if (state)
            QToolButton::setIcon(_checkedIcon);
        else
            QToolButton::setIcon(_defaultIcon);
    }, Qt::UniqueConnection);

    if (isChecked())
        return QToolButton::setIcon(_checkedIcon);
}

void CustomToolButton::setHoverIcon(const QIcon & hoverIcon)
{
    if(hoverIcon.isNull())
        return;

    _hoverIcon = hoverIcon;
    connect(this, &CustomToolButton::mouseEnter, this, [=]() {
        if (!_hoverIcon.isNull())
            QToolButton::setIcon(_hoverIcon);
    }, Qt::UniqueConnection);
    connect(this, &CustomToolButton::mouseLeave, this, [=]() {
        if (!_defaultIcon.isNull())
            QToolButton::setIcon(_defaultIcon);
    }, Qt::UniqueConnection);
}

void CustomToolButton::setHoverMaskText(const QString &text,const QColor & TextColor)
{
    _isHoverMaskTextFlag = true;
    _hoverMaskText = text;
    _hoverMaskTextColor = TextColor;
}


void CustomToolButton::setCenterAlignment()
{
    _CenterAlignment = true;
    _FirstSetAlignmentFlag = true;
}

void CustomToolButton::setRotate(qreal rotate)
{
    if(!text().isEmpty() && !_Transform)
    {
        _Text = text();
        setText("");
    }
    if(!_Transform)
        _Transform = true;
    _Rotate = rotate;
    update();
}
void CustomToolButton::setTransparentStyleSheet()
{
    QString originSheet = this->styleSheet();

    originSheet.append("QToolButton{background-color:transparent;}"
                       "QToolButton::hover{background-color:gray;color:white;border-radius:2px;}"
                       "QToolButton::pressed{background-color:gray;padding-left:2px;padding-top:2px;}");

    this->setStyleSheet(originSheet);
}
void CustomToolButton::setTransparentWhiteBorderStyleSheet()
{
    QString originSheet = this->styleSheet();

    originSheet.append("QToolButton{background-color:transparent;color:white;border:1px solid white;}"
                       "QToolButton::hover{background-color:gray;color:white;border-radius:2px;}"
                       "QToolButton::pressed{background-color:gray;padding-left:2px;padding-top:2px;}");

    this->setStyleSheet(originSheet);
}

void CustomToolButton::setBlackWhiteStyleSheet()
{
    this->setStyleSheet("QToolButton{background-color:#3c3c3c;color:white;border:none;border-radius:4px;}"
                        "QToolButton::hover{background-color:#2181ec;color:white;}"
                        "QToolButton::pressed{background-color:#2181ec;color:white;padding-left:2px;padding-top:2px;}"
                        "QToolButton::checked{background-color:#2181ec;color:white;padding-left:2px;padding-top:2px;}");

}

void CustomToolButton::setBlackBlueWhiteStyleSheet()
{
    this->setStyleSheet("QToolButton{background-color:#4a4a4a;color:white;border-radius:2px;}"
                        "QToolButton::hover{background-color:#2680eb;color:white;border-radius:2px;}"
                        "QToolButton::pressed{background-color:#2680eb;color:white;border-radius:2px;padding-left:2px;padding-top:2px;}"
                        "QToolButton::!enabled{background-color:#e4e4e4;color:black;border-radius:2px;}");

}

void CustomToolButton::setBlueBlueWhiteStyleSheet()
{
    this->setStyleSheet("QToolButton{background-color:#2680eb;color:white;border-radius:2px;}"
                        "QToolButton::hover{background-color:#1f6bc1;color:white;border-radius:2px;}"
                        "QToolButton::pressed{background-color:#1f6bc1;color:white;border-radius:2px;padding-left:2px;padding-top:2px;}"
                        "QToolButton::!enabled{background-color:#e4e4e4;color:black;border-radius:2px;}");
}

void CustomToolButton::setDefaultStyleSheet()
{
    this->setStyleSheet("QToolButton{background-color:#ebebeb;color:black;border-radius:2px;}"
                        "QToolButton::hover{background-color:#56b054;color:white;border-radius:2px;}"
                        "QToolButton::pressed{background-color:#56b054;color:white;border-radius:2px;padding-left:2px;padding-top:2px;}"
                        "QToolButton::checked{background-color:#56b054;color:white;border-radius:2px;padding-left:2px;padding-top:2px;}");

}

void CustomToolButton::setBlackBlackStyleSheet()
{
    this->setStyleSheet("QToolButton{background-color:#555555;color:white;border-radius:2px;}"
                        "QToolButton::hover{background-color:#56b054;color:white;border-radius:2px;}"
                        "QToolButton::pressed{background-color:#56b054;color:white;border-radius:2px;padding-left:2px;padding-top:2px;}"
                        "QToolButton::!enabled{background-color:#e4e4e4;color:black;border-radius:2px;}");
}

void CustomToolButton::setGreenBlackStyleSheet()
{
    this->setStyleSheet("QToolButton{background-color:#56b054;color:black;border-radius:2px;}"
                        "QToolButton::hover{background-color:#56b054;color:white;border-radius:2px;}"
                        "QToolButton::pressed{background-color:#56b054;color:white;border-radius:2px;padding-left:2px;padding-top:2px;}"
                        "QToolButton::!enabled{background-color:#e4e4e4;color:black;border-radius:2px;}");
}
void CustomToolButton::setGreenWhiteStyleSheet()
{
    this->setStyleSheet("QToolButton{background-color:#56b054;color:white;border-radius:2px;}"
                        "QToolButton::hover{background-color:#56b054;color:black;border-radius:2px;}"
                        "QToolButton::pressed{background-color:#56b054;color:black;border-radius:2px;padding-left:2px;padding-top:2px;}"
                        "QToolButton::!enabled{background-color:#e4e4e4;color:black;border-radius:2px;}");
}

void  CustomToolButton::setText(const QString &text)
{
    if(_Transform)
        _Text = text;
    else
        return QToolButton::setText(text);
}
QString CustomToolButton::text()
{
    if(_Transform)
        return _Text;
    else
        return QToolButton::text();
}

void CustomToolButton::setFontPixelSize(int size)
{
    QFont font = this->font();
    font.setPixelSize(size);
    setFont(font);
}

void CustomToolButton::setFontWeight(int weight)
{
    QFont font = this->font();
    font.setWeight(weight);
    setFont(font);
}

void CustomToolButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    emit(mouseEnter());

    if (_enterSwitchCursorFlag)
        switchCursorShape();
}

void CustomToolButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    emit(mouseLeave());

    if (_enterSwitchCursorFlag)
        recoverCursorShape();
}

bool CustomToolButton::event(QEvent *e)
{
    if(e->type() == QEvent::Resize && _CenterAlignment)
        updateCenterAlignmentStylesheet();

    if (e->type() == QEvent::EnabledChange)
        emit(enabledChange());

    if(e->type() == QEvent::HoverEnter)
        _isHoverFlag = true;
    else if(e->type() == QEvent::HoverLeave)
        _isHoverFlag = false;

    return QToolButton::event(e);
}

void CustomToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);
    if(_Transform)
        DrawRotateText();
    if(_isHoverMaskTextFlag && _isHoverFlag)
        DrawHoverMaskText();
}
void CustomToolButton::DrawHoverMaskText()
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

    QFont f = painter.font();

    QFontMetrics fm(f);
    QRectF br(fm.boundingRect(_hoverMaskText));

    QPainterPath path;
    path.addText(0, 0, f, _hoverMaskText);

    QPointF rectCenter = this->rect().center();
    QPointF center(br.center());

    //平移原点位置，根据矩形中心点和文字矩形中心点进行平移计算
    painter.translate(rectCenter-center);

    painter.fillPath(path, _hoverMaskTextColor);

    painter.restore();
}

void CustomToolButton::DrawRotateText()
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

    QFont f = painter.font();
    QFontMetrics fm(f);
    QRectF br(fm.boundingRect(_Text));

    QPainterPath path;
    path.addText(0, 0, f, _Text);

    QPointF rectCenter = this->rect().center();
    QPointF center(br.center());

    //平移原点位置，根据矩形中心点和文字矩形中心点进行平移计算
    painter.translate(rectCenter-center);
    painter.translate(center);
    painter.rotate(_Rotate);
    painter.translate(-center);

    QColor color = this->palette().buttonText().color();
    painter.fillPath(path, color);

    painter.restore();
}

void CustomToolButton::updateCenterAlignmentStylesheet()
{
    QFontMetrics fontMetrics = this->fontMetrics();
    int CenterWidth = fontMetrics.width(this->text()) + this->iconSize().width();
    int Width = this->width();
    int padding_left = (Width - CenterWidth)/2;

    QString originSheet = this->styleSheet();

    if(_FirstSetAlignmentFlag)
    {
        originSheet.append(QString("QToolButton{padding-left:%1}"
                                   "QToolButton::pressed{padding-left:%2;padding-top:1;}"
                                   "QToolButton::checked{padding-left:%2;padding-top:1;}")
                           .arg(padding_left).arg(padding_left + 2));
        _FirstSetAlignmentFlag = false;
    }else
    {
        originSheet.replace(QRegExp("\\bQToolButton\\{padding\\-left\\:\\d*\\}"), QString("QToolButton{padding-left:%1}").arg(padding_left));
        originSheet.replace(QRegExp("\\bQToolButton\\:\\:pressed\\{padding\\-left\\:\\d*\\;\\b"), QString("QToolButton::pressed{padding-left:%2;").arg(padding_left + 2));
        originSheet.replace(QRegExp("\\bQToolButton\\:\\:checked\\{padding\\-left\\:\\d*\\;\\b"), QString("QToolButton::checked{padding-left:%2;").arg(padding_left + 2));
    }

    this->setStyleSheet(originSheet);
}


void CustomToolButton::switchCursorShape()
{
    QApplication::setOverrideCursor(_enterCursorShape);
}
void CustomToolButton::recoverCursorShape()
{
    QApplication::restoreOverrideCursor();
}
