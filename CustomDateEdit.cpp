#include "CustomDateEdit.h"
#include "CBasicPath.h"

using namespace NSCBasicClass;
CustomDateEdit::CustomDateEdit(QWidget *parent) : QLineEdit(parent)
{
    initWidget();
}
CustomDateEdit::CustomDateEdit(QDate date,QWidget *parent) :_Date(date), QLineEdit(parent)
{
    initWidget();
    setDate(_Date);
}

CustomDateEdit::~CustomDateEdit()
{
    delete _CalendarWidget;
}

void CustomDateEdit::setDate(QDate date)
{
    _CalendarWidget->setSelectedDate(date);
    setText(date.toString(_DateFormat));
}
void CustomDateEdit::setActionIcon(QIcon icon)
{
    _DateEditAction->setIcon(icon);
}

void CustomDateEdit::RemoveMiniDate()
{
	QDate defaultDate(1990,1,1);
	_CalendarWidget->setMinimumDate(defaultDate);
}

void CustomDateEdit::setCalendarRange(QPair<QDate, QDate>& range)
{
	_CalendarWidget->setDateRange(range.first, range.second);
}

bool CustomDateEdit::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == _CalendarWidget && event->type() == QEvent::WindowDeactivate)
    {
        if(_CalendarWidget->isVisible())
            _CalendarWidget->hide();
    }

    return QLineEdit::eventFilter(watched,event);
}

void CustomDateEdit::initWidget()
{
    _DateEditAction = new QAction(this);
	_DateEditAction->setIcon(QIcon(MergeIconPath("/calendar.png")));
    this->setAlignment(Qt::AlignCenter);

    this->addAction(_DateEditAction,QLineEdit::TrailingPosition);
    connect(_DateEditAction,SIGNAL(triggered(bool)),this,SLOT(DateEditAction_triggered_slot(bool)));

    _CalendarWidget = new QCalendarWidget(this);
    _CalendarWidget->installEventFilter(this);
	_CalendarWidget->setMinimumDate(QDate::currentDate());
    _CalendarWidget->setWindowFlags((Qt::Drawer|Qt::FramelessWindowHint));
    _CalendarWidget->hide();
    connect(_CalendarWidget,SIGNAL(clicked(QDate)),this,SLOT(CalendarWidget_clicked_slot(QDate)));

	setReadOnly(true);
}


void CustomDateEdit::setMinimumDate_slot(QDate date)
{
	_CalendarWidget->setMinimumDate(date);
	QDate currentdate = QDate::fromString(this->text(),"yyyy.MM.dd");
	if(currentdate < date)
		setText(date.toString(_DateFormat));
}

void CustomDateEdit::setMaximumDate_slot(QDate date)
{
	_CalendarWidget->setMaximumDate(date);
	QDate currentdate = QDate::fromString(this->text(), "yyyy.MM.dd");
	if (currentdate > date)
		setText(date.toString(_DateFormat));
}

void CustomDateEdit::DateEditAction_triggered_slot(bool state)
{
    Q_UNUSED(state);
    QPoint currentPos = mapToGlobal(QPoint(0,this->height()));
    _CalendarWidget->setGeometry(currentPos.x(),currentPos.y(),400,350);
    if(_CalendarWidget->isVisible())
        _CalendarWidget->hide();
    else
    {
        _CalendarWidget->setSelectedDate(QDate::fromString(text(),_DateFormat));
        _CalendarWidget->raise();
        _CalendarWidget->show();
    }
}
void CustomDateEdit::CalendarWidget_clicked_slot(QDate date)
{
    setText(date.toString(_DateFormat));
    if(_CalendarWidget->isVisible())
        _CalendarWidget->hide();

	emit(CustomDateChange(date));
}

void CustomDateEdit::SetCalendarShowVerticalBar(bool bShow)
{
    if (_CalendarWidget)
        _CalendarWidget->setVerticalHeaderFormat(bShow?QCalendarWidget::ISOWeekNumbers:QCalendarWidget::NoVerticalHeader);
}

