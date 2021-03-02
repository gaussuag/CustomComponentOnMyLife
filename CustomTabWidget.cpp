#include "CustomTabWidget.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QButtonGroup>

#include "CustomToolButton.h"


CustomTabWidget::CustomTabWidget(QStringList &TabButtons, QWidget *parent) : QWidget(parent)
{
    initWidget(TabButtons);
}

CustomTabWidget::~CustomTabWidget()
{

}

void CustomTabWidget::setCurrentIndex(int index)
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

void CustomTabWidget::insertWidget(int index, QWidget *w)
{
    if(_stackedWidget)
        _stackedWidget->insertWidget(index,w);
}

void CustomTabWidget::checkedButton(int index,bool Flag)
{
    if(_buttonsMap.values().contains(index))
        _buttonsMap.key(index)->setChecked(Flag);
}

void CustomTabWidget::enableButton(int index, bool Flag)
{
    if(_buttonsMap.values().contains(index))
        _buttonsMap.key(index)->setEnabled(Flag);
}

void CustomTabWidget::setCustomSwitchWidgetFlag(bool flag)
{
    _customSwitchWidgetFlag = flag;
    _buttonGroup->setExclusive(false);
}

void CustomTabWidget::initWidget(const QStringList & Buttons)
{
    setStyleSheet("QWidget{background-color:transparent;}");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

   _buttonLayout = new QHBoxLayout();
   _buttonLayout->setMargin(10);
   _buttonLayout->setSpacing(30);
   _buttonLayout->addStretch();

   _buttonGroup = new QButtonGroup(this);
   _buttonGroup->setExclusive(true);

   auto initTabButton = [&](const QString & buttonText){
     CustomToolButton *bt = new CustomToolButton(buttonText,this);
     bt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
     bt->setDefaultStyleSheet();
     bt->setCheckable(true);
     connect(bt,&CustomToolButton::clicked,this,&CustomTabWidget::clickedButtons_slot);

     return bt;
   };

   for(int i=0;i<Buttons.size();++i)
   {
       auto Bt = initTabButton(Buttons.at(i));
       _buttonsMap.insert(Bt,i);
       _buttonLayout->addWidget(Bt,2);
       _buttonGroup->addButton(Bt);
   }
    _buttonLayout->addStretch();

   _stackedWidget = new QStackedWidget(this);

   mainLayout->addLayout(_buttonLayout);
   mainLayout->addWidget(_stackedWidget);
}


void CustomTabWidget::clickedButtons_slot()
{
    auto Bt = qobject_cast<CustomToolButton *>(sender());

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
