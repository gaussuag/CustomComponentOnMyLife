#ifndef CUSTOMBUBBLEWIDGET_H
#define CUSTOMBUBBLEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QRect>
#include <QMargins>
#include <QDebug>
#include <QHBoxLayout>
#include <QTimer>
#include <QEvent>
#include <QStyleOption>



class CustomBubbleWidget : public QWidget
{
	Q_OBJECT
public:
	enum ArrowDirection {
		Top_Left = 1,
		Top_Right = 2,
		Top_Center = 3,
		Bottom_Left = 4,
		Bottom_Right = 5,
		Bottom_Center = 6,
		Left = 7,
		Right = 8
	};
	enum WidgetPopupDirection {
		PopupTop = 1,
		PopupBottom = 2,
		PopupLeft = 3,
		PopupRight = 4
	};
	enum BubbleWidgetPosition {
		OnTopCenter,
		OnBottomCenter
	};


	explicit CustomBubbleWidget(QWidget *parent = nullptr);
	explicit CustomBubbleWidget(CustomBubbleWidget::ArrowDirection arrowDirection, QWidget *parent = nullptr);
	~CustomBubbleWidget();

	/*!
	 * \brief showWithReferenceWidget 根据显示位置以及参考widget的位置和大小来显示这个控件
	 * \param widget 参考Widget
	 * \param Position 显示模式，[OnTopCenter]:在参考widget的上方居中显示; [OnBottomCenter]:在参考widget的下方居中显示。
	 */
	void showWithReferenceWidget(const QWidget * widget, CustomBubbleWidget::BubbleWidgetPosition Position);
	/*!
	 *
	 * \brief showWithReferenceWidget 根据显示位置以及参考位置和大小来显示这个控件
	 * \param GlobalPos 参考位置
	 * \param Size	参考大小
	 * \param Position 显示模式，[OnTopCenter]:在参考widget的上方居中显示; [OnBottomCenter]:在参考widget的下方居中显示。
	 */
	void showWithReferenceWidget(const QPoint & GlobalPos, const QSize & Size, CustomBubbleWidget::BubbleWidgetPosition Position);



	void setBackgroundColor(QColor color);
	void setCenterWidget(QWidget *widget);
	QWidget *getCenterWidget();
	void setArrowDirection(CustomBubbleWidget::ArrowDirection direction);

    void setRadiusArgument(int radius);
public slots:
	void show();


protected:
	bool eventFilter(QObject * watcher, QEvent * event);

	void paintEvent(QPaintEvent *event);

	void paintWidget(QPainter *painter);
	void DrawRoundRect(QPainter *painter);
	void DrawTriang(QPainter *painter);
private:
	CustomBubbleWidget::ArrowDirection _arrowDirection{ Left };
	CustomBubbleWidget::WidgetPopupDirection _PopupDirection = PopupBottom;

	QMargins _LayoutMargin;
	QHBoxLayout *_CenterWidgetLayout = nullptr;
	QWidget *_CenterWidget = nullptr;

	QColor _BackgroundColor = Qt::gray;

    int _radiusArg{8};
	int _triangleHeight;
	int _triangleWidth;
	int _triangleoffset;

	QRectF _Rect;
	QPointF _ArrowTopPoint;

	void setCenterWidgetLayoutContentsMargins();

	void initWidget();


	QRectF getDrawRect();
	QPolygon getTriangPolygon();
	void handleDrawParameter();

	/*!
	 * \brief handlePositionOnArrow 根据当前控件位置，以及方向枚举值，计算出实际绘制的geometry
	 */
	void handlePositionOnArrow();

	/*!
	* \brief getArrowPoint 根据参考位置，参考大小以及箭头方向计算出箭头坐标
	* \param Pos 参考位置
	* \param Size 参考大小
	* \param Direction 箭头方向枚举
	* \return 箭头坐标点
	*/
	QPoint getArrowPoint(const QPoint & Pos, const QSize & Size, CustomBubbleWidget::ArrowDirection Direction);

	/*!
	 * \brief checkDisplayGeometryVaild 根据箭头坐标以及箭头方向，计算出控件绘制后的geometry参数
	 * \param Pos 箭头坐标值
	 * \param ArrowDirection 箭头方向
	 * \return [true] 该箭头坐标以及箭头方向计算出的geometry合法;[false] geometry不合法，将箭头方向枚举对换位置
	 * \todo 还有一些箭头方向逻辑没有处理
	 */
	bool checkDisplayGeometryVaild(QPoint Pos, CustomBubbleWidget::ArrowDirection ArrowDirection);
	/*!
	 * \brief checkGeometry 判断传入的rect参数是否合法，不合法则将箭头枚举改变为反方向
	 * \param rect geometry的Rect值
	 * \param arrowDirection 箭头方向
	 * \return [true] 该箭头坐标以及箭头方向计算出的geometry合法;[false] geometry不合法，将箭头方向枚举对换位置
	 */
	bool checkGeometryVaild(const QRect & rect, CustomBubbleWidget::ArrowDirection arrowDirection);

signals:
	void widgetshow(QWidget *);
};

#endif // CUSTOMBUBBLEWIDGET_H
