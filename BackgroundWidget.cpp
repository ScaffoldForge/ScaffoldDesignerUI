#include "BackgroundWidget.h"
#include <QPainter>

BackgroundWidget::BackgroundWidget(QWidget *parent)
    : QWidget(parent),
    background(":/imgs/images/scaffold_forge_title.png") // Load the image from resources
{
    // Let the layout system know this widget can expand freely
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void BackgroundWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // If the image loaded successfully, draw it stretched to fill the widget
    if (!background.isNull()) {
        painter.drawPixmap(rect(), background);
    }

    // Call base class paintEvent to ensure proper QWidget behavior
    QWidget::paintEvent(event);
}
