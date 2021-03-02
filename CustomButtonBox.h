#ifndef CUSTOMBUTTONBOX_H
#define CUSTOMBUTTONBOX_H

#include <QWidget>

#include <QPushButton>

class QHBoxLayout;

class CustomButtonBox : public QWidget
{
    Q_OBJECT
public:
    explicit CustomButtonBox(QWidget *parent = nullptr);
    ~CustomButtonBox();

    QPushButton* appendButton(const QString & text,bool isChecked);

    void setButtonStyleSheet(const QString & stylesheet);
private:
    QHBoxLayout *_buttonLayout{nullptr};

    QButtonGroup *_buttonGroup{nullptr};
    QList<QPushButton*> _buttons;

    void initBox();
signals:

};

#endif // CUSTOMBUTTONBOX_H
