#include "CustomTabButtonsWidget.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QButtonGroup>

#include <QToolButton>

CustomTabButtonsWidget::CustomTabButtonsWidget(QStringList &TabButtons, QWidget *parent) : QWidget(parent)
{
    initWidget(TabButtons);
}

CustomTabButtonsWidget::~CustomTabButtonsWidget()
{

}



void CustomTabButtonsWidget::setCurrentIndex(int index)
{
    _stackedWidget->setCurrentIndex(index);

    for(const auto & Bt : _buttonsMap.keys())
    {
        if(_buttonsMap.value(Bt) == index)
            Bt->setChecked(true);
        else
            Bt->setChecked(false);
    }
}

void CustomTabButtonsWidget::insertWidget(int index, QWidget *w)
{
    if(_stackedWidget)
        _stackedWidget->insertWidget(index,w);
}

void CustomTabButtonsWidget::checkedButton(int index,bool Flag)
{
    if(_buttonsMap.values().contains(index))
        _buttonsMap.key(index)->setChecked(Flag);
}

void CustomTabButtonsWidget::enableButton(int index, bool Flag)
{
    if(_buttonsMap.values().contains(index))
        _buttonsMap.key(index)->setEnabled(Flag);
}

void CustomTabButtonsWidget::setCustomSwitchWidgetFlag(bool flag)
{
    _customSwitchWidgetFlag = flag;
    _buttonGroup->setExclusive(false);
}

void CustomTabButtonsWidget::initWidget(const QStringList & buttons)
{
    setStyleSheet("QWidget{background-color:transparent;}");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    _tabButtonWidget = new QWidget(this);
    _tabButtonWidget->setFixedHeight(28);
    _tabButtonWidget->setObjectName("tabButtonWidget");
    _tabButtonWidget->setStyleSheet("QWidget#tabButtonWidget{background-color:#4a4a4a;}");

    _buttonLayout = new QHBoxLayout(_tabButtonWidget);
    _buttonLayout->setContentsMargins(35,0,35,0);
    _buttonLayout->setSpacing(48);

    _buttonGroup = new QButtonGroup(this);
    _buttonGroup->setExclusive(true);

    auto initTabButton = [&](const QString & buttonText){
        QToolButton *bt = new QToolButton(this);
        bt->setText(buttonText);
        bt->setFixedHeight(28);
        bt->setStyleSheet("QToolButton{background-color:transparent;"
                          "border:0px;"
                          "color:#b9b9b9;"
                          "font-size:16px;}"
                          "QToolButton::checked{background-color:transparent;"
                          "border-top:0px;border-bottom:3px solid white;"
                          "border-left:0px;border-right:0px;"
                          "color:white;}");
        bt->setCheckable(true);
        connect(bt,&QToolButton::clicked,this,&CustomTabButtonsWidget::clickedButtons_slot);

        return bt;
    };

    for(int i=0;i<buttons.size();++i)
    {
        auto Bt = initTabButton(buttons.at(i));
        _buttonsMap.insert(Bt,i);
        _buttonLayout->addWidget(Bt,2);
        _buttonGroup->addButton(Bt);
    }
    _buttonLayout->addStretch(0);

    _stackedWidget = new QStackedWidget(this);
//    _stackedWidget->setObjectName("StackedWidget");
//    _stackedWidget->setStyleSheet("QWidget#StackedWidget{"
//                              "background-color:#3e3e3e;}");

    mainLayout->addWidget(_tabButtonWidget);
    mainLayout->addWidget(_stackedWidget);
}


void CustomTabButtonsWidget::clickedButtons_slot()
{
    auto Bt = qobject_cast<QToolButton *>(sender());

    if(_buttonsMap.contains(Bt))
    {
        auto index = _buttonsMap.value(Bt);

        ///如果为自定义切换界面类型，则不触发切换界面操作。
        if(_customSwitchWidgetFlag)
        {
            Bt->setChecked(false);
            emit(clickedTabButton(index));
            return;
        }

        if(index < _stackedWidget->count())
            _stackedWidget->setCurrentIndex(index);
    }
}
