#include "arrowpopupwidget.h"

#include <QLabel>
#include <QGridLayout>
#include <QPainter>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QPainterPath>
#include <QBitmap>
#include <QIcon>
#include <QPushButton>
#include <QStyle>

static ArrowPopupWidget *theSolitaryBalloonTip = nullptr;

void ArrowPopupWidget::showBalloon(const QIcon &icon, const QString &title,
                              const QString &message, const QPoint &pos,
                              int timeout, bool showArrow)
{
    hideBalloon();
    if (message.isEmpty() && title.isEmpty())
        return;

    theSolitaryBalloonTip = new ArrowPopupWidget(icon, title, message);
    if (timeout < 0)
        timeout = 10000; //10 s default
    theSolitaryBalloonTip->balloon(pos, timeout, showArrow);
}

void ArrowPopupWidget::hideBalloon()
{
    if (!theSolitaryBalloonTip)
        return;
    theSolitaryBalloonTip->hide();
    delete theSolitaryBalloonTip;
    theSolitaryBalloonTip = nullptr;
}

void ArrowPopupWidget::updateBalloonPosition(const QPoint& pos)
{
    if (!theSolitaryBalloonTip)
        return;
    theSolitaryBalloonTip->hide();
    theSolitaryBalloonTip->balloon(pos, 0, theSolitaryBalloonTip->showArrow);
}

bool ArrowPopupWidget::isBalloonVisible()
{
    return theSolitaryBalloonTip;
}

ArrowPopupWidget::ArrowPopupWidget(const QIcon &icon, const QString &title, const QString &message)
    : QWidget(nullptr, Qt::ToolTip),
      timerId(-1),
      showArrow(true)
{
    setAttribute(Qt::WA_DeleteOnClose);

    QLabel *titleLabel = new QLabel;
    titleLabel->installEventFilter(this);
    titleLabel->setText(title);
    QFont f = titleLabel->font();
    f.setBold(true);
    titleLabel->setFont(f);
    titleLabel->setTextFormat(Qt::PlainText); // to maintain compat with windows

    const int iconSize = 18;
    const int closeButtonSize = 15;

    QPushButton *closeButton = new QPushButton;
    closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    closeButton->setIconSize(QSize(closeButtonSize, closeButtonSize));
    closeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    closeButton->setFixedSize(closeButtonSize, closeButtonSize);
    QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QLabel *msgLabel = new QLabel;
    msgLabel->installEventFilter(this);
    msgLabel->setText(message);
    msgLabel->setTextFormat(Qt::PlainText);
    msgLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QGridLayout *layout = new QGridLayout;
    if (!icon.isNull()) {
        QLabel *iconLabel = new QLabel;
        iconLabel->setPixmap(icon.pixmap(iconSize, iconSize));
        iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        iconLabel->setMargin(2);
        layout->addWidget(iconLabel, 0, 0);
        layout->addWidget(titleLabel, 0, 1);
    } else {
        layout->addWidget(titleLabel, 0, 0, 1, 2);
    }

    layout->addWidget(closeButton, 0, 2);

    layout->addWidget(msgLabel, 1, 0, 1, 3);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->setContentsMargins(3, 3, 3, 3);
    setLayout(layout);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(0xff, 0xff, 0xe1));
    pal.setColor(QPalette::WindowText, Qt::black);
    setPalette(pal);
}

ArrowPopupWidget::~ArrowPopupWidget()
{
    theSolitaryBalloonTip = nullptr;
}

void ArrowPopupWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), pixmap);
}

void ArrowPopupWidget::resizeEvent(QResizeEvent *ev)
{
    QWidget::resizeEvent(ev);
}

void ArrowPopupWidget::balloon(const QPoint& pos, int msecs, bool showArrow)
{
    this->showArrow = showArrow;
    QScreen *screen = QGuiApplication::screenAt(pos);
    if (!screen) return;
    QRect scr = screen->geometry();
    QSize sh = sizeHint();
    const int border = 1;
    const int arrowHeight = 8, arrowOffset = 18, arrowWidth = 16, borderRadius = 0;

    // Determine arrow position
    bool arrowAtTop = (pos.y() + sh.height() + arrowHeight < scr.height());
    bool arrowAtLeft = (pos.x() + sh.width() - arrowOffset < scr.width());

    // Resize content margin
    setContentsMargins(
        border + 3,
        border + (arrowAtTop ? arrowHeight : 0) + 2,
        border + 3,
        border + (arrowAtTop ? 0 : arrowHeight) + 2);
    updateGeometry();
    sh  = sizeHint();

    // Determine position of widget rect
    int marginLeft, marginRight, marginTop, marginBottom;
    QSize sz = sizeHint();
    if (!arrowAtTop) {
        marginLeft = marginTop = 0;
        marginRight = sz.width() - 1;
        marginBottom = sz.height() - arrowHeight - 1;
    } else {
        marginLeft = 0;
        marginTop = arrowHeight;
        marginRight = sz.width() - 1;
        marginBottom = sz.height() - 1;
    }

    // Draw border of whole widget including arrow
    QPainterPath path;
    path.moveTo(marginLeft + borderRadius, marginTop);
    if (arrowAtTop && arrowAtLeft) {
        if (showArrow) {
            path.lineTo(marginLeft + arrowOffset, marginTop);
            path.lineTo(marginLeft + arrowOffset + arrowWidth/2, marginTop - arrowHeight);
            path.lineTo(marginLeft + arrowOffset + arrowWidth, marginTop);
        }
        move(qMax(pos.x() - arrowOffset, scr.left() + 2), pos.y());
    } else if (arrowAtTop && !arrowAtLeft) {
        if (showArrow) {
            path.lineTo(marginRight - arrowOffset - arrowWidth, marginTop);
            path.lineTo(marginRight - arrowOffset - arrowWidth/2, marginTop - arrowHeight);
            path.lineTo(marginRight - arrowOffset, marginTop);
        }
        move(qMin(pos.x() - sh.width() + arrowOffset, scr.right() - sh.width() - 2), pos.y());
    }
    path.lineTo(marginRight - borderRadius, marginTop);
    path.arcTo(QRect(marginRight - borderRadius*2, marginTop, borderRadius*2, borderRadius*2), 90, -90);
    path.lineTo(marginRight, marginBottom - borderRadius);
    path.arcTo(QRect(marginRight - borderRadius*2, marginBottom - borderRadius*2, borderRadius*2, borderRadius*2), 0, -90);
    if (!arrowAtTop && !arrowAtLeft) {
        if (showArrow) {
            path.lineTo(marginRight - arrowOffset, marginBottom);
            path.lineTo(marginRight - arrowOffset - arrowWidth/2, marginBottom + arrowHeight);
            path.lineTo(marginRight - arrowOffset - arrowWidth, marginBottom);
        }
        move(qMin(pos.x() - sh.width() + arrowOffset, scr.right() - sh.width() - 2),
             pos.y() - sh.height());
    } else if (!arrowAtTop && arrowAtLeft) {
        if (showArrow) {
            path.lineTo(arrowOffset + arrowWidth, marginBottom);
            path.lineTo(arrowOffset + arrowWidth/2, marginBottom + arrowHeight);
            path.lineTo(arrowOffset, marginBottom);
        }
        move(qMax(pos.x() - arrowOffset, scr.x() + 2), pos.y() - sh.height());
    }
    path.lineTo(marginLeft + borderRadius, marginBottom);
    path.arcTo(QRect(marginLeft, marginBottom - borderRadius*2, borderRadius*2, borderRadius*2), -90, -90);
    path.lineTo(marginLeft, marginTop + borderRadius);
    path.arcTo(QRect(marginLeft, marginTop, borderRadius*2, borderRadius*2), 180, -90);

    // Set the mask
    QBitmap bitmap = QBitmap(sizeHint());
    bitmap.fill(Qt::color0);
    QPainter painter1(&bitmap);
    painter1.setPen(QPen(Qt::color1, border));
    painter1.setBrush(QBrush(Qt::color1));
    painter1.drawPath(path);
    setMask(bitmap);

    // Draw the border
    pixmap = QPixmap(sz);
    QPainter painter2(&pixmap);
    painter2.setPen(QPen(palette().color(QPalette::Window).darker(160), border));
    painter2.setBrush(palette().color(QPalette::Window));
    painter2.drawPath(path);

    if (msecs > 0)
        timerId = startTimer(msecs);
    show();
}

void ArrowPopupWidget::mousePressEvent(QMouseEvent *e)
{
    close();
}

void ArrowPopupWidget::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == timerId) {
        killTimer(timerId);
        if (!underMouse())
            close();
        return;
    }
    QWidget::timerEvent(e);
}