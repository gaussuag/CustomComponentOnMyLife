#ifndef CUSTOMHEADERVIEW_H
#define CUSTOMHEADERVIEW_H

#include <QHeaderView>

class CustomHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit CustomHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr);
    ~CustomHeaderView() override;

protected:
    /*!
     * \brief updateGeometries 刷新Header的Geometry时的响应函数，计算checkbox的位置
     */
    void updateGeometries() override;

    /*!
     * \brief mousePressEvent 判断点击位置是否在CheckBox范围内
     * \param event
     */
    void mousePressEvent(QMouseEvent *event) override;

    /*!
     * \brief mouseDoubleClickEvent 判断逻辑同mouseDoubleClickEvent
     * \param event
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /*!
     * \brief paintSection 重载绘制事件，在此函数中绘制了CheckBox。
     * \param painter
     * \param rect
     * \param logicalIndex
     */
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
private:
    bool _checkBoxCheckedFlag{false};
    QRect _checkBoxGeometry;
signals:
    void clickedCheckBox(bool);
public slots:
};

#endif // CUSTOMHEADERVIEW_H
