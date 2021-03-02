#ifndef CUSTOMTABSWIDGET_H
#define CUSTOMTABSWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QList>
#include <QMap>

class QStackedWidget;
class QHBoxLayout;
class QButtonGroup;

class TabButton;

class CustomTabsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomTabsWidget(QWidget *parent = nullptr);
    ~CustomTabsWidget() override;

    /*!
     * \brief setCurrentWidget 设置当前显示的Widget
     * \param widget
     */
    void setCurrentWidget(QWidget * widget);

    /*!
     * \brief appendWidget 添加一个Widget到Tabs窗口中
     */
    bool appendWidget(const QString & widgetTitle,QWidget *w);

    /*!
     * \brief setMaximumTabCount 设置可添加的标签页的最大数
     * \param tabCount
     */
    void setMaximumTabCount(int tabCount);

    int count();
private:
    QHBoxLayout *_tabsButtonLayout{nullptr};
    QWidget *_titleButtonWidget{nullptr};
    QStackedWidget *_stackedWidget{nullptr};

    /// Title Bar Parameter
    QSize _tabButtonSize{QSize(200,36)};
    int _maximumTabCount{777};

    /// Tab & Widget Map
    QMap<TabButton*,QWidget *> _tabButton_WidgetMap;

    void initWidget();

    TabButton *initTabButton(const QString &widgetTitle, int index);
signals:
    void clickedTabButton(int);

    void requestMinimize();
    void requestClose();
private slots:
    void tabButton_requestClose_slot();
    void tabButton_toggled_slot(bool flag);

    /*!
     * \brief stackedWidget_currentChanged_slot StackedWidget显示的Widget与对应的TabButton状态同步处理函数
     * \param index
     */
    void stackedWidget_currentChanged_slot(int index);
};


/*!
 * \brief The TabButton class 标签页的标签按钮
 */
class TabButton : public QToolButton
{
    Q_OBJECT
public:
    explicit TabButton(QWidget * parent = nullptr);
    ~TabButton();

    /*!
     * \brief closeButton 标签按钮上的关闭按钮对象
     * \return
     */
    QToolButton *closeButton(){ return _closeButton;}

private:
    QToolButton *_closeButton{nullptr};

    void initWidget();
signals:
    void requestClose();
};

#endif // CUSTOMTABSWIDGET_H
