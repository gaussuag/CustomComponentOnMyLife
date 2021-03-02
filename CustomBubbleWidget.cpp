#include "CustomBubbleWidget.h"
#include <QApplication>
#include <QDesktopWidget>


CustomBubbleWidget::CustomBubbleWidget(QWidget *parent)
	:QWidget(parent)
{
	initWidget();
}
CustomBubbleWidget::CustomBubbleWidget(ArrowDirection arrowDirection, QWidget *parent)
	: QWidget(parent), _arrowDirection(arrowDirection)
{
	initWidget();
	hide();
}
CustomBubbleWidget::~CustomBubbleWidget()
{
}

void CustomBubbleWidget::showWithReferenceWidget(const QWidget *widget, CustomBubbleWidget::BubbleWidgetPosition Position)
{
	auto Pos = (widget->parentWidget() == nullptr) ? widget->mapToGlobal(widget->pos()) : widget->parentWidget()->mapToGlobal(widget->pos());
	return showWithReferenceWidget(Pos, widget->size(), Position);
}

void CustomBubbleWidget::showWithReferenceWidget(const QPoint & Pos, const QSize & Size, CustomBubbleWidget::BubbleWidgetPosition Position)
{
    Q_UNUSED(Position)
	//根据参考起点和参考大小，以及箭头方向，计算出一个箭头位置坐标
	QPoint newpos = getArrowPoint(Pos,Size,_arrowDirection);
	//根据箭头位置坐标，计算出新的geometry的矩形值，并检查是否超范围，若超范围则将箭头方向取反，并重新计算一次箭头的位置
	if (!checkDisplayGeometryVaild(newpos, _arrowDirection))
		newpos = getArrowPoint(Pos,Size,_arrowDirection);
	//移动并绘制控件
	move(newpos);
	show();
}

QPoint CustomBubbleWidget::getArrowPoint(const QPoint & Pos,const QSize & Size,CustomBubbleWidget::ArrowDirection Direction)
{
	QPoint pos;
	switch (Direction)
	{
	case Top_Left:
	case Top_Right:
	case Top_Center:
		pos.setX(Size.width() / 2 + Pos.x());
		pos.setY(Pos.y() + Size.height());
		return pos;
	case Bottom_Left:
	case Bottom_Right:
	case Bottom_Center:
		pos.setX(Size.width() / 2 + Pos.x());
		pos.setY(Pos.y());
		return pos;
	case Left:
		break;
	case Right:
		break;
	}
	return pos;
}
bool CustomBubbleWidget::checkDisplayGeometryVaild(QPoint Pos, CustomBubbleWidget::ArrowDirection arrowDirection)
{
	//Advance HandleParameter
	handleDrawParameter();
	getTriangPolygon();
	QPoint currentPos = Pos;
	switch (arrowDirection)
	{
	case Top_Left:
	case Top_Right:
	case Top_Center:
		currentPos.setX(currentPos.x() - _ArrowTopPoint.x());
		currentPos.setY(currentPos.y() + _ArrowTopPoint.y());
		break;
	case Bottom_Left:
	case Bottom_Right:
	case Bottom_Center:
		currentPos.setX(currentPos.x() - _ArrowTopPoint.x());
		currentPos.setY(currentPos.y() - _ArrowTopPoint.y());
		break;
	case Left:
		currentPos.setY(currentPos.y() - _ArrowTopPoint.y());
		break;
	case Right:
		currentPos.setX(currentPos.x() - _ArrowTopPoint.x());
		currentPos.setY(currentPos.y() - _ArrowTopPoint.y());
		break;
	}
	QRect rect = QRect(currentPos.x(), currentPos.y(), this->size().width(), this->size().height());
	return checkGeometryVaild(rect, arrowDirection);
}
bool CustomBubbleWidget::checkGeometryVaild(const QRect & rect, CustomBubbleWidget::ArrowDirection arrowDirection)
{
	switch (arrowDirection)
	{
	case Top_Left:
	case Top_Right:
	case Top_Center:
		break;
	case Bottom_Left:
	case Bottom_Right:
	case Bottom_Center:
		if (rect.y() < 0)
		{
			_arrowDirection = Top_Center;
			return false;
		}
		break;
	case Left:
		//currentPos.setY(currentPos.y() - _ArrowTopPoint.y());
		break;
	case Right:
		//currentPos.setX(currentPos.x() - _ArrowTopPoint.x());
	//	currentPos.setY(currentPos.y() - _ArrowTopPoint.y());
		break;
	}

	return true;
}

bool CustomBubbleWidget::eventFilter(QObject *watcher, QEvent *event)
{
	if (watcher == _CenterWidget)
	{
		if (event->type() == QEvent::Resize)
			setCenterWidgetLayoutContentsMargins();
    }

	return QWidget::eventFilter(watcher, event);
}

void CustomBubbleWidget::initWidget()
{
    installEventFilter(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::WindowStaysOnTopHint);

	setAttribute(Qt::WA_TranslucentBackground);

	_CenterWidgetLayout = new QHBoxLayout(this);
	_CenterWidgetLayout->setSpacing(0);
}

void CustomBubbleWidget::setCenterWidget(QWidget *widget)
{
	_CenterWidget = widget;

	_CenterWidgetLayout->addWidget(widget);
	//Strong Update Geometry
    show();
    hide();

	setCenterWidgetLayoutContentsMargins();
}
QWidget * CustomBubbleWidget::getCenterWidget()
{
	return _CenterWidget;
}
void CustomBubbleWidget::setCenterWidgetLayoutContentsMargins()
{
    ///提前计算绘制所需的参数，用于边界调整
	handleDrawParameter();
    qreal FilletRadius = ((this->width() < this->height()) ? this->width() : this->height()) / 8;

	switch (_arrowDirection) {
	case Top_Left:
	case Top_Right:
	case Top_Center:
		_LayoutMargin = QMargins(FilletRadius, FilletRadius + _triangleHeight, FilletRadius, FilletRadius);
		break;
	case Bottom_Left:
	case Bottom_Right:
	case Bottom_Center:
		_LayoutMargin = QMargins(FilletRadius, FilletRadius, FilletRadius, FilletRadius + _triangleHeight);
		break;
	case Left:
		_LayoutMargin = QMargins(FilletRadius + _triangleHeight, FilletRadius, FilletRadius, FilletRadius);
		break;
	case Right:
		_LayoutMargin = QMargins(FilletRadius, FilletRadius, FilletRadius + _triangleHeight, FilletRadius);
		break;
	default:
		break;
	}
	if (_CenterWidgetLayout != nullptr)
		_CenterWidgetLayout->setContentsMargins(_LayoutMargin);

	QPoint curPos = pos();
	QSize newSize = _CenterWidgetLayout->sizeHint();
	setGeometry(curPos.x(), curPos.y(), newSize.width(), newSize.height());
}

void CustomBubbleWidget::setArrowDirection(CustomBubbleWidget::ArrowDirection direction)
{
	_arrowDirection = direction;
	setCenterWidgetLayoutContentsMargins();
    this->update();
}

void CustomBubbleWidget::setRadiusArgument(int radius)
{
    _radiusArg = radius;
    update();
}



void CustomBubbleWidget::setBackgroundColor(QColor color)
{
	_BackgroundColor = color;
	this->update();
}

void CustomBubbleWidget::show()
{
	handlePositionOnArrow();
	QWidget::show();
}


void CustomBubbleWidget::handlePositionOnArrow()
{
	//Advance HandleParameter
	handleDrawParameter();
	getTriangPolygon();

	QPoint currentPos = this->pos();
	switch (_arrowDirection)
	{
	case Top_Left:
	case Top_Right:
	case Top_Center:
		currentPos.setX(currentPos.x() - _ArrowTopPoint.x());
		currentPos.setY(currentPos.y() + _ArrowTopPoint.y());
		break;
	case Bottom_Left:
	case Bottom_Right:
	case Bottom_Center:
		currentPos.setX(currentPos.x() - _ArrowTopPoint.x());
		currentPos.setY(currentPos.y() - _ArrowTopPoint.y());
		break;
	case Left:
		currentPos.setY(currentPos.y() - _ArrowTopPoint.y());
		break;
	case Right:
		currentPos.setX(currentPos.x() - _ArrowTopPoint.x());
		currentPos.setY(currentPos.y() - _ArrowTopPoint.y());
		break;
	}
	this->setGeometry(currentPos.x(), currentPos.y(), this->size().width(), this->size().height());
}

void CustomBubbleWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	paintWidget(&painter);

}
void CustomBubbleWidget::paintWidget(QPainter *painter)
{
	painter->setRenderHint(QPainter::Antialiasing, true);

	handleDrawParameter();

	//Draw RoundRect
	DrawRoundRect(painter);

	//DrawTriang
	DrawTriang(painter);
}
void CustomBubbleWidget::DrawRoundRect(QPainter *painter)
{
	_Rect = getDrawRect();
	painter->setPen(Qt::NoPen);
	painter->setBrush(_BackgroundColor);

	qreal referenceArg = (_Rect.width() < _Rect.height()) ? _Rect.width() : _Rect.height();
    painter->drawRoundedRect(_Rect, referenceArg / _radiusArg, referenceArg / _radiusArg);
}
void CustomBubbleWidget::DrawTriang(QPainter *painter)
{
	QPolygon TriangPolygon = getTriangPolygon();
	painter->drawPolygon(TriangPolygon);
}

void CustomBubbleWidget::handleDrawParameter()
{
    //handleTriangParameter
    ///根据当前的矩形参数以及三角形的方向，计算三角形的参数
	QRect widgetRect = this->rect();
	qreal referenceArg;
	if ((_arrowDirection == Left) || (_arrowDirection == Right))
	{
		referenceArg = (widgetRect.width() <= widgetRect.height()) ? widgetRect.width() : widgetRect.height();
		_triangleHeight = referenceArg / 4;
		_triangleWidth = referenceArg / 3;
	}
	else
	{
		referenceArg = widgetRect.width();
		_triangleoffset = referenceArg / 6;
		_triangleHeight = referenceArg / 30;
		_triangleWidth = referenceArg / 15;
	}
}

QRectF CustomBubbleWidget::getDrawRect()
{
	QRect widgetRect = this->rect();
	switch (_arrowDirection) {
	case Top_Left:
	case Top_Right:
	case Top_Center:
		return QRectF(0, _triangleHeight, widgetRect.width() - 1, widgetRect.height() - _triangleHeight - 1);
	case Bottom_Left:
	case Bottom_Right:
	case Bottom_Center:
		return QRectF(0, 0, widgetRect.width() - 1, widgetRect.height() - _triangleHeight - 1);
	case Left:
		return QRectF(_triangleHeight, 0, widgetRect.width() - _triangleHeight - 1, widgetRect.height() - 1);
	case Right:
		return QRectF(0, 0, widgetRect.width() - _triangleHeight - 1, widgetRect.height() - 1);
	default:
		return QRectF();
	}
}
QPolygon CustomBubbleWidget::getTriangPolygon()
{
	QRectF rect = getDrawRect();
	QPolygon polygon;
	switch (_arrowDirection)
	{
	case Top_Left:
		polygon << QPoint(_triangleoffset, _triangleHeight);
		polygon << QPoint(_triangleoffset + _triangleWidth / 2, 0);
		_ArrowTopPoint = QPoint(_triangleoffset + _triangleWidth / 2, 0);
		polygon << QPoint(_triangleoffset + _triangleWidth, _triangleHeight);
		break;
	case Top_Right:
		polygon << QPoint(rect.width() - _triangleWidth - _triangleoffset, _triangleHeight);
		polygon << QPoint(rect.width() - _triangleWidth / 2 - _triangleoffset, 0);
		_ArrowTopPoint = QPoint(rect.width() - _triangleWidth / 2 - _triangleoffset, 0);
		polygon << QPoint(rect.width() - _triangleoffset, _triangleHeight);
		break;
	case Top_Center:
		polygon << QPoint(rect.width() / 2 - _triangleWidth / 2, _triangleHeight);
		polygon << QPoint(rect.width() / 2, 0);
		_ArrowTopPoint = QPoint(rect.width() / 2, 0);
		polygon << QPoint(rect.width() / 2 + _triangleWidth / 2, _triangleHeight);
		break;
	case Bottom_Left:
		polygon << QPoint(_triangleoffset, rect.height());
		polygon << QPoint(_triangleoffset + _triangleWidth / 2, rect.height() + _triangleHeight);
		_ArrowTopPoint = QPoint(_triangleoffset + _triangleWidth / 2, rect.height() + _triangleHeight);
		polygon << QPoint(_triangleoffset + _triangleWidth, rect.height());
		break;
	case Bottom_Right:
		polygon << QPoint(rect.width() - _triangleWidth - _triangleoffset, rect.height());
		polygon << QPoint(rect.width() - _triangleWidth / 2 - _triangleoffset, rect.height() + _triangleHeight);
		_ArrowTopPoint = QPoint(rect.width() - _triangleWidth / 2 - _triangleoffset, rect.height() + _triangleHeight);
		polygon << QPoint(rect.width() - _triangleoffset, rect.height());
		break;
	case Bottom_Center:
		polygon << QPoint(rect.width() / 2 - _triangleWidth / 2, rect.height());
		polygon << QPoint(rect.width() / 2, rect.height() + _triangleHeight);
		_ArrowTopPoint = QPoint(rect.width() / 2, rect.height() + _triangleHeight);
		polygon << QPoint(rect.width() / 2 + _triangleWidth / 2, rect.height());
		break;
	case Left:
		polygon << QPoint(_triangleHeight, rect.height() / 2 - _triangleWidth / 2);
		polygon << QPoint(0, rect.height() / 2);
		_ArrowTopPoint = QPoint(0, rect.height() / 2);
		polygon << QPoint(_triangleHeight, rect.height() / 2 + _triangleWidth / 2);
		break;
	case Right:
		polygon << QPoint(rect.width(), rect.height() / 2 - _triangleWidth / 2);
		polygon << QPoint(rect.width() + _triangleHeight, rect.height() / 2);
		_ArrowTopPoint = QPoint(rect.width() + _triangleHeight, rect.height() / 2);
		polygon << QPoint(rect.width(), rect.height() / 2 + _triangleWidth / 2);
		break;
	}
	return polygon;
}

