#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H
#include <QSlider>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QSignalMapper>
#include <QDebug>

class CustomSlider:public QSlider
{
    Q_OBJECT
public:
    explicit CustomSlider(Qt::Orientation SliderType,int SliderThickness = 20,QWidget *parent = nullptr);
    ~CustomSlider();
	
    //设置滑块条样式
    void setHorizontalSliderQss(int height,                                       //滑动条高度
                      const QString &backgroundColor = "transparent",              //控件背景色
                      const QString &backgroundimage_url = "",          //滑块图片png
                      const QString &normalColor = "#9B9999",           //滑条颜色
                      const QString &handleColor = "#B1443F");          //指示器颜色
    void setVerticalSliderQss(int width,                               //滑动条高度
                      const QString &backgroundColor = "#FFFFFF",       //控件背景色
                      const QString &backgroundimage_url = "",          //滑块图片png
                      const QString &normalColor = "#9B9999",           //滑条颜色
                      const QString &handleColor = "#B1443F");          //指示器颜色

    int DefaultHeight = 20;
    int DefaultWidth = 20;

    void setSliderMaxRange(qint64 MaxRange);

    void setIgnoreDirectionKeyFlag(bool);

    int sliderPositionFromValue(int Value);

protected:
	void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

    void leaveEvent(QEvent *event);

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *ev);
private:
    int handlewidth = 0;//滑块宽度(返回的是滑块的外接矩形宽度)
    int handleheight = 0;//滑块高度(返回的是滑块的外接矩形高度)

	//过滤方向键标志位
    bool _ignoreDirectionKeyFlag{false};

    ///鼠标按下标志位
    bool _mousePressedFlag{false};

    void init_SliderButton(Qt::Orientation SliderType, int SliderThickness);

signals:
    void hoverValueChanged(const QPoint &,int);
    void leaveSlider();
    void clickedSlider(qint64);

};

#endif // CUSTOMSLIDER_H
