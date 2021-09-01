#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QRect>
#include <QIcon>

#include "arrowpopupwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget wgt;
    auto btn = new QPushButton("Click Me!", &wgt);
    auto layout = new QHBoxLayout;
    layout->addWidget(btn);
    wgt.setLayout(layout);
    QObject::connect(btn, &QPushButton::clicked, [btn] {
        QRect rc = btn->rect();
        QPoint pt = btn->mapToGlobal(QPoint(rc.width()/2, rc.height()));
         ArrowPopupWidget::showBalloon(QIcon(),
            "Gretting", "Hello World. What's your name?", pt, 10000, true);
    });

    wgt.setWindowTitle("Arrow Popup Widget Example");
    wgt.setFixedSize(QSize(250, 50));
    wgt.show();

    return app.exec();
}
