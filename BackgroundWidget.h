#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include <QWidget>
#include <QPixmap>

/**
 * @class BackgroundWidget
 * @brief Custom QWidget that displays a scalable background image.
 *
 * This widget paints a background image (e.g., from a Qt resource)
 * scaled to the full widget area. Useful as a stylized base for UIs.
 */
class BackgroundWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for the BackgroundWidget.
     * @param parent The parent widget.
     */
    explicit BackgroundWidget(QWidget *parent = nullptr);

protected:
    /**
     * @brief Handles the paint event to draw the background image.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap background;  ///< Cached background image pixmap.
};


#endif // BACKGROUNDWIDGET_H
