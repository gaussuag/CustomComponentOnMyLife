#include "CustomLineEdit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent) :_Completer(nullptr), QLineEdit(parent)
{
    initWidget();
}


CustomLineEdit::~CustomLineEdit()
{
}

void CustomLineEdit::setAutoComplete(bool flag)
{
	if (flag)
	{
		_Completer = new QCompleter(this);
		_Completer->setCaseSensitivity(Qt::CaseInsensitive);
		this->setCompleter(_Completer);
	}
	else
		this->setCompleter(nullptr);
}

void CustomLineEdit::setCompleterStringList(QStringList StringList)
{
	if (_Completer != nullptr)
	{
		if (_Completer->model() != nullptr)
		{
			QStringListModel *StringListModel = qobject_cast<QStringListModel *>(_Completer->model());
			if (StringListModel != nullptr)
				StringListModel->setStringList(StringList);
		}
		else
		{
			QStringListModel *StringListModel = new QStringListModel(this);
			StringListModel->setStringList(StringList);
			_Completer->setModel(StringListModel);
		}
    }
}

void CustomLineEdit::setResizeToContentsFlag(bool flag)
{
    _isResizeToContentsFlag = flag;
}


bool CustomLineEdit::eventFilter(QObject * watched, QEvent * event)
{
	if (watched == this && QEvent::Paint)
    {
        if(_isResizeToContentsFlag)
        {
            int width = this->fontMetrics().width(this->text()) + 80;
            if(this->width() < width)
                setFixedWidth(width);
        }
	}
	return QLineEdit::eventFilter(watched, event);
}



void CustomLineEdit::initWidget()
{
	installEventFilter(this);
}

