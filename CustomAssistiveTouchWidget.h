#ifndef CUSTOMASSISTIVETOUCHWIDGET_H
#define CUSTOMASSISTIVETOUCHWIDGET_H

#include "CustomToolButton.h"

#include <QEvent>
#include <QMouseEvent>
#include <QStyleOption>

class QHBoxLayout;
class QTiemr;

class CustomAssistiveTouchWidget: public CustomToolButton
{
    Q_OBJECT
public:
    	explicit CustomAssistiveTouchWidget(QWidget *parent = nullptr);
    	~CustomAssistiveTouchWidget();
public slots:
    //void show();


protected:
	bool eventFilter(QObject * watcher, QEvent * event);
	void mouseMoveEvent(QMouseEvent * event);

private:
	QHBoxLayout *_CenterWidgetLayout=nullptr;

	int _mousePressTime{500};
	bool _mousePressFlag{false};
	QTimer *_mousePressTimer{nullptr};

	bool _moveFlag{false};
	QPoint _PressedPos;
	QPoint _CurrentPos;

	void initWidget();
	

	/**
	 * /brief 处理鼠标按下事件
	 * /details 在此函数中进行鼠标按下定时器初始化并开启定时器判断鼠标按下时长。
	 */
	void handleMouseButtonPressEvent();

	/**
	 * /brief 处理鼠标弹起事件
	 * /details 在此函数中对鼠标按下定时器进行清除。
	 */
	void handleMouseButtonReleaseEvent();
	/*!
	 * /brief 鼠标按下超时响应事件
	 * 
	 */
	void handlePressTimeOutEvent();

signals:
	void widgetshow(QWidget *);
};

#endif // CUSTOMASSISTIVETOUCHWIDGET_H
