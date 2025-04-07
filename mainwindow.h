#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsDropShadowEffect>

#include "BackgroundWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief The main application window for ScaffoldDesignerUI.
 *
 * This window provides the landing screen with a fixed initial size and
 * a responsive background image. It contains a menu bar with an "About"
 * option that displays information about the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window.
     * @param parent Optional parent widget.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroys the main window and cleans up the UI.
     */
    ~MainWindow();

protected:
    /**
     * @brief Event filter to detect hover events on interactive elements.
     *
     * This is used to apply or remove glow effects dynamically based on hover state.
     *
     * @param watched The object being watched for events.
     * @param event The event received.
     * @return true if the event was handled; otherwise false.
     */
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    /**
     * @brief Displays the About dialog with version and project overview.
     */
    void showAboutDialog();

    /**
     * @brief Slot triggered when the user clicks the "BEGIN" label.
     *
     * This slot is responsible for transitioning the application from the
     * landing screen to the main interface. It typically unlocks the window size
     * and may switch views, change the central widget, or perform initial setup.
     */
    void beginApp();


private:
    Ui::MainWindow *ui; ///< Pointer to the UI layout generated from Qt Designer.

    /**
     * @brief Glow effect used to highlight interactive text (e.g., "BEGIN") on hover.
     */
    QGraphicsDropShadowEffect* hoverGlowEffect = nullptr;
};

#endif // MAINWINDOW_H
