#ifndef CUSTOMMESSAGEDIALOG_H
#define CUSTOMMESSAGEDIALOG_H

#include <QDialog>


class CustomToolButton;
class CustomLabel;

class CustomMessageDialog : public QDialog
{
    Q_OBJECT

public:
	enum ReturnValue{
		FirstButton = 11,
		SecondButton = 12,
        ThirdButton = 13
	};
    explicit CustomMessageDialog(QWidget *parent = nullptr);
    explicit CustomMessageDialog(QString ButtonText1, QString ButtonText2, QWidget *parent = nullptr);
    explicit CustomMessageDialog(QString Information, QString ButtonText1, QString ButtonText2
                                 ,Qt::AlignmentFlag informationAlignmentFlag = Qt::AlignmentFlag::AlignCenter, QWidget *parent = nullptr);

    explicit CustomMessageDialog(QWidget * widget,const QString & Information, const QString & ButtonText1, const QString & ButtonText2, QWidget * parent = nullptr);

    ~CustomMessageDialog() override;

    static CustomMessageDialog::ReturnValue CustomCenterWidgetDialog(QWidget * centerWidget,const QString & Title,const QString & Information = QString(),
                                                                     const QString & ButtonText1 = "ok", const QString & ButtonText2 = "");

    static void InformationMessageDialog(QString Title, QString Information, QString ButtonText,
                                         Qt::AlignmentFlag informationAlignmentFlag = Qt::AlignmentFlag::AlignCenter);
    static CustomMessageDialog::ReturnValue TwiceSelectButtonMessageDialog(QString Title, QString Information,
                                                                           QString ButtonText1, QString ButtonText2,
                                                                           Qt::AlignmentFlag informationAlignmentFlag = Qt::AlignmentFlag::AlignCenter);

	void appendInformation(const QString &);
	void ActiveButtonThird(const QString& btnName);

private slots:
    void ButtonSignalMapper_slot(QWidget *widget);
private:
	QString _Information = "";
	QString _ButtonText1 = "OK";
	QString _ButtonText2 = "";
    CustomToolButton *_Button1 = nullptr;
    CustomToolButton *_Button2 = nullptr;
    CustomToolButton *_Button3 = nullptr;

	CustomLabel *informationLabel{ nullptr };

    void initWidget();
    void initInformationWidget(Qt::AlignmentFlag informationAlignmentFlag = Qt::AlignmentFlag::AlignCenter);
	void initCenterWidget(QWidget * widget);
};



#endif // CUSTOMMESSAGEDIALOG_H
