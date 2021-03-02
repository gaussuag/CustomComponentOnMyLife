#ifndef CUSTOMNAVIGATIONWIDGET_H
#define CUSTOMNAVIGATIONWIDGET_H

#include <QWidget>

class CustomNavigationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomNavigationWidget(QWidget *parent = nullptr);

    void setText(const QString & text);

    void setLeftButtonFlag(bool flag);
    void setRightButtonFlag(bool flag);

    void setFont(const QFont & font);

    void setTextColor(const QColor & color);

    void setEnableColor(const QColor & color);

    void setEnableArrowColor(const QColor & color);

    void setDisableColor(const QColor & color);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);
private:
    bool _leftEnableFlag{true};
    bool _rightEnableFlag{true};

    bool _pressedLeftRectFlag{false};
    bool _pressedRightRectFlag{false};

    bool _horverLeftRectFlag{false};
    bool _horverRightRectFlag{false};

    QString _text{""};
    QFont _font;
    QRect _leftRect,_rightRect;

    QColor _textColor{Qt::black};
    QColor _disableColor{Qt::gray};
    QColor _enableColor{QColor("#2181ec")};
    QColor _enableArrowColor{Qt::white};

    void initWidget();

    void handleHoverEvent(const QPoint &pos);
    void handleLeftButtonPressedEvent(const QPoint &pos);
    void handleLeftButtonClickedEvent(const QPoint &pos);
signals:
    void clickedLeftButton();
    void clickedRightButton();
public slots:
};

#endif // CUSTOMNAVIGATIONWIDGET_H
