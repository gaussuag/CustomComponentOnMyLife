#include "CustomTextEdit.h"

#include <QKeyEvent>
#include <QClipboard>
#include <QMimeData>
#include <QTextBlock>
#include <QApplication>
#include <QScreen>
#include <QStandardPaths>
#include <QUrl>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QDate>
#include <QScrollBar>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QFontMetrics>

#include <QTextLayout>
#include <QTextLine>

#include <QDebug>






CustomTextEdit::CustomTextEdit(QWidget *parent)
    :_AdjustWidthFlag(false),_AdjustHeightFlag(false),_ElideFlag(false),_ExpandBrowser(false),_TextElidedMode(Qt::ElideRight)
      ,QTextBrowser(parent)
{
    initWidget();
}

CustomTextEdit::~CustomTextEdit()
{
    deleteTempImage();
}

void CustomTextEdit::setPreViewMode(bool flag)
{
    if(flag)
    {
        setReadOnly(true);
        setStyleSheet("CustomTextEdit{background-color:transparent;border:0px;}");

    }else
    {
        setReadOnly(false);
    }
}

void CustomTextEdit::setElidedMode(bool flag)
{
    _AdjustWidthFlag = !flag;
    _AdjustHeightFlag = !flag;

    _ElideFlag = flag;
}
void CustomTextEdit::setElidedTextLenth(int lenth)
{
    _ElidedTextLenth = lenth;
}

void CustomTextEdit::expandBrowser()
{
    if(_ElideFlag)
    {
        _ElideFlag = false;
        _ExpandBrowser = true;
        _PickupSize = size();
        //enable Auto AdjustHeighFlag
        _AdjustHeightFlag = true;
        QTextBrowser::setText(_context);
    }
}
void CustomTextEdit::collapseBrowser()
{
    if(_ExpandBrowser)
    {
        _ExpandBrowser = false;
        _ElideFlag = true;
        //disable Auto AdjustHeighFlag
        _AdjustHeightFlag = false;
        setText(_context);
        this->setFixedSize(_PickupSize);
    }
}

void CustomTextEdit::setPixmap(const QPixmap & pixmap)
{
	QString FilePath = saveTempImage(pixmap);
	if (FilePath.isEmpty())
		return;

	insertImage(FilePath);
}

QString CustomTextEdit::toPlainText() const
{
    if(_ElideFlag)
        return _context;
    else
        return QTextBrowser::toPlainText();
}
void CustomTextEdit::setText(const QString &text)
{
    _context = text;
    if(_ElideFlag)
        setElidedText(text);
    else
        QTextBrowser::setText(text);
}
void CustomTextEdit::setElidedText(const QString &text)
{
    QFontMetrics fontMetrics = this->fontMetrics();
    QTextBrowser::setText(fontMetrics.elidedText(text,_TextElidedMode,_ElidedTextLenth));
}
//Auto adjust WidgetSize from Contents
void CustomTextEdit::setAutoAdjustWidgetSizeFromContents(bool WidthFlag, bool HeightFlag)
{
	_AdjustWidthFlag = WidthFlag;

	_AdjustHeightFlag = HeightFlag;
}

void CustomTextEdit::deleteTempImage()
{
    for(const auto& ImagePath : _TempImageList)
    {
        QFile Image(ImagePath);
        if(Image.exists())
            Image.remove();
    }
}

bool CustomTextEdit::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(keyEvent->matches(QKeySequence::Paste))
            {
                const QClipboard *clipboard = QApplication::clipboard();

                if(clipboard->mimeData()->hasImage())
                {
                    handlePasteImage(clipboard);
                    return true;
                }else
					return QTextEdit::eventFilter(obj, event);
            }
        }
	}
	else if (obj == verticalScrollBar() && _AdjustHeightFlag)
	{
		if (event->type() == QEvent::Show)
			setMinimumHeight(document()->size().height() + 10);
	}
	else if (obj == horizontalScrollBar() && _AdjustWidthFlag)
	{
		if (event->type() == QEvent::Show)
			setMinimumWidth(document()->size().width() + 10);
	}

    return QTextEdit::eventFilter(obj,event);
}

void CustomTextEdit::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        //Don't Show DropMimeData Text Or Url etc...
        QMimeData *data = new QMimeData;
        QDropEvent *e = new QDropEvent(event->pos(),event->possibleActions(),data,event->mouseButtons(),event->keyboardModifiers());
        QTextEdit::dropEvent(e);
        delete data;

        handleDropUrls(event->mimeData()->urls());
    }else
        return QTextEdit::dropEvent(event);
}




void CustomTextEdit::initWidget()
{
    installEventFilter(this);
	setUndoRedoEnabled(true);
	setReadOnly(false);
    setTextInteractionFlags(Qt::TextBrowserInteraction|Qt::TextEditorInteraction);
	

    QFont Font = font();
    Font.setFamily("Arial");
    Font.setPixelSize(16);
    document()->setDefaultFont(Font);

    setOpenLinks(false);
    connect(this,SIGNAL(anchorClicked(QUrl)),this,SLOT(anchorClicked_slot(QUrl)));

}

void CustomTextEdit::handlePasteImage(const QClipboard *clipboard)
{
    QString FilePath = saveTempImage(clipboard->pixmap());
    if(FilePath.isEmpty())
        return;

    insertImage(FilePath);
}
void CustomTextEdit::insertImage(const QString &ImagePath)
{
    QTextCursor cursor = textCursor();
	
    QImage image(ImagePath);
    if(image.isNull())
        return;

    if(image.size().width() > 150 || image.size().height() > 150)
        image = image.scaled(150,150,Qt::KeepAspectRatio,Qt::SmoothTransformation);


    QTextBlockFormat BlockFormat =  cursor.blockFormat();
    QTextCharFormat CharFormat = cursor.charFormat();

    QTextDocument * textDocument = document();
    textDocument->addResource( QTextDocument::ImageResource, ImagePath, QVariant (image));

    QTextImageFormat ImageFormat;
    ImageFormat.setAnchor(true);
    ImageFormat.setAnchorHref(ImagePath);
    ImageFormat.setWidth(image.size().width()+1);
    ImageFormat.setHeight(image.size().height()+1);
    ImageFormat.setName(ImagePath);

    cursor.insertImage(ImageFormat);
    cursor.insertText(" ",CharFormat);
    cursor.insertBlock(BlockFormat,CharFormat);
}

QString CustomTextEdit::saveTempImage(QPixmap pixmap)
{
    QString tempFolderPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString CurrentDate = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString FileName = tempFolderPath + "/" + "INSnap" + CurrentDate + ".png";

    bool flag = pixmap.save(FileName);
    if(flag)
    {
        _TempImageList.append(FileName);
        return FileName;
    }
    else
        return QString();
}

void CustomTextEdit::handleDropUrls(QList<QUrl> Urls)
{
    for(const auto& url : Urls)
    {
        QImage Image(url.toLocalFile());
        if(!Image.isNull())
            insertImage(url.toLocalFile());
        else
            insertPlainText(url.toString());
    }

}

//点击图片，弹出图片查看窗口
void CustomTextEdit::anchorClicked_slot(QUrl Url)
{
    emit(clickedUrl(Url.toString()));
}



