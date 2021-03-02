#include "CustomCountButton.h"
#include <QDebug>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>



CustomCountButton::CustomCountButton(QWidget *parent) : QToolButton(parent)
{
    initWidget();
}

CustomCountButton::~CustomCountButton()
{

}

void CustomCountButton::setText(const QString &text)
{
    _textLabel->setText(text);
}

void CustomCountButton::setCountNumber(int count)
{
    _countLabel->setText(QString::number(count));
}

void CustomCountButton::setCountLabelSize(const QSize &size)
{
    _countLabel->setFixedSize(size);
    _layout->update();
}

void CustomCountButton::setCountLabelStyleSheet(const QString &StyleSheet)
{
    _countLabel->setStyleSheet(StyleSheet);
    _countLabel->adjustSize();
}

void CustomCountButton::setTextLabelStyleSheet(const QString &StyleSheet)
{
    _textLabel->setStyleSheet(StyleSheet);
    _textLabel->adjustSize();
}

void CustomCountButton::setLayoutMargin(int horizontalMargin)
{
    _layoutMargin = horizontalMargin;
    _layout->setContentsMargins(_layoutMargin,0,_layoutMargin,0);
    adjustSize();
}

void CustomCountButton::initWidget()
{
    if(_layoutOrientation == Qt::Orientation::Horizontal)
        _layout = new QHBoxLayout(this);
    else if(_layoutOrientation == Qt::Orientation::Vertical)
        _layout = new QVBoxLayout(this);
    _layout->setSpacing(12);
    _layout->setContentsMargins(_layoutMargin,0,_layoutMargin,0);

    _countLabel = new QLabel("0",this);
    _countLabel->setAlignment(Qt::AlignCenter);
    _countLabel->setObjectName("countLabel");

    _textLabel = new QLabel(this);
    _textLabel->setAlignment(Qt::AlignCenter);
    _textLabel->setObjectName("textLabel");

    _layout->addWidget(_countLabel);
    _layout->addWidget(_textLabel);
    _layout->setAlignment(_countLabel,Qt::AlignCenter);
    _layout->setAlignment(_textLabel,Qt::AlignCenter);

    connect(this,&CustomCountButton::pressed,this,[=](){
        _layout->setContentsMargins(_layoutMargin+2,2,_layoutMargin-2,0);
    });
    connect(this,&CustomCountButton::released,this,[=](){
        _layout->setContentsMargins(_layoutMargin,0,_layoutMargin,0);
    });
}
