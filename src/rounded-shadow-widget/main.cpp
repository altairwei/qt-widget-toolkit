#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

#include "shadowwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget wgt;
    auto btn = new QPushButton("Click Me!", &wgt);
    auto layout = new QHBoxLayout;
    layout->addWidget(btn);
    wgt.setLayout(layout);
    QObject::connect(btn, &QPushButton::clicked, [&] {
        ShadowWidget *sh = new ShadowWidget(&wgt);
        sh->setTimeOut(4000);
        sh->setMovable(true);
        auto layout = new QHBoxLayout;
        layout->addWidget(new QLabel("Gretting\nHello World. What's your name?"));
        sh->widget()->setLayout(layout);
        sh->show();
    });

    wgt.setWindowTitle("Rounded Shadow Widget Example");
    wgt.setFixedSize(QSize(450, 150));
    wgt.show();

    return app.exec();
}
