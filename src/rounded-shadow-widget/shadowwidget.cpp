#include "shadowwidget.h"

#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>

ShadowWidget::ShadowWidget(QWidget *parent)
    : QWidget(parent)
    , m_widget(new QWidget)
{
    auto layout = new QVBoxLayout;
    layout->addWidget(m_widget);
    layout->setContentsMargins(6, 6, 6, 6);
    setLayout(layout);

    // Set the window as borderless and displayed on the top layer
    // Don't show icons on the taskbar
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);

    // If you want to make it work like a popup, you should use these flags.
    //setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint | Qt::Popup | Qt::WindowStaysOnTopHint)

    // The outer window appears transparent
    setAttribute(Qt::WA_TranslucentBackground, true);

    // This is not necessary for rounded shadow
    setAttribute(Qt::WA_DeleteOnClose);

    // Add the corresponding shadow effect to the inner window
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(0, 0);
    shadow_effect->setColor(QColor(150, 150, 150));
    shadow_effect->setBlurRadius(6);
    m_widget->setGraphicsEffect(shadow_effect);

    m_widget->setStyleSheet("border:1px solid #FFFFFF;border-radius:7px;background-color:#FFFFFF;");
}
