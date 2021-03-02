#ifndef CUSTOMMENU_H
#define CUSTOMMENU_H

#include <QMenu>
#include <QWidgetAction>
#include <QActionGroup>
#include <QEvent>
#include <QActionEvent>
#include <QMap>
#include <QPair>

class QToolButton;

class CustomMenu : public QMenu
{
    Q_OBJECT

public:
	explicit CustomMenu(QWidget *parent = nullptr);
	explicit CustomMenu(const QString &title, QWidget *parent = nullptr);
    ~CustomMenu();

	QPair<QAction*, QAction*> addDoubleAction(const QIcon &, const QString &);
	QPair<QAction*, QAction*> addDoubleAction(const QString &);
	//Insert WidgetItem On Menu
    QAction *addWidget(QWidget *);

    void insertActionToMenu(int index, QAction* p_action, bool isAddSeparator = false);

    void setBlackBlueStyleSheet();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QMap<QObject*,QAction *> _WidgetObjectActionMap;

	QString MainWidgetStyleSheet{""};
	QString SubButtonStyleSheet{ "" };
	void initDefaultMenu();

	QToolButton * initDoubleActionWidget(const QIcon & icon, const QString & text, QWidget *widget);
};
#endif // CUSTOMMENU_H
