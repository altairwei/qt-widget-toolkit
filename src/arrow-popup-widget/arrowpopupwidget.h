#pragma once

#include <QWidget>
#include <QPainterPath>

class ArrowPopupWidget : public QWidget
{
    Q_OBJECT

public:
    static void showPopup(const QIcon &icon, const QString &title,
                            const QString &msg, const QPoint &pos,
                            int timeout, bool showArrow = true);
    static void hidePopup();
    static bool isPopupVisible();
    static void updatePopupPosition(const QPoint& pos);

private:
    ArrowPopupWidget(const QIcon &icon, const QString &title,
                const QString &msg);
    ~ArrowPopupWidget();
    void showAtPos(const QPoint&, int, bool);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

private:
    QPixmap pixmap;
    QPainterPath m_path;
    int timerId;
    bool showArrow;
};
