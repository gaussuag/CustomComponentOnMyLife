#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

class CustomButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CustomButton(QWidget *parent = nullptr);
    explicit CustomButton(const QString & text,QWidget *parent = nullptr);
    ~CustomButton();
	void setButtonBackgroundColor(QColor Color);

	void setTransparentStyleSheet();
    void setBlueBlueWhiteStyleSheet();

	void setFontPixelSize(int size);


	void setChecked(bool state);

	void setIcon(const QIcon &);
	void setCheckedIcon(const QIcon &);
	void setHoverIcon(const QIcon & hoverIcon);

    QColor backgroundColor(){return _backgroundColor;}
public slots:
protected:
    void mouseReleaseEvent(QMouseEvent*);

	bool event(QEvent * e);

	void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

public slots:
    void sendClickedSignal(bool state = false);

private:
	QIcon _defaultIcon;
	QIcon _checkedIcon;
    QIcon _hoverIcon;

    QColor _backgroundColor;

signals:
    void mouseEnter();
    void mouseLeave();

    void clickedRightButton();
	void showToolTip();
};

#endif // CUSTOMBUTTON_H
