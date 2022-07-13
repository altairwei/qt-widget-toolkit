#ifndef SHADOWWIDGET_H
#define SHADOWWIDGET_H

#include <QWidget>
#include <QPoint>

class ShadowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShadowWidget(QWidget *parent = nullptr);

    QWidget *widget() { return m_widget; }
    void setTimeOut(int timeout) { m_timeout = timeout; }

protected:
    void showEvent(QShowEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QWidget *m_widget;
    int m_timerId;
    int m_timeout;
    QPoint startPos;
};

#endif // SHADOWWIDGET_H
