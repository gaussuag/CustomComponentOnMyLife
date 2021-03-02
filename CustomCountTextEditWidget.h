#ifndef CUSTOMCOUNTTEXTEDITWIDGET_H
#define CUSTOMCOUNTTEXTEDITWIDGET_H

#include <QWidget>

class QLabel;
class QTextEdit;

class CustomCountTextEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomCountTextEditWidget(QWidget *parent = nullptr);
    explicit CustomCountTextEditWidget(int maxCount,const QString & title = QString(), QWidget *parent = nullptr);
    ~CustomCountTextEditWidget();

    QString getText();
public slots:
    void setTitle(const QString &title);
    void setMaxCount(int maxCount);
    void setText(const QString &text);
private:
    QLabel *_titleLabel{nullptr};
    QLabel *_currentCountLabel{nullptr};
    QLabel *_maxCountLabel{nullptr};
    int _maxCount{110};

    QTextEdit *_textEdit{nullptr};

    void initWidget(const QString & title = QString());


signals:

};

#endif // CUSTOMCOUNTTEXTEDITWIDGET_H
