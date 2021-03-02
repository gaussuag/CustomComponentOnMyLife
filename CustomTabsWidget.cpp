#include "CustomTabsWidget.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QButtonGroup>

#include "CIconManage.h"

CustomTabsWidget::CustomTabsWidget(QWidget *parent) : QWidget(parent)
{
    initWidget();
}

CustomTabsWidget::~CustomTabsWidget()
{

}

void CustomTabsWidget::setCurrentWidget(QWidget *widget)
{
    /// 检查此Widget是否已经存在于记录中
    if(_tabButton_WidgetMap.values().contains(widget))
        _stackedWidget->setCurrentWidget(widget);
}

bool CustomTabsWidget::appendWidget(const QString &widgetTitle, QWidget *w)
{
    /// 检查是否超出预定的标签页最大数
    if(_stackedWidget->count() >= _maximumTabCount)
        return false;

    TabButton *tabButton = initTabButton(widgetTitle,_tabsButtonLayout->count());

    _stackedWidget->addWidget(w);
    _tabButton_WidgetMap.insert(tabButton,w);

    emit(tabButton->toggled(true));

    return true;
}

void CustomTabsWidget::setMaximumTabCount(int tabCount)
{
    _maximumTabCount = tabCount;
}

int CustomTabsWidget::count()
{
    return _stackedWidget->count();
}

TabButton* CustomTabsWidget::initTabButton(const QString & widgetTitle,int index)
{
    TabButton *tabButton = new TabButton(this);
    tabButton->setFixedSize(_tabButtonSize);
    tabButton->setAutoExclusive(true);
    tabButton->setChecked(true);
    _tabsButtonLayout->insertWidget(index,tabButton);

    connect(tabButton,&TabButton::requestClose,this,&CustomTabsWidget::tabButton_requestClose_slot);
    connect(tabButton,&QToolButton::toggled,this,&CustomTabsWidget::tabButton_toggled_slot);

    /// 自动缩略显示
    tabButton->setText(fontMetrics().elidedText(widgetTitle,Qt::TextElideMode::ElideRight,tabButton->width() - 10));
    tabButton->setToolTip(widgetTitle);

    return tabButton;
}

/// 移除TabButton以及按钮对应的Widget，会析构对象
void CustomTabsWidget::tabButton_requestClose_slot()
{
    auto tabButton = qobject_cast<TabButton*>(sender());
    if(tabButton)
    {
        if(_tabButton_WidgetMap.contains(tabButton))
        {
            auto widget = _tabButton_WidgetMap.take(tabButton);
            _stackedWidget->removeWidget(widget);
            _tabsButtonLayout->removeWidget(tabButton);

            delete tabButton;
            delete widget;
        }
    }
}

/// TabButton点击时的切换显示的Widet处理函数
void CustomTabsWidget::tabButton_toggled_slot(bool flag)
{
    auto tabButton = qobject_cast<TabButton*>(sender());
    if(tabButton && flag)
    {
        if(_tabButton_WidgetMap.contains(tabButton))
            _stackedWidget->setCurrentWidget(_tabButton_WidgetMap.value(tabButton));
    }
}



void CustomTabsWidget::initWidget()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setObjectName("CustomTabsWidget");
    setStyleSheet("QWidget#CustomTabsWidget{background-color:transparent;}");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(0);
    titleLayout->setMargin(0);

   _tabsButtonLayout = new QHBoxLayout();
   _tabsButtonLayout->setMargin(0);
   _tabsButtonLayout->setSpacing(0);

   _titleButtonWidget = new QWidget(this);
   _titleButtonWidget->setFixedSize(144,36);
   QHBoxLayout *titleButtonLayout = new QHBoxLayout(_titleButtonWidget);
   titleButtonLayout->setSpacing(0);
   titleButtonLayout->setMargin(0);

   QToolButton *minimizeButton = new QToolButton(_titleButtonWidget);
   minimizeButton->setStyleSheet("QToolButton{"
                                 "background-color:transparent;"
                                 "border:0px;}"
                                 "QToolButton::hover{"
                                 "background-color:#323232;}");
   minimizeButton->setFixedSize(72,36);
   minimizeButton->setIconSize(QSize(15,21));
   minimizeButton->setIcon(QIcon(DefaultIcon->icon_Svg_minus.getIconPixmap(minimizeButton->iconSize(),"#ffffff")));
   connect(minimizeButton,&QToolButton::clicked,this,&CustomTabsWidget::requestMinimize);

   QToolButton *closeButton = new QToolButton(_titleButtonWidget);
   closeButton->setStyleSheet("QToolButton{"
                                 "background-color:transparent;"
                                 "border:0px;}"
                                 "QToolButton::hover{"
                                 "background-color:#E34850;}");
   closeButton->setFixedSize(72,36);
   closeButton->setIconSize(QSize(13,21));
   closeButton->setIcon(QIcon(DefaultIcon->icon_Svg_times.getIconPixmap(closeButton->iconSize(),"#ffffff")));
   connect(closeButton,&QToolButton::clicked,this,&CustomTabsWidget::requestClose);

   titleButtonLayout->addWidget(minimizeButton);
   titleButtonLayout->addWidget(closeButton);

   titleLayout->addLayout(_tabsButtonLayout);
   titleLayout->addStretch(2);
   titleLayout->addWidget(_titleButtonWidget);


   _stackedWidget = new QStackedWidget(this);
   connect(_stackedWidget,&QStackedWidget::currentChanged,
           this,&CustomTabsWidget::stackedWidget_currentChanged_slot);

   mainLayout->addLayout(titleLayout);
   mainLayout->addWidget(_stackedWidget);
}

/// 当stackedWidget显示的Widget发生变化时，会进入此函数检查Widget对应的TabButton是否设置为Checked
void CustomTabsWidget::stackedWidget_currentChanged_slot(int index)
{
    auto widget = _stackedWidget->widget(index);
    if(widget)
    {
        if(_tabButton_WidgetMap.values().contains(widget))
        {
            auto tabButton = _tabButton_WidgetMap.key(widget);
            if(!tabButton->isChecked())
                tabButton->setChecked(true);
        }
    }
}

TabButton::TabButton(QWidget *parent) : QToolButton(parent)
{
    initWidget();
}

TabButton::~TabButton()
{

}

void TabButton::initWidget()
{
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("QToolButton{"
                  "background-color:transparent;"
                  "color:#ffffff;"
                  "font-size:14px;"
                  "border:0px;}"
                  "QToolButton::checked{"
                  "background-color:#2F2F2F;}");

    setCheckable(true);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(8);
    mainLayout->setSpacing(0);

    _closeButton = new QToolButton(this);
    _closeButton->setFixedSize(20,20);
    _closeButton->setStyleSheet("QToolButton{"
                                 "background-color:transparent;"
                                 "border:0px;}"
                                 "QToolButton::hover{"
                                 "background-color:#E34850;}");
    _closeButton->setIconSize(QSize(8,13));
    _closeButton->setIcon(QIcon(DefaultIcon->icon_Svg_times.getIconPixmap(_closeButton->iconSize(),"#ffffff")));

    connect(_closeButton,&QToolButton::clicked,this,&TabButton::requestClose);

    mainLayout->addStretch();
    mainLayout->addWidget(_closeButton);
}
