#include "CustomComboBox.h"
#include "CBasicPath.h"

using namespace NSCBasicClass;

CustomComboBox::CustomComboBox(QWidget *parent) : QWidget(parent)
{
    initWidget();
}

QAction *CustomComboBox::getDropDownButtonAction()
{
    return _LineEdit->actions().at(0);
}


bool CustomComboBox::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == _DropDownList && event->type() == QEvent::WindowDeactivate)
    {
        if (_DropDownList->isVisible())
            _DropDownList->hide();
    }

    return QWidget::eventFilter(watched,event);
}

void CustomComboBox::initWidget()
{
    _LineEdit = new QLineEdit(this);
    _LineEdit->setFixedHeight(25);
    _LineEdit->installEventFilter(this);
	_LineEdit->addAction(QIcon(MergeIconPath("/triangle_bottom.png")), QLineEdit::TrailingPosition);
    connect(_LineEdit->actions().at(0),SIGNAL(triggered(bool)),this,SLOT(DropDownButton_slot(bool)));

	QAction *DownArrowKey = new QAction(tr("DownArrowKey"), _LineEdit);
	DownArrowKey->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	DownArrowKey->setShortcut(Qt::Key_Down);
	connect(DownArrowKey, SIGNAL(triggered(bool)), this, SLOT(DownArrowKey_triggered_slot(bool)));
	_LineEdit->addAction(DownArrowKey);

	QAction *UpArrowKey = new QAction(tr("UpArrowKey"), _LineEdit);
	UpArrowKey->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	UpArrowKey->setShortcut(Qt::Key_Up);
	connect(UpArrowKey, SIGNAL(triggered(bool)), this, SLOT(UpArrowKey_triggered_slot(bool)));
	_LineEdit->addAction(UpArrowKey);


    _HBoxLayout = new QHBoxLayout(this);
    _HBoxLayout->setSpacing(0);
    _HBoxLayout->setMargin(0);
    _HBoxLayout->addWidget(_LineEdit);

	initDropDownWidget();
}

void CustomComboBox::initDropDownWidget()
{
    _DropDownList = new QListWidget(this);
    _DropDownList->installEventFilter(this);
    _DropDownList->setStyleSheet("QListWidget{background-color:black;color:white;outline:0px;}");
    _DropDownList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _DropDownList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _DropDownList->setWindowFlags(Qt::Drawer|Qt::FramelessWindowHint);
    _DropDownList->hide();
}




int CustomComboBox::getListWidgetHeightFromItemWidget()
{
    int height = 0;
    for(int i=0;i<_DropDownList->count();i++)
        height += _DropDownList->itemWidget(_DropDownList->item(i))->height();

    return height +4;
}
int CustomComboBox::getListWidgetHeightFromItem()
{
    int height = 0;
    for(int i=0;i<_DropDownList->count();i++)
        height += _DropDownList->item(i)->sizeHint().height();

    return height + 4;
}


int CustomComboBox::getCurrentLineEditTextOnListWidget()
{
	QString currentText = _LineEdit->text();
	for(int i=0;i<_DropDownList->count();i++)
	{
		if (QString::compare(currentText, _DropDownList->item(i)->text()) == 0)
			return i;
	}
	return -1;
}

void CustomComboBox::UpArrowKey_triggered_slot(bool state)
{
	Q_UNUSED(state);
	if (_LineEdit->hasFocus())
	{ 
		int Position = getCurrentLineEditTextOnListWidget();
		if (Position == -1 || Position == 0)
			_LineEdit->setText(_DropDownList->item(_DropDownList->count() - 1)->text());
		else if (Position > 0)
			_LineEdit->setText(_DropDownList->item(Position - 1)->text());
	}
}
void  CustomComboBox::DownArrowKey_triggered_slot(bool state)
{
	Q_UNUSED(state);
	if (_LineEdit->hasFocus())
	{
		int Position = getCurrentLineEditTextOnListWidget();
		if ((Position == -1)  || (Position == (_DropDownList->count() - 1)))
			_LineEdit->setText(_DropDownList->item(0)->text());
		else if (Position >= 0 && Position < _DropDownList->count() - 1)
			_LineEdit->setText(_DropDownList->item(Position + 1)->text());
	}
}

void CustomComboBox::DropDownButton_slot(bool state)
{
    Q_UNUSED(state);

    QPoint point = mapToGlobal(QPoint(0,this->height()));

    _DropDownList->setGeometry(point.x(),point.y(),
                               this->width(),getListWidgetHeightFromItem());
    if(_DropDownList->isVisible())
        _DropDownList->hide();
    else
    {
        _DropDownList->raise();
        _DropDownList->show();
    }
}
void CustomComboBox::clearDropDownListItem()
{
    _DropDownList->clear();
    _LineEdit->clear();
}
void CustomComboBox::SetDropDownListItem(QString text)
{
    addComboBoxItem(text);
}

void CustomComboBox::addComboBoxItem(QListWidgetItem *item)
{
    item->setTextAlignment(Qt::AlignVCenter);
    item->setSizeHint(QSize(this->width(),20));
    _DropDownList->addItem(item);
    connect(_DropDownList,SIGNAL(itemPressed(QListWidgetItem*)),this,SLOT(DropDownList_itemPressed_slot(QListWidgetItem*)));
}

QListWidgetItem* CustomComboBox::addComboBoxItem(const QString & text)
{
    auto item = new QListWidgetItem(_DropDownList);
    item->setText(text);
    addComboBoxItem(item);
    return item;
}


void CustomComboBox::setEditUnable()
{
    _EditEnable = false;
    _LineEdit->setEnabled(false);
    _LineEdit->actions().at(0)->setIcon(QIcon());
}
void CustomComboBox::DropDownList_itemPressed_slot(QListWidgetItem *item)
{
    if(item != nullptr)
        _LineEdit->setText(item->text());

    if(_DropDownList->isVisible())
        _DropDownList->hide();
}



