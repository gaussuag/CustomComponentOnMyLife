#ifndef CUSTOMMASKWIDGET_H
#define CUSTOMMASKWIDGET_H

#include <QLabel>

class CustomMaskLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CustomMaskLabel(QWidget *parent = nullptr);
    ~CustomMaskLabel();

    void setIcon(const QPixmap &pixmap,Qt::WindowFrameSection IconPosition = Qt::TopLeftSection);

protected:
    void paintEvent(QPaintEvent *event);
    void drawIconPixmap();
private:
    bool _drawIconFlag{false};
    Qt::WindowFrameSection _iconPostion{Qt::TopLeftSection};
    int _iconPixmapMargin{6};

    QColor _backgroundColor{"#3d252525"};


    QPixmap _iconPixmap;

    void initWidget();
signals:

public slots:
};

#endif // CUSTOMMASKWIDGET_H
