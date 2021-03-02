#include "CustomButtonBox.h"

#include <QHBoxLayout>
#include <QButtonGroup>

CustomButtonBox::CustomButtonBox(QWidget *parent) : QWidget(parent)
{
    initBox();
}

CustomButtonBox::~CustomButtonBox()
{

}

QPushButton *CustomButtonBox::appendButton(const QString &text, bool isChecked)
{
    QPushButton *button = new QPushButton(text,this);
    _buttonGroup->addButton(button);

    button->setFixedHeight(height());
    button->setCheckable(true);
    button->setChecked(isChecked);

    button->setStyleSheet("QPushButton{"
                          "background-color:#323232;"
                          "color:#B9B9B9;"
                          "border-radius:4px;"
                          "font-size:14px;}"
                          "QPushButton::checked{"
                          "background-color:#4A4A4A;"
                          "color:#E3E3E3;}");

    _buttonLayout->addWidget(button);
    _buttonLayout->update();
    adjustSize();

    _buttons.append(button);

    return button;
}

void CustomButtonBox::setButtonStyleSheet(const QString &stylesheet)
{
    for(auto & bt : _buttons)
        bt->setStyleSheet(stylesheet);
}

void CustomButtonBox::initBox()
{
    setAttribute(Qt::WA_StyledBackground);

    _buttonLayout = new QHBoxLayout(this);
    _buttonLayout->setMargin(0);
    _buttonLayout->setSpacing(0);

    _buttonGroup = new QButtonGroup(this);
    _buttonGroup->setExclusive(true);
}
