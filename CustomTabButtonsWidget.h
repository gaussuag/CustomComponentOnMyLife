#ifndef CUSTOMTABBUTTONSWIDGET_H
#define CUSTOMTABBUTTONSWIDGET_H

#include <QWidget>
#include <QList>
#include <QMap>

class QStackedWidget;
class QToolButton;
class QHBoxLayout;
class QButtonGroup;

class CustomTabButtonsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomTabButtonsWidget(QStringList & TabButtons,QWidget *parent = nullptr);
    ~CustomTabButtonsWidget() override;

    /*!
     * \brief insertWidget 在StackedWidget中插入Widget
     * \param index
     * \param w
     */
    void insertWidget(int index, QWidget *w);

    /*!
     * \brief checkedButton 根据Index对按钮进行check操作
     * \param index
     * \param Flag
     */
    void checkedButton(int index, bool Flag);

    /*!
     * \brief enableButton 根据Index对按钮进行使能设置
     * \param index
     * \param Flag
     */
    void enableButton(int index,bool Flag);

    /*!
     * \brief setCustomSwitchWidgetFlag 设置自定义切换Widget的标志位
     * \param flag
     */
    void setCustomSwitchWidgetFlag(bool flag);

    /*!
     * \brief setCurrentIndex 设置当前显示的控件界面Index
     * \param index
     */
    void setCurrentIndex(int index);


    /*!
     * \brief getTabButtonWidget 获取按钮标签控件指针
     * \return
     */
    QWidget *getTabButtonWidget() { return _tabButtonWidget;}
private slots:
    void clickedButtons_slot();
private:
    bool _customSwitchWidgetFlag{false};

    QButtonGroup *_buttonGroup{nullptr};
    QWidget *_tabButtonWidget{nullptr};
    QHBoxLayout *_buttonLayout{nullptr};
    QStackedWidget *_stackedWidget{nullptr};

    QMap<QToolButton*,int> _buttonsMap;

    void initWidget(const QStringList & buttons = QStringList());

signals:
    void clickedTabButton(int);
};

#endif // CUSTOMTABBUTTONSWIDGET_H
