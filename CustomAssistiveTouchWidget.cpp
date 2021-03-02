#include "CustomAssistiveTouchWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QTimer>
#include <qpushbutton.h>
#include <QDebug>

CustomAssistiveTouchWidget::CustomAssistiveTouchWidget(QWidget *parent) : CustomToolButton(parent)
{
	initWidget();
}
CustomAssistiveTouchWidget::~CustomAssistiveTouchWidget()
{}



bool CustomAssistiveTouchWidget::eventFilter(QObject * watcher, QEvent * event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		if (watcher == this)
			handleMouseButtonPressEvent();
	}
	else if (event->type() == QEvent::MouseButtonRelease)
	{
		if (watcher == this)
			handleMouseButtonReleaseEvent();
	}
	return CustomToolButton::eventFilter(watcher,event);
}

void CustomAssistiveTouchWidget::mouseMoveEvent(QMouseEvent *event)
{
	///移动使能标志位使能，进行鼠标移动控件操作
	if (_moveFlag)
	{
		QPoint pos = _CurrentPos + (event->screenPos().toPoint() - _PressedPos);
		move(pos);
	}
	else
	{
		/// 鼠标按下并且在定时器超时之前移出控件范围，则销毁定时器，不做移动使能。
		if (_mousePressFlag && !(this->geometry().contains(event->screenPos().toPoint())))
			handleMouseButtonReleaseEvent();
	}

	return CustomToolButton::mouseMoveEvent(event);
}

void CustomAssistiveTouchWidget::handleMouseButtonPressEvent()
{
	if (_mousePressTimer != nullptr)
	{
		if (_mousePressTimer->isActive())
			return;
	}

	_mousePressFlag = true;
	_mousePressTimer = new QTimer(this);
	connect(_mousePressTimer, &QTimer::timeout, this, [=]() {
		_mousePressTimer->stop();
		if (!_mousePressFlag)
			return handleMouseButtonReleaseEvent();
		else
			handlePressTimeOutEvent();
	});

	_mousePressTimer->start(_mousePressTime);

}

void CustomAssistiveTouchWidget::handleMouseButtonReleaseEvent()
{
	_mousePressFlag = false;
	if(_mousePressTimer != nullptr)
	{
		if(_mousePressTimer->isActive())
			_mousePressTimer->stop();
		delete _mousePressTimer;
		_mousePressTimer = nullptr;
	}
	if (_moveFlag)
	{
		_moveFlag = false;
		setEnabled(true);
	}
}

void CustomAssistiveTouchWidget::handlePressTimeOutEvent()
{
	_moveFlag = true;

	///鼠标按下的坐标
	_PressedPos = QCursor::pos();
	///当前控件的左上角坐标
	_CurrentPos = this->pos();
}

void CustomAssistiveTouchWidget::initWidget()
{
	installEventFilter(this);
	setWindowFlags(Qt::FramelessWindowHint);
	//setAttribute(Qt::WA_TranslucentBackground);
}
