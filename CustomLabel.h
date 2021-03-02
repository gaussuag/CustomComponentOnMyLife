#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QFont>
#include <QTimer>


class CustomLabel : public QLabel
{
    Q_OBJECT
public:
	enum TextAnimation {
		None = 0,
		EllipsisAnimation = 1
	};
    explicit CustomLabel(QWidget *parent = 0);
    explicit CustomLabel(QString Text,QWidget *parent = 0);
    ~CustomLabel();
    void setNecessaryFlag(bool);
    void setText(const QString &text);
    void setTextClickedEnable(bool);

    void setIcon(const QPixmap &pixmap, Qt::WindowFrameSection IconPosition = Qt::WindowFrameSection::TopLeftSection);

    void setPixmapByColor(const QColor & color);

    void setTipsTextColor();
    void setFontPixelSize(int size);
    void setFontColor(QColor color);

    void setLinkTextFontColor(const QString &color);
    void setLinkTextFontSize(int size);

    void setBackgroundColor(const QColor & color);

    void startTextAnimation(CustomLabel::TextAnimation AnimationFlag);
    void stopTextAnimation();

    void setHoverMask(bool flag, const QString &backgroundColor = "#c1252525",
                      const QPixmap & pixmap = QPixmap(), Qt::WindowFrameSection IconPosition = Qt::WindowFrameSection::TopLeftSection);

    void setSelectable(bool flag,const QColor & selectedBorderColor = QColor("#2680EB"));
    bool isSelected() { return _isSelectedFlag;}

    void setAutoFitTextHeightFlag(bool flag);
    bool getAutoFitTextHeightFlag();


    void setUsageDataInt32(qint32 usageData);
    qint32 getUsageDataInt32() { return UsageDataInt32; }
signals:
    void ClickedText();

	void resize_Signal();

	void enterLabel();
	void leaveLabel();

	void clickedLeftButton();
	void clickedRightButton();

	void doubleClickedLeftButton();
	void doubleClickedRightButton();

protected:
	void mouseDoubleClickEvent(QMouseEvent * event);
    bool eventFilter(QObject *watched, QEvent *event);

    void paintEvent(QPaintEvent *event);
    void drawIconPixmap();
    void drawSelectedBorder();
private slots:
    void linkActivated_slot(QString string);
private:
    bool _isNecessary{ false };
    bool _isTextClick{ false };
	bool _autoFitTextHeightFlag{ false };

    int _LinkTextFontSize = 10;
    QString _LinkTextFontColor = "rgb(167, 156, 94)";

	//Animation Parameter
	CustomLabel::TextAnimation animationFlag{CustomLabel::TextAnimation::None};
	QTimer *animationTimer{nullptr};
	QString ellipsisAnimationString{""};
	int ellipsisCount{3};
	int currentEllipsisCount{0};

    // Label Icon Pixmap Parameter
    bool _drawIconFlag{false};
    Qt::WindowFrameSection _iconPostion{Qt::TopLeftSection};
    QPixmap _iconPixmap;
    int _iconPixmapMargin{6};

    // Hover Mask Label Parameter
    CustomLabel *_maskLabel{nullptr};

    // Selected Parameter
    bool _enableSelectFlag{false};
    bool _isSelectedFlag{false};
    QColor _selectedBorderColor;

	//UsageData
	qint32 UsageDataInt32;

    void initWidget();
	void initAnimation();
	void initEllipsisAnimation();
	void clearAnimation();
};

#endif // CUSTOMLABEL_H
