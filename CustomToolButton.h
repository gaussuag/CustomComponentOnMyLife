#ifndef CUSTOMTOOLBUTTON_H
#define CUSTOMTOOLBUTTON_H

#include <QToolButton>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QFontMetrics>
#include <QMovie>

class CustomToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit CustomToolButton(QWidget *parent = 0);
	explicit CustomToolButton(const QString& text,QWidget *parent = 0);
    ~CustomToolButton();

    void setCenterAlignment();
    
    void setText(const QString &text);
    QString text();

    void setFontPixelSize(int size);
    void setFontWeight(int weight);

	void setRotate(qreal rotate);
    void setTransparentStyleSheet();
    void setTransparentWhiteBorderStyleSheet();

    void setGreenBlackStyleSheet();
    void setGreenWhiteStyleSheet();
    void setDefaultStyleSheet();
    void setBlackBlackStyleSheet();
    void setBlackWhiteStyleSheet();

    void setBlackBlueWhiteStyleSheet();
    void setBlueBlueWhiteStyleSheet();

	void setIconGif(QString GifPath);
	void startGif();
	void stopGif();
	void setIconGifenable(bool Flag, QString IconPath = QString());

	void setEnterButtonCursorOnButtonUnable(Qt::CursorShape CursorShape);
	void setEnterButtonSwitchCursorEnable(bool Flag);
	void setEnterButtonCursor(Qt::CursorShape CursorShape);

	void setIcon(const QIcon &);
	void setCheckedIcon(const QIcon &);
    void setHoverIcon(const QIcon & hoverIcon);

    void setHoverMaskText(const QString & text, const QColor &FontColor = QColor());
protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
    bool event(QEvent *e);


	void paintEvent(QPaintEvent *);
    void DrawRotateText();
    void DrawHoverMaskText();
    void DrawCountNumberText();
private:
    bool _CenterAlignment = false;
	bool _FirstSetAlignmentFlag = false;
    bool _Transform = false;
    qreal _Rotate = 0;
    QString _Text;

	QMovie *_movie;

	//Enter Cursor Parameter
	bool _enterSwitchCursorFlag{ false };
	Qt::CursorShape _enterCursorShape;
	Qt::CursorShape _EnterButtonCursorShapeOnButtonUnable;
	
	//Icon Parameter
	QIcon _defaultIcon;
	QIcon _checkedIcon;
	QIcon _hoverIcon;

    //Hover Mask Parameter
    QString _hoverMaskText{""};
    QColor _hoverMaskTextColor{Qt::black};
    bool _isHoverMaskTextFlag{false};
    bool _isHoverFlag{false};


    void updateCenterAlignmentStylesheet();
	void switchCursorShape();
    void recoverCursorShape();
signals:
	void mouseEnter();
	void mouseLeave();

	void enabledChange();
};

#endif // CUSTOMTOOLBUTTON_H
