#ifndef SCAFFOLDDESIGNER_H
#define SCAFFOLDDESIGNER_H

#include <QWidget>
#include <QStandardItem>

namespace Ui {
class ScaffoldDesigner;
}

/**
 * @brief ScaffoldDesigner is the main UI widget for building project scaffolds.
 *
 * This QWidget-based interface contains a tree view for managing folders, files, and libraries,
 * as well as control buttons for adding new elements. It is laid out in Qt Designer.
 */
class ScaffoldDesigner : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the ScaffoldDesigner widget.
     * @param parent The parent widget (optional).
     */
    explicit ScaffoldDesigner(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~ScaffoldDesigner();

public slots:
    /**
     * @brief Initializes the UI with a top-level project node and optional dependencies.
     * @param projectName The name of the root project or library.
     * @param dependencies The list of dependencies associated with the project.
     */
    void initializeWithProject(const QString &projectName, const QList<QString> &dependencies);

private slots:
    /**
     * @brief Handles the Add File button click.
     *
     * Adds a file under the currently selected folder in the tree.
     * Does nothing if a file is selected.
     */
    void onAddFileClicked();

    /**
     * @brief Handles the Add Folder button click.
     *
     * Adds a folder under the currently selected folder in the tree.
     * Does nothing if a file is selected.
     */
    void onAddFolderClicked();

private:
    Ui::ScaffoldDesigner *ui; ///< Pointer to the generated UI layout from Qt Designer.
};

#endif // SCAFFOLDDESIGNER_H
