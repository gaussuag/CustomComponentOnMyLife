#ifndef CUSTOMCOUNTBUTTON_H
#define CUSTOMCOUNTBUTTON_H

#include <QToolButton>

class QLabel;
class QLayout;

class CustomCountButton : public QToolButton
{
    Q_OBJECT
public:
    explicit CustomCountButton(QWidget * parent = nullptr);
    ~CustomCountButton();

    void setText(const QString & text);
    void setCountNumber(int count);

    void setCountLabelSize(const QSize & size);
    void setCountLabelStyleSheet(const QString & StyleSheet);

    void setTextLabelStyleSheet(const QString & StyleSheet);

    void setLayoutMargin(int horizontalMargin);
private:
    Qt::Orientation _layoutOrientation{Qt::Orientation::Horizontal};
    QLayout *_layout{nullptr};
    QLabel *_countLabel{nullptr};
    QLabel *_textLabel{nullptr};

    int _layoutMargin{16};

    void initWidget();
};


#endif // CUSTOMCOUNTBUTTON_H
