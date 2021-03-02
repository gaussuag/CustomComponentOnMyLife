#include "CustomMessageDialog.h"

#include "CustomToolButton.h"
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QWidget>
#include "CustomLabel.h"
#include "CommonOperation.h"

#include <QDebug>


CustomMessageDialog::CustomMessageDialog(QWidget *parent) : QDialog(parent)
{
    initWidget();
}
CustomMessageDialog::CustomMessageDialog(QString ButtonText1, QString ButtonText2, QWidget *parent)
    :QDialog(parent),_ButtonText1(ButtonText1), _ButtonText2(ButtonText2)
{
    initWidget();
}
CustomMessageDialog::CustomMessageDialog(QString Information, QString ButtonText1, QString ButtonText2, Qt::AlignmentFlag informationAlignmentFlag, QWidget *parent)
    :QDialog(parent), _Information(Information), _ButtonText1(ButtonText1), _ButtonText2(ButtonText2)
{
    initInformationWidget(informationAlignmentFlag);
}
CustomMessageDialog::CustomMessageDialog(QWidget * widget, const QString &Information, const QString & ButtonText1, const QString & ButtonText2, QWidget *parent)
    : QDialog(parent),_Information(Information),_ButtonText1(ButtonText1),_ButtonText2(ButtonText2)
{
    initCenterWidget(widget);
}


CustomMessageDialog::~CustomMessageDialog()
{

}


CustomMessageDialog::ReturnValue CustomMessageDialog::CustomCenterWidgetDialog(QWidget * centerWidget,const QString & Title,const QString & Information,
                                           const QString & ButtonText1, const QString & ButtonText2)
{
    QWidget *centerWidgetOriginParent = centerWidget->parentWidget();
    CustomMessageDialog centerDialog(centerWidget,Information,ButtonText1,ButtonText2,nullptr);
    centerDialog.setWindowTitle(Title);
    int value = centerDialog.exec();
    centerWidget->setParent(centerWidgetOriginParent);
    return CustomMessageDialog::ReturnValue(value);
}

void CustomMessageDialog::InformationMessageDialog(QString Title, QString Information, QString ButtonText, Qt::AlignmentFlag informationAlignmentFlag)
{
    CustomMessageDialog informationDialog(Information, ButtonText, "",informationAlignmentFlag, nullptr);
    informationDialog.setWindowTitle(Title);
    informationDialog.exec();
}
CustomMessageDialog::ReturnValue CustomMessageDialog::TwiceSelectButtonMessageDialog(QString Title, QString Information,
                                             QString ButtonText1, QString ButtonText2
                                             , Qt::AlignmentFlag informationAlignmentFlag)
{
    CustomMessageDialog twiceSelectButtonMessageDialog(Information, ButtonText1, ButtonText2,informationAlignmentFlag, nullptr);
    twiceSelectButtonMessageDialog.setWindowTitle(Title);
    int value = twiceSelectButtonMessageDialog.exec();

    return CustomMessageDialog::ReturnValue(value);
}

void CustomMessageDialog::initWidget()
{
    this->setStyleSheet("QDialog{background-color:white;border:1px solid gray;}");

    QSignalMapper *SignalMapper = new QSignalMapper(this);
    connect(SignalMapper, SIGNAL(mapped(QWidget *)), this, SLOT(ButtonSignalMapper_slot(QWidget *)));

    auto InitBtn = [this, SignalMapper](CustomToolButton* &p_btn, QString btnName = QString()) {
        p_btn = new CustomToolButton(this);
        p_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        p_btn->setMinimumSize(50, 25);
        p_btn->setObjectName(btnName);
        p_btn->setText(btnName);
        p_btn->setGreenBlackStyleSheet();
        connect(p_btn, SIGNAL(clicked()), SignalMapper, SLOT(map()));
        SignalMapper->setMapping(p_btn, p_btn);
    };

    InitBtn(_Button1, _ButtonText1);
    InitBtn(_Button2, _ButtonText2);
    QHBoxLayout *HBoxLayout = new QHBoxLayout();
    HBoxLayout->setSpacing(10);
    HBoxLayout->addWidget(_Button1);
    HBoxLayout->addWidget(_Button2);

    InitBtn(_Button3);
    _Button3->hide();
    QVBoxLayout* p_VBoxlayout = new QVBoxLayout(this);
    p_VBoxlayout->setMargin(80);
    p_VBoxlayout->setSpacing(10);
    p_VBoxlayout->addLayout(HBoxLayout);
    p_VBoxlayout->addWidget(_Button3);
}

void CustomMessageDialog::initInformationWidget(Qt::AlignmentFlag informationAlignmentFlag)
{
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    this->setStyleSheet("QDialog{background-color:white;border:1px solid gray;}");
    this->setMinimumSize(320, 160);
    this->setMaximumWidth(500);

    QVBoxLayout *VBoxLayout = new QVBoxLayout(this);
    VBoxLayout->setMargin(20);
    VBoxLayout->setSpacing(10);

    informationLabel = new CustomLabel(_Information, this);
    informationLabel->adjustSize();
    informationLabel->setWordWrap(true);

    informationLabel->setStyleSheet("background-color:transparent;");
    informationLabel->setAlignment(informationAlignmentFlag);
    QFont font = informationLabel->font();
    font.setPixelSize(20);
    informationLabel->setFont(font);
    VBoxLayout->addWidget(informationLabel);

    QHBoxLayout *HBoxLayout = new QHBoxLayout();
    HBoxLayout->setMargin(0);
    HBoxLayout->setSpacing(10);
    HBoxLayout->addStretch(2);

    QSignalMapper *SignalMapper = new QSignalMapper(this);
    connect(SignalMapper, SIGNAL(mapped(QWidget *)), this, SLOT(ButtonSignalMapper_slot(QWidget *)));

    auto InitBtn = [this, SignalMapper](CustomToolButton* &p_btn, QString btnName = QString()) {
        p_btn = new CustomToolButton(this);
        p_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        p_btn->setMinimumSize(50, 25);
        p_btn->setObjectName(btnName);
        p_btn->setText(btnName);
        p_btn->setGreenBlackStyleSheet();
        connect(p_btn, SIGNAL(clicked()), SignalMapper, SLOT(map()));
        SignalMapper->setMapping(p_btn, p_btn);
    };


    InitBtn(_Button1, _ButtonText1);
    HBoxLayout->addWidget(_Button1);

    if (!_ButtonText2.isEmpty())
    {
        InitBtn(_Button2, _ButtonText2);
        HBoxLayout->addWidget(_Button2);
    }
    VBoxLayout->addStretch(2);
    VBoxLayout->addLayout(HBoxLayout);

    InitBtn(_Button3);
    _Button3->hide();
    VBoxLayout->addWidget(_Button3);
}

void CustomMessageDialog::initCenterWidget(QWidget * widget)
{
    setWindowFlags(Qt::WindowCloseButtonHint);
    this->setStyleSheet("QDialog{background-color:white;}");

    QVBoxLayout *VBoxLayout = new QVBoxLayout(this);
    VBoxLayout->setMargin(20);
    VBoxLayout->setSpacing(10);

    if(!_Information.isEmpty())
    {
        informationLabel = new CustomLabel(_Information, this);
        informationLabel->adjustSize();
        informationLabel->setWordWrap(true);

        informationLabel->setStyleSheet("background-color:transparent;");
        informationLabel->setAlignment(Qt::AlignmentFlag::AlignLeft);
        QFont font = informationLabel->font();
        font.setPixelSize(20);
        informationLabel->setFont(font);
        VBoxLayout->addWidget(informationLabel);
    }

	auto centerLayout = new QHBoxLayout;
	centerLayout->addStretch(0);
	centerLayout->addWidget(widget);
	centerLayout->addStretch(0);
	centerLayout->setStretch(1, 1);
	VBoxLayout->addLayout(centerLayout);

    QHBoxLayout *HBoxLayout = new QHBoxLayout();
    HBoxLayout->setMargin(0);
    HBoxLayout->setSpacing(10);
    HBoxLayout->addStretch(2);

    QSignalMapper *SignalMapper = new QSignalMapper(this);
    connect(SignalMapper, SIGNAL(mapped(QWidget *)), this, SLOT(ButtonSignalMapper_slot(QWidget *)));

    auto InitBtn = [this, SignalMapper](CustomToolButton* &p_btn, QString btnName = QString()) {
        p_btn = new CustomToolButton(btnName,this);
        p_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        p_btn->setMinimumSize(50, 25);
        p_btn->setObjectName(btnName);
        p_btn->setGreenWhiteStyleSheet();
        connect(p_btn, SIGNAL(clicked()), SignalMapper, SLOT(map()));
        SignalMapper->setMapping(p_btn, p_btn);
    };

    if(!_ButtonText1.isEmpty())
    {
        InitBtn(_Button1, _ButtonText1);
        HBoxLayout->addWidget(_Button1);
    }
    if(!_ButtonText2.isEmpty())
    {
        InitBtn(_Button2, _ButtonText2);
        HBoxLayout->addWidget(_Button2);
    }

    VBoxLayout->addStretch(2);
    VBoxLayout->addLayout(HBoxLayout);
}

void CustomMessageDialog::ButtonSignalMapper_slot(QWidget *widget)
{
    if (widget == _Button1)
        done(FirstButton);
    else if (widget == _Button2)
        done(SecondButton);
    else if (widget == _Button3)
        done(ThirdButton);
}

void CustomMessageDialog::appendInformation(const QString & Information)
{
    if (informationLabel != nullptr)
    {
        informationLabel->setText(QString("%1\n%2").arg(informationLabel->text()).arg(Information));
        this->adjustSize();
        this->move(CommonOperationHelper->centerPoint(this->size()));
    }
}

void CustomMessageDialog::ActiveButtonThird(const QString & btnName)
{
    _Button3->setText(btnName);
    _Button3->setVisible(true);
}

