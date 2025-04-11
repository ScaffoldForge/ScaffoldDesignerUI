#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>

namespace Ui {
class LandingPage;
}

/**
 * @class LandingPage
 * @brief Widget representing the landing page for the application.
 *
 * This widget provides the initial landing screen and handles launching the
 * project configurator dialog. If the user accepts the configuration, it emits
 * a signal with the project details.
 */
class LandingPage : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a LandingPage widget.
     * @param parent The parent widget (default is nullptr).
     */
    explicit LandingPage(QWidget *parent = nullptr);

    /**
     * @brief Destroys the LandingPage widget and cleans up resources.
     */
    ~LandingPage();

signals:
    /**
     * @brief Emitted when the project configurator is accepted.
     * @param projectName The name of the project.
     * @param dependencies A list of selected project dependencies.
     */
    void projectConfigured(const QString &projectName, const QList<QString> &dependencies);

private slots:
    /**
     * @brief Launches the project configurator dialog.
     *
     * If the user accepts the configuration, emits the projectConfigured signal.
     */
    void launchProjectConfigurator();

private:
    Ui::LandingPage *ui; ///< Pointer to the UI layout generated from Qt Designer.
};

#endif // LANDINGPAGE_H
