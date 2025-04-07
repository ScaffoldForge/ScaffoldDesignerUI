#ifndef PROJECTCONFIGURATOR_H
#define PROJECTCONFIGURATOR_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

/**
 * @brief The ProjectConfigurator class provides a dialog to configure a new project.
 *
 * This dialog allows users to specify a project name and select from a list of available
 * C++ libraries (e.g., Boost, OpenCV, OpenMP, Eigen) as dependencies for the project.
 */
class ProjectConfigurator : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a ProjectConfigurator dialog.
     * @param parent The parent widget (default is nullptr).
     */
    explicit ProjectConfigurator(QWidget *parent = nullptr);

    /**
     * @brief Destructor for the ProjectConfigurator dialog.
     */
    ~ProjectConfigurator();

    /**
     * @brief Retrieves the project name entered by the user.
     * @return A QString containing the project name.
     */
    QString getProjectName() const;

    /**
     * @brief Retrieves the list of selected dependencies.
     * @return A QList of QString objects representing the selected libraries.
     */
    QList<QString> getSelectedDependencies() const;

private slots:
    /**
     * @brief Slot triggered when the OK button is clicked.
     *
     * This slot accepts the dialog, allowing the configuration settings to be processed.
     */
    void handleAccepted();

    /**
     * @brief Slot triggered when the Cancel button is clicked.
     *
     * This slot rejects the dialog, canceling the configuration process.
     */
    void handleRejected();

private:
    Ui::Dialog *ui; ///< Pointer to the UI elements generated from Qt Designer.
};

#endif // PROJECTCONFIGURATOR_H
