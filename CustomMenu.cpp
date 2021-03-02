#include "CustomMenu.h"
#include <QToolButton>
#include <QHBoxLayout>

#include <QDebug>

CustomMenu::CustomMenu(QWidget *parent)
    : QMenu(parent)
{
	initDefaultMenu();
}

CustomMenu::CustomMenu(const QString & title, QWidget * parent) : QMenu(title,parent)
{
	initDefaultMenu();
}

CustomMenu::~CustomMenu()
{
}

QPair<QAction*, QAction*> CustomMenu::addDoubleAction(const QString & text)
{
	return addDoubleAction(QIcon(), text);
}
QPair<QAction*, QAction*> CustomMenu::addDoubleAction(const QIcon & icon, const QString & text)
{
	QWidget * widget = new QWidget(this);
	QToolButton *subButton = initDoubleActionWidget(icon, text, widget);
	QAction *mainAction = addWidget(widget);
	QAction *subAction = new QAction(this);

	connect(subButton, &QToolButton::clicked, this, [=](bool flag) {
		emit(subAction->triggered(flag));
		close();
	});

	QPair<QAction*, QAction*> ActionPair;
	ActionPair.first = mainAction;
	ActionPair.second = subAction;

	return ActionPair;
}

QAction* CustomMenu::addWidget(QWidget *widget)
{
	widget->installEventFilter(this);
	QWidgetAction *WidgetAction = new QWidgetAction(this);
	WidgetAction->setDefaultWidget(widget);
	addAction(WidgetAction);

	_WidgetObjectActionMap.insert(widget, WidgetAction);

	return WidgetAction;
}

bool CustomMenu::eventFilter(QObject *watched, QEvent *event)
{
    if(_WidgetObjectActionMap.contains(watched))
    {
        QAction *action = _WidgetObjectActionMap.value(watched);
        if(action != nullptr)
        {
            if(event->type() == QEvent::Enter)
                setActiveAction(action);
            else if(event->type() == QEvent::MouseButtonRelease)
            {
                emit (action->triggered());
                close();
            }
        }
    }

    return QMenu::eventFilter(watched,event);
}

void CustomMenu::initDefaultMenu()
{
	installEventFilter(this);
	setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

    setStyleSheet("QMenu{background-color:white;border: 1px solid gray; border-radius:5px;padding:5px 5px;}"
        "QMenu::item{background-color:white;padding:10px 12px;margin:0px 12px;}"
        "QMenu::item:selected{background-color:#f4f4f4;}"
        "QMenu::item::!enabled:unchecked{text-decoration: line-through;}"
        "QMenu::item::checked{text-decoration: underline;font-weight: bold;}");

    MainWidgetStyleSheet = "QWidget{background-color:transparent;border:0px solid gray;}"
        "QWidget::hover{background-color:#f4f4f4;border:0px solid gray;}";
    SubButtonStyleSheet = "QToolButton{background-color:transparent;border:1px solid black;border-radius:4px;}"
        "QToolButton::hover{background-color:transparent;border:1px solid red;border-radius:4px;}";
}



QToolButton* CustomMenu::initDoubleActionWidget(const QIcon & icon,const QString & text,QWidget *widget)
{
	widget->setStyleSheet(MainWidgetStyleSheet);
	QHBoxLayout *Layout = new QHBoxLayout(widget);

	QToolButton *MainButton = new QToolButton(widget);
	MainButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	MainButton->setAttribute(Qt::WA_TransparentForMouseEvents);
	MainButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	MainButton->setCheckable(false);
	MainButton->setIcon(icon);
	MainButton->setText(text);

	QToolButton *subButton = new QToolButton(widget);
	subButton->setFixedSize(12,12);
	subButton->setCheckable(false);
	subButton->setStyleSheet(SubButtonStyleSheet);

	Layout->addWidget(MainButton);
	Layout->addWidget(subButton);

	return subButton;
}

void CustomMenu::insertActionToMenu(int index, QAction* p_action, bool isAddSeparator)
{
    auto allActions = actions();
    if (allActions.size() <= index)
    {
        addAction(p_action);
        return;
    }

    insertAction(allActions.at(index),p_action);
    if(isAddSeparator)
        insertSeparator(allActions.at(index));
}

void CustomMenu::setBlackBlueStyleSheet()
{
    setStyleSheet("QMenu{background-color:#2F2F2F;border: 1px solid #5A5A5A; border-radius:4px;padding:5px 5px;}"
        "QMenu::item{background-color:transparent;color:#ffffff;padding:10px 12px;margin:0px 12px;}"
        "QMenu::item:selected{background-color:#332680EB;}"
        "QMenu::item::!enabled:unchecked{text-decoration: line-through;color:#6E6E6E;}"
        "QMenu::item::checked{text-decoration: underline;}");

    MainWidgetStyleSheet = "QWidget{background-color:transparent;border:0px solid gray;}"
        "QWidget::hover{background-color:#2680EB;border:0px solid gray;}";
    SubButtonStyleSheet = "QToolButton{background-color:transparent;border:1px solid black;border-radius:4px;}"
        "QToolButton::hover{background-color:transparent;border:1px solid red;border-radius:4px;}";

}
