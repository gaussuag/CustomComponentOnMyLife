#include "CustomButtonGroup.h"


CustomButtonGroup::CustomButtonGroup(QObject *parent) : QObject(parent)
{
}

CustomButtonGroup::~CustomButtonGroup()
{

}

void CustomButtonGroup::addButton(QAbstractButton * button)
{
    if(!button->isCheckable())
        button->setCheckable(true);

    connect(button,&QAbstractButton::clicked,this,&CustomButtonGroup::button_clickeded_slot);
}

void CustomButtonGroup::removeButton(QAbstractButton * button)
{
    disconnect(button,&QAbstractButton::clicked,this,&CustomButtonGroup::button_clickeded_slot);
    if(_checkedButton == button)
        _checkedButton = nullptr;
}

void CustomButtonGroup::button_clickeded_slot(bool flag)
{
    auto button = qobject_cast<QAbstractButton*>(sender());

    if(!button)
        return;
    /// 检查当前是否有其他按钮选中，有则将其弹起
    if(flag)
    {
        if(_checkedButton)
            _checkedButton->setChecked(false);
        _checkedButton = button;
    }
    /// 当前按钮是弹起操作，意味着这个组内没有按钮处于checked状态，将记录清空
    else
        _checkedButton = nullptr;
}
