#include "CustomCountTextEditWidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>

CustomCountTextEditWidget::CustomCountTextEditWidget(QWidget *parent) : QWidget(parent)
{
    initWidget();
}

CustomCountTextEditWidget::CustomCountTextEditWidget(int maxCount, const QString &title, QWidget *parent)
    : QWidget(parent),_maxCount(maxCount)
{
    initWidget(title);
}

CustomCountTextEditWidget::~CustomCountTextEditWidget()
{

}

QString CustomCountTextEditWidget::getText()
{
    QString text;
    if(_textEdit)
        text = _textEdit->toPlainText();

    return text;
}

void CustomCountTextEditWidget::setText(const QString & text)
{
    if(_textEdit)
        _textEdit->setText(text);
}

void CustomCountTextEditWidget::setTitle(const QString & title)
{
    _titleLabel->setText(title);
    _titleLabel->adjustSize();
}

void CustomCountTextEditWidget::setMaxCount(int maxCount)
{
    _maxCount = maxCount;
    _maxCountLabel->setText(QString("/%1").arg(_maxCount));
    _maxCountLabel->adjustSize();
}

void CustomCountTextEditWidget::initWidget(const QString & title)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);

    _titleLabel = new QLabel(title,this);
    _titleLabel->setStyleSheet("QLabel{color:white;font-size:14px;}");

    _currentCountLabel = new QLabel("0",this);
    _currentCountLabel->setStyleSheet("QLabel{color:white;font-size:14px;}");

    _maxCountLabel = new QLabel(QString("/%1").arg(_maxCount),this);
    _maxCountLabel->setStyleSheet("QLabel{color:white;font-size:14px;}");

    titleLayout->addWidget(_titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(_currentCountLabel);
    titleLayout->addWidget(_maxCountLabel);


    _textEdit = new QTextEdit(this);
    _textEdit->setAcceptRichText(false);
    connect(_textEdit,&QTextEdit::textChanged,this,[=](){
        int currentCount = _textEdit->toPlainText().count();

        if(currentCount > _maxCount)
        {
            QTextCursor cursor = _textEdit->textCursor();
            if(cursor.position() != _maxCount)
                cursor.setPosition(_maxCount);

            cursor.movePosition(QTextCursor::End,QTextCursor::MoveMode::KeepAnchor);

            if(cursor.hasSelection())
                cursor.removeSelectedText();

            _textEdit->setTextCursor(cursor);
        }

        currentCount = _textEdit->toPlainText().count();
        _currentCountLabel->setText(QString("%1").arg(currentCount));
    });

    mainLayout->addLayout(titleLayout);
    mainLayout->addWidget(_textEdit);
}

