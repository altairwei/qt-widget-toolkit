#pragma once

#include <QWidget>

class ArrowPopupWidget : public QWidget
{
    Q_OBJECT
public:
    static void showBalloon(const QIcon &icon, const QString &title,
                            const QString &msg, const QPoint &pos,
                            int timeout, bool showArrow = true);
    static void hideBalloon();
    static bool isBalloonVisible();
    static void updateBalloonPosition(const QPoint& pos);

private:
    ArrowPopupWidget(const QIcon &icon, const QString &title,
                const QString &msg);
    ~ArrowPopupWidget();
    void balloon(const QPoint&, int, bool);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

private:
    QPixmap pixmap;
    int timerId;
    bool showArrow;
};