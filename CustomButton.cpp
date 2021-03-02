#include "CustomButton.h"

CustomButton::CustomButton(QWidget *parent) : QPushButton(parent)
{

}

CustomButton::CustomButton(const QString &text, QWidget *parent) :QPushButton(text,parent)
{

}

CustomButton::~CustomButton()
{
}


void CustomButton::setButtonBackgroundColor(QColor Color)
{
	if (!this->autoFillBackground())
		this->setAutoFillBackground(true);
	if (!this->isFlat())
		this->setFlat(true);
	QPalette ButtonPalette = this->palette();
	ButtonPalette.setColor(QPalette::Button, Color);
    this->setPalette(ButtonPalette);

    _backgroundColor = Color;
}

void CustomButton::setTransparentStyleSheet()
{
	QString originSheet = this->styleSheet();

	originSheet.append("QPushButton{background-color:transparent;}"
		"QPushButton::hover{background-color:transparent;color:white;border-radius:5px;border:1px solid gray;}"
		"QPushButton::checked{background-color:transparent;padding-left:2px;padding-top:2px}"
		"QPushButton::pressed{background-color:transparent;padding-left:2px;padding-top:2px}"
		);

	this->setStyleSheet(originSheet);
}


void CustomButton::setBlueBlueWhiteStyleSheet()
{
    this->setStyleSheet("QPushButton{background-color:#2680eb;color:white;border-radius:2px;}"
                        "QPushButton::hover{background-color:#1f6bc1;color:white;border-radius:2px;}"
                        "QPushButton::pressed{background-color:#1f6bc1;color:white;border-radius:2px;padding-left:2px;padding-top:2px;}"
                        "QPushButton::!enabled{background-color:#e4e4e4;color:black;border-radius:2px;}");
}


void CustomButton::setFontPixelSize(int size)
{
	QFont font = this->font();
	font.setPixelSize(size);
	setFont(font);
}

void CustomButton::setChecked(bool state)
{
	if ((!_checkedIcon.isNull()) && (!_defaultIcon.isNull()))
	{
		if (state)
			QPushButton::setIcon(_checkedIcon);
		else
			QPushButton::setIcon(_defaultIcon);
	}
	return QPushButton::setChecked(state);
}

void CustomButton::setIcon(const QIcon & defaultIcon)
{
	_defaultIcon = defaultIcon;
	setIconSize(this->size());
	return QPushButton::setIcon(_defaultIcon);
}

void CustomButton::setCheckedIcon(const QIcon & checkedIcon)
{
	_checkedIcon = checkedIcon;

	connect(this, &CustomButton::clicked, this, [=](bool state) {
		if (state)
			QPushButton::setIcon(_checkedIcon);
		else
			QPushButton::setIcon(_defaultIcon);
	}, Qt::UniqueConnection);

	if (isChecked())
		return QPushButton::setIcon(_checkedIcon);
}

void CustomButton::setHoverIcon(const QIcon & hoverIcon)
{
	_hoverIcon = hoverIcon;
	connect(this, &CustomButton::mouseEnter, this, [=]() {
		if (!_hoverIcon.isNull())
			QPushButton::setIcon(_hoverIcon);
	}, Qt::UniqueConnection);
	connect(this, &CustomButton::mouseLeave, this, [=]() {
		if (!_defaultIcon.isNull())
			QPushButton::setIcon(_defaultIcon);
	}, Qt::UniqueConnection);
}

void CustomButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
        emit(clickedRightButton());

    return QPushButton::mouseReleaseEvent(event);
}

bool CustomButton::event(QEvent *e)
{
	if (e->type() == QEvent::ToolTip)
		emit(showToolTip());
	return QPushButton::event(e);
}

void CustomButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    emit(mouseEnter());
}

void CustomButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    emit(mouseLeave());
}

void CustomButton::sendClickedSignal(bool state)
{
    this->setChecked(state);
    emit(clicked(state));
}

 
