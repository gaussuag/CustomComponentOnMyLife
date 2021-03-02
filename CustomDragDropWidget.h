#ifndef CUSTOMDRAGDROPWIDGET_H
#define CUSTOMDRAGDROPWIDGET_H

#include <QWidget>
#include <QUrl>
#include <QList>
#include <QPointer>

class QVBoxLayout;
class QStackedWidget;

/*!
 * \brief The CustomDragDropWidget class 处理系统文件管理器拖入文件的Qt界面的DragDrop事件的Widget\n
 *                                       可设置接收拖入的文件类型，提供设置一些拖拽界面效果以及处理拖入的文件链接后触发Drop事件信号。
 */
class CustomDragDropWidget : public QWidget
{
    Q_OBJECT
public:
    enum CustomDragDropActionEnum{
        noneAction = 0,
        rejectTypeAction = 1,
        acceptTypeAction = 2
    };
    explicit CustomDragDropWidget(QWidget *parent = nullptr);
    explicit CustomDragDropWidget(const QStringList & acceptMimeTypes,QWidget *parent = nullptr);
    ~CustomDragDropWidget();

    /*!
     * \brief acceptFlag 获取当前Drag Drop事件触发的Acion类型
     * \return
     */
    CustomDragDropWidget::CustomDragDropActionEnum acceptFlag() { return _isAcceptFlag;}

    /*!
     * \brief setAcceptMimeTypes 设置接受的文件类型列表
     * \param acceptMimeTypes
     */
    void setAcceptMimeTypes(const QStringList & acceptMimeTypes);

    /*!
     * \brief setAcceptAllType 设置接受所有文件类型
     * \param flag
     */
    void setAcceptAllType(bool flag);

    /*!
     * \brief setXoredMask 设置界面响应拖入事件的有效区域。
     * \param xoredWidgets 要剔除的控件指针，这个列表中的控件区域，将会从拖入事件有效区域中去除，携带拖入事件的光标移动到此控件区域时将不会触发拖入处理。
     */
    void setXoredMask(const QList<QWidget *> &xoredWidgets);
    /*!
     * \brief resizeWithParentWidget 设置拖入处理界面是否跟随父窗口的尺寸做自适应调整。
     * \param flag
     */
    void resizeWithParentWidget(bool flag);

    /*!
     * \brief setDragNormalTipsCenterWidget 设置正常模式下的拖入提示Widget，该Widget用于显示在此界面的中心对齐区域
     * \param widget
     */
    void setDragNormalTipsCenterWidget(QWidget *widget);

    /*!
     * \brief setDragEnterTipsCenterWidget 设置拖入事件接受时的提示Widget，该Widget用于显示在有拖入事件并且事件可接受的情况。
     * \param widget
     */
    void setDragEnterTipsCenterWidget(QWidget *widget);

    /*!
     * \brief setDragRejectTipsCenterWidget 设置拖入事件拒接时的提示Widget，该Widget用于显示在有拖入事件并且事件被拒绝的情况。
     * \param widget
     */
    void setDragRejectTipsCenterWidget(QWidget *widget);

    /*!
     * \brief setDragEnterBorderColor 设置拖入事件接受时边框样式颜色
     * \param color
     */
    void setDragEnterBorderColor(const QColor & color);


    /*!
     * \brief setDragEnterBackgroundColor 设置拖入事件接受时背景颜色
     * \param color
     */
    void setDragEnterBackgroundColor(const QColor & color);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
private:
    /// 预设的接受类型
    QStringList _acceptMimeTypes;
    /// 接受DragEvent的标志位
    CustomDragDropWidget::CustomDragDropActionEnum _isAcceptFlag{CustomDragDropActionEnum::noneAction};

    /// 默认接受所有事件标志位，不做类型检查
    bool _isAcceptAllTypeFlag{false};

    /// Style Parameter
    QColor _dragEnterBorderColor{Qt::gray};
    QColor _dragEnterAcceptBackgroundColor{Qt::transparent};
    QColor _dragEnterRejectBackgroundColor{Qt::transparent};

    /// 跟随父窗口自适应大小标志位
    bool _resizeWithParentWidget{false};
    /// 剔除事件响应区域控件列表
    QList<QWidget *> _xoredWidgets;

    QVBoxLayout *_mainLayout{nullptr};
    QStackedWidget *_stackedWidget{nullptr};
    QPointer<QWidget> _normalTipsWidget{nullptr};
    QPointer<QWidget> _enterTipsWidget{nullptr};
    QPointer<QWidget> _rejectTipsWidget{nullptr};

    void initDefaultStyle();
    void initWidget();
    void initConnect();

    /// 根据本地文件路径以及预设的接受类型检查该文件路径是否可接受
    bool urlIsAcceptMimeType(const QString &url);

    /// 检查链接列表是否全部都有效
    bool validationMimeTypeByUrls(const QList<QUrl> &Urls);

    /// 处理Urls中的链接，二次检查是否为预设的接受类型，是则触发acceptDropUrls信号
    void handleDropUrls(const QList<QUrl> &Urls);

    /// 计算并剔除预设值的控件区域，并设置有效遮罩Region
    void handleXoredWidgetMask();

    /// 切换显示的Tips界面
    void switchTipsWidget();

    void updateAcceptFlag(CustomDragDropActionEnum actionEnum);
private slots:
    void acceptFlagChanged_slot();
signals:
    /// 拖入事件接受标志位变更信号
    void acceptFlagChanged();

    /// 拖入的事件落在有效区域并且类型检查有效时触发此信号，将会传递拖入获取到的本地文件路径列表
    void acceptDropLocalPaths(const QStringList &);

    void dragDropWidgetDragEventLeave();

    void dragDropValidationRegionChanged(const QRegion & region);
};

#endif // CUSTOMDRAGDROPWIDGET_H
