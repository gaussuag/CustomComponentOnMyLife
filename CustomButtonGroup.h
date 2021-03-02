#ifndef CUSTOMBUTTONGROUP_H
#define CUSTOMBUTTONGROUP_H

#include <QObject>
#include <QAbstractButton>

class CustomButtonGroup : public QObject
{
    Q_OBJECT
public:
    explicit CustomButtonGroup(QObject *parent = nullptr);
    ~CustomButtonGroup();

    void addButton(QAbstractButton *button);
    void removeButton(QAbstractButton *button);

    QAbstractButton *checkedButton() { return _checkedButton;}
private slots:
    void button_clickeded_slot(bool flag);
private:
    QAbstractButton *_checkedButton{nullptr};

signals:
    void buttonToggled(QAbstractButton*,bool);
};

#endif // CUSTOMBUTTONGROUP_H
