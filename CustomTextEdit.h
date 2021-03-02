#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextBrowser>
#include <QEvent>
#include <QDropEvent>



class CustomTextEdit : public QTextBrowser
{
    Q_OBJECT

public:
    CustomTextEdit(QWidget *parent = nullptr);
    ~CustomTextEdit();

    void setPreViewMode(bool flag);
    void setAutoAdjustWidgetSizeFromContents(bool WidthFlag, bool HeightFlag);

    //设置文本省略模式
    void setElidedMode(bool flag);
    //设置文本最长长度，超出部分则以...省略显示
    void setElidedTextLenth(int lenth);

    QString toPlainText() const;
public slots:
    void setText(const QString &text);

    //展开所有文本信息，会重新调整TextBrowser的大小
    void expandBrowser();
    //收起文本信息，恢复到展开之前的大小
    void collapseBrowser();

	//填入截图操作像素
	void setPixmap(const QPixmap &);
protected:
    bool eventFilter(QObject *, QEvent *);
    void dropEvent(QDropEvent *event) override;
private slots:
    void anchorClicked_slot(QUrl Url);
private:
    QList<QString> _TempImageList;
    bool _AdjustWidthFlag;
    bool _AdjustHeightFlag;

    //Elied Text Function Parameter
    bool _ElideFlag;
    int _ElidedTextLenth;
    Qt::TextElideMode _TextElidedMode;
    QString _context;
    QSize _PickupSize;
    bool _ExpandBrowser;


	void initWidget();
    void handlePasteImage(const QClipboard *clipboard);
    QString saveTempImage(QPixmap pixmap);

    void deleteTempImage();
    void handleDropUrls(QList<QUrl> Urls);
    void insertImage(const QString &ImagePath);


    void setElidedText(const QString &text);
signals:
    void clickedUrl(const QString&);
};

#endif // CUSTOMTEXTEDIT_H
