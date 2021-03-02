#ifndef CUSTOMLINE_H
#define CUSTOMLINE_H

#include <QWidget>
#include <QPaintEvent>

class CustomLine : public QWidget
{
    Q_OBJECT
public:

    explicit CustomLine(QWidget *parent = nullptr,
                        int LineWidth = 1,
                        Qt::Orientation Orientation = Qt::Orientation::Vertical,
                        QColor Color = QColor(Qt::gray),Qt::PenStyle PenStyle = Qt::PenStyle::CustomDashLine);
    ~CustomLine();

    void setLineOrientation(Qt::Orientation Orientation);
    void setDashStepLenth(qreal StepLenth,qreal StepSpaceLenth = 2.0);

    void setLineColor(const QString & Color);

    void setLineWidth(int w);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Qt::Orientation _orientation{Qt::Orientation::Vertical};

    Qt::PenStyle _penStyle{Qt::PenStyle::CustomDashLine};

    qreal _dashStepLenth{10.0};
    qreal _dashStepSpaceLenth{2.0};
    int _lineWidth{1};

    QColor _lineColor{Qt::black};
};

#endif // CUSTOMLINE_H
