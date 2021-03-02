#ifndef CUSTOMDATEEDIT_H
#define CUSTOMDATEEDIT_H

#include <QLineEdit>
#include <QApplication>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDate>
#include <QCalendarWidget>
#include <QAction>
#include <QEvent>

#include <QDebug>

class CustomDateEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CustomDateEdit(QWidget *parent = 0);
    CustomDateEdit(QDate date, QWidget *parent);
    ~CustomDateEdit();

    void setDate(QDate);
    void setActionIcon(QIcon icon);
	void RemoveMiniDate();
	void setCalendarRange(QPair<QDate, QDate>& range);
	void SetCalendarShowVerticalBar(bool bShow);

signals:
	void CustomDateChange(QDate date);
public slots:
	void setMinimumDate_slot(QDate date);
	void setMaximumDate_slot(QDate date);
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void DateEditAction_triggered_slot(bool state);
    void CalendarWidget_clicked_slot(QDate date);
private:
    QAction *_DateEditAction = nullptr;
    QCalendarWidget *_CalendarWidget = nullptr;
    QDate _Date;
    QString _DateFormat = "yyyy.MM.dd";

    void initWidget();

};

#endif // CUSTOMDATEEDIT_H
