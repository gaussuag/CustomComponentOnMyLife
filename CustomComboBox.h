#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QMouseEvent>
#include <QAction>

#include <QDebug>
class CustomComboBox : public QWidget
{
    Q_OBJECT

public:
    explicit CustomComboBox(QWidget *parent = Q_NULLPTR);

    QListWidget *getDropDownListWidget()
    {
        return _DropDownList;
    }

    QLineEdit *getLineEdit()
    {
        return _LineEdit;
    }

	QString getText()
	{
        return _LineEdit->text();
	}

    QAction *getDropDownButtonAction();

    void SetDropDownListItem(QString string);

    QListWidgetItem* addComboBoxItem(const QString& text);

    void addComboBoxItem(QListWidgetItem* item);

    void clearDropDownListItem();

    void setEditUnable();

    void setText(QString string){_LineEdit->setText(string);}

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
	void UpArrowKey_triggered_slot(bool);

	void DownArrowKey_triggered_slot(bool state);

    void DropDownButton_slot(bool state);

    void DropDownList_itemPressed_slot(QListWidgetItem *item);

private:
    void initWidget();

    void initDropDownWidget();

    int getListWidgetHeightFromItemWidget();

    int getListWidgetHeightFromItem();

    int getCurrentLineEditTextOnListWidget();

private:
    QLineEdit * _LineEdit = nullptr;

	bool _EditEnable = true;

    QListWidget *_DropDownList = nullptr;

    QLabel *_TipsLabel = nullptr;

    QHBoxLayout *_HBoxLayout = nullptr;

};

#endif // CUSTOMCOMBOBOX_H
