#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QApplication>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QAction>
#include <QEvent>

#include <QDebug>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CustomLineEdit(QWidget *parent = 0);
	~CustomLineEdit();

	void setAutoComplete(bool  flag);
	void setCompleterStringList(QStringList StringList);

    /// 设置跟随文本内容长度调整控件宽度的标志位i
    void setResizeToContentsFlag(bool flag);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QCompleter *_Completer;
    bool _isResizeToContentsFlag{false};
    void initWidget();

};

#endif // CUSTOMLINEEDIT_H
