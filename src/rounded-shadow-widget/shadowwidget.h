#ifndef SHADOWWIDGET_H
#define SHADOWWIDGET_H

#include <QWidget>

class ShadowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShadowWidget(QWidget *parent = nullptr);

    QWidget *widget() { return m_widget; }

private:
    QWidget *m_widget;

};

#endif // SHADOWWIDGET_H
