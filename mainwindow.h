#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsDropShadowEffect>

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

signals:
    // To forward the configuration data to another component.
    void projectConfigurationReady(const QString &projectName, const QList<QString> &dependencies);

private slots:
    /**
     * @brief Displays the About dialog with version and project overview.
     */
    void showAboutDialog();

    /**
     * @brief Slot to handle when a project is configured in the LandingPage.
     * @param projectName The name of the project.
     * @param dependencies List of selected dependencies.
     */
    void onProjectConfigured(const QString &projectName, const QList<QString> &dependencies);

private:
    Ui::MainWindow *ui; ///< Pointer to the UI layout generated from Qt Designer.

};

#endif // MAINWINDOW_H
