#include "CustomDragDropWidget.h"

#include <QDir>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QMimeDatabase>
#include <QMimeType>
#include <QtConcurrentRun>

static QStringList getFilesBy(const QString &dirPath, bool isRecursively);

CustomDragDropWidget::CustomDragDropWidget(QWidget *parent):QWidget(parent),_isAcceptAllTypeFlag(true)
{
    initDefaultStyle();
    initWidget();
    initConnect();
}

CustomDragDropWidget::CustomDragDropWidget(const QStringList &acceptMimeTypes, QWidget *parent) : QWidget(parent),_acceptMimeTypes(acceptMimeTypes)
{
    initDefaultStyle();
    initWidget();
    initConnect();
}

CustomDragDropWidget::~CustomDragDropWidget()
{

}

void CustomDragDropWidget::setAcceptMimeTypes(const QStringList &acceptMimeTypes)
{
    _acceptMimeTypes = acceptMimeTypes;
}

void CustomDragDropWidget::setAcceptAllType(bool flag)
{
    _isAcceptAllTypeFlag = flag;
}

void CustomDragDropWidget::setDragNormalTipsCenterWidget(QWidget *widget)
{
    if(!widget)
        return;

    _normalTipsWidget = QPointer<QWidget>(widget);
    _stackedWidget->insertWidget(0,widget);
}

void CustomDragDropWidget::setDragEnterTipsCenterWidget(QWidget *widget)
{
    if(!widget)
        return;

    _enterTipsWidget = QPointer<QWidget>(widget);
    _stackedWidget->insertWidget(1,widget);
    widget->hide();
}

void CustomDragDropWidget::setDragRejectTipsCenterWidget(QWidget * widget)
{
    if(!widget)
        return;

    _rejectTipsWidget = QPointer<QWidget>(widget);
    _stackedWidget->insertWidget(2,widget);

    widget->hide();
}

void CustomDragDropWidget::setDragEnterBorderColor(const QColor &color)
{
    _dragEnterBorderColor = color;
    update();
}

void CustomDragDropWidget::setDragEnterBackgroundColor(const QColor &color)
{
    _dragEnterAcceptBackgroundColor = color;
    update();
}

void CustomDragDropWidget::setXoredMask(const QList<QWidget *> &xoredWidgets)
{
    _xoredWidgets = xoredWidgets;
}

void CustomDragDropWidget::resizeWithParentWidget(bool flag)
{
    _resizeWithParentWidget = flag;
    if(_resizeWithParentWidget && parent())
        parent()->installEventFilter(this);
}

bool CustomDragDropWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == parentWidget())
    {
        if(QEvent::Resize == event->type() || QEvent::Show == event->type())
        {
            if(_resizeWithParentWidget)
            {
                setGeometry(0,0,parentWidget()->width(),parentWidget()->height());
                _mainLayout->update();
            }
            if(!_xoredWidgets.isEmpty())
                handleXoredWidgetMask();
        }
    }

    return QWidget::eventFilter(watched,event);
}

void CustomDragDropWidget::dragEnterEvent(QDragEnterEvent *event)
{
    /// 恢复标志位
    updateAcceptFlag(CustomDragDropActionEnum::noneAction);

    /// 多线程读取文件列表类型，判断是否接收事件
    auto urls = event->mimeData()->urls();
    QtConcurrent::run(this,&CustomDragDropWidget::validationMimeTypeByUrls,urls);

    /// 先设置忽略事件提示，并接收此进入事件
    event->setDropAction(Qt::DropAction::IgnoreAction);
    event->accept();
}

void CustomDragDropWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    updateAcceptFlag(CustomDragDropActionEnum::noneAction);
    emit(dragDropWidgetDragEventLeave());

    return QWidget::dragLeaveEvent(event);
}

void CustomDragDropWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(_isAcceptFlag == CustomDragDropActionEnum::acceptTypeAction)
    {
        event->setDropAction(Qt::DropAction::LinkAction);
        event->accept();
        update();
    }
    else if((_isAcceptFlag == CustomDragDropActionEnum::rejectTypeAction)
            || (_isAcceptFlag == CustomDragDropActionEnum::noneAction))
        event->ignore();
}

void CustomDragDropWidget::dropEvent(QDropEvent *event)
{
    handleDropUrls(event->mimeData()->urls());
}

void CustomDragDropWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if((_isAcceptFlag == CustomDragDropActionEnum::acceptTypeAction)
            || (_isAcceptFlag == CustomDragDropActionEnum::rejectTypeAction))
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QPen pen;
        pen.setColor(_dragEnterBorderColor);
        pen.setStyle(Qt::PenStyle::CustomDashLine);
        pen.setWidth(6);
        pen.setDashPattern(QVector<qreal>()<<0.5<<2);

        painter.setPen(pen);
        painter.setBrush((_isAcceptFlag == CustomDragDropActionEnum::acceptTypeAction) ? _dragEnterAcceptBackgroundColor
                                                                                       : _dragEnterRejectBackgroundColor);

        QRect Rect = rect();
        painter.drawRect(Rect);
    }
}

void CustomDragDropWidget::initDefaultStyle()
{
    _dragEnterBorderColor = QColor("#b9b9b9");
    _dragEnterAcceptBackgroundColor = QColor("#33378ef0");
    _dragEnterRejectBackgroundColor = QColor("#7fE34850");
}

void CustomDragDropWidget::initWidget()
{
    setMouseTracking(true);
    setAcceptDrops(true);

    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setSpacing(0);
    _mainLayout->setMargin(0);

    _stackedWidget = new QStackedWidget(this);

    _mainLayout->addStretch();
    _mainLayout->addWidget(_stackedWidget,1,Qt::AlignCenter);
    _mainLayout->addStretch();
}

void CustomDragDropWidget::initConnect()
{
    connect(this,&CustomDragDropWidget::acceptFlagChanged,this,&CustomDragDropWidget::acceptFlagChanged_slot,Qt::QueuedConnection);
}

bool CustomDragDropWidget::urlIsAcceptMimeType(const QString & url)
{
    if(_isAcceptAllTypeFlag)
        return true;

    QMimeDatabase mimeDatabase;
    QString mimeType = mimeDatabase.mimeTypeForFile(url).name();
    for(const auto & presetMimeType : _acceptMimeTypes)
    {
        if(mimeType.startsWith(presetMimeType))
            return true;
    }
    return false;
}

bool CustomDragDropWidget::validationMimeTypeByUrls(const QList<QUrl> & Urls)
{
    bool AcceptFlag = false;
    for(auto url : Urls)
    {
        AcceptFlag = urlIsAcceptMimeType(url.toLocalFile());
        if(!AcceptFlag)
            break;
    }

    updateAcceptFlag((AcceptFlag) ? CustomDragDropActionEnum::acceptTypeAction : CustomDragDropActionEnum::rejectTypeAction);

    return AcceptFlag;
}

void CustomDragDropWidget::handleDropUrls(const QList<QUrl> & Urls)
{
    bool mimeTypeValidationFlag = validationMimeTypeByUrls(Urls);
    if(mimeTypeValidationFlag)
    {
        QStringList localPaths;
        for(const auto & url : Urls)
        {
            QString localFile = url.toLocalFile();
            /// 检查是否有目录路径，有则遍历出目录下所有子文件列表
            QFileInfo fileInfo(localFile);
            if(fileInfo.isDir())
                localPaths.append(getFilesBy(localFile,true));
            else
                localPaths.append(localFile);
        }
        emit(acceptDropLocalPaths(localPaths));
    }

    updateAcceptFlag(CustomDragDropActionEnum::noneAction);
}

void CustomDragDropWidget::updateAcceptFlag(CustomDragDropActionEnum actionEnum)
{
    if(_isAcceptFlag != actionEnum)
    {
        _isAcceptFlag = actionEnum;
        emit(acceptFlagChanged());

    }
}

void CustomDragDropWidget::acceptFlagChanged_slot()
{
    /// 更换显示的TipsWidget
    switchTipsWidget();
    update();
}

void CustomDragDropWidget::switchTipsWidget()
{
    if(_isAcceptFlag == CustomDragDropActionEnum::acceptTypeAction)
    {
        if(_stackedWidget && !_enterTipsWidget.isNull())
            _stackedWidget->setCurrentWidget(_enterTipsWidget);
    }
    else if(_isAcceptFlag == CustomDragDropActionEnum::rejectTypeAction)
    {
        if(_stackedWidget && !_rejectTipsWidget.isNull())
            _stackedWidget->setCurrentWidget(_rejectTipsWidget);
    }
    else if(_isAcceptFlag == CustomDragDropActionEnum::noneAction)
    {
        if(_stackedWidget && !_normalTipsWidget.isNull())
            _stackedWidget->setCurrentWidget(_normalTipsWidget);
    }
}

void CustomDragDropWidget::handleXoredWidgetMask()
{
    QRegion mainRegion = geometry();
    for(const auto & widget : _xoredWidgets)
        mainRegion = mainRegion.xored(QRegion(widget->geometry()));

    setMask(mainRegion);
    emit(dragDropValidationRegionChanged(mainRegion));
}




QStringList getFilesBy(const QString &dirPath, bool isRecursively)
{
    QStringList files;
    QDir dir(dirPath);

    if(dir.isEmpty())
        return files;

    if(isRecursively)
    {
        auto subFolders = dir.entryInfoList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot);

        for(const auto subFolder : subFolders)
            files.append(getFilesBy(subFolder.absoluteFilePath(),true));
    }
    auto currentFiles = dir.entryList(QDir::Filter::Files);
    if(!currentFiles.isEmpty())
    {
        for(const auto & file : currentFiles)
            files.append(dirPath + "/" + file);
    }

    return files;
}



