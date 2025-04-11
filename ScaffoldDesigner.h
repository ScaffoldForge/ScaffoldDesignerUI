#ifndef SCAFFOLDDESIGNER_H
#define SCAFFOLDDESIGNER_H

#include <QWidget>
#include <QStandardItem>


namespace Ui {
class ScaffoldDesigner;
}

/**
 * @brief The ScaffoldDesigner widget.
 *
 * This widget is designed in Qt Designer and should have its layout set up
 * (for example, with a QSplitter dividing a directory tree view and a right panel).
 */
class ScaffoldDesigner : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the ScaffoldDesigner widget.
     * @param parent The parent widget (default is nullptr).
     */
    explicit ScaffoldDesigner(QWidget *parent = nullptr);

    /**
     * @brief Destructor for ScaffoldDesigner.
     */
    ~ScaffoldDesigner();

public slots:
    /**
     * @brief Initializes the directory tree with the provided project name and dependencies.
     * @param projectName The project name.
     * @param dependencies The list of selected dependencies.
     */
    void initializeWithProject(const QString &projectName, const QList<QString> &dependencies);

private slots:
    /**
     * @brief Slot triggered when the user clicks the Add File button.
     *
     * This slot creates a new file item (with a default name and icon) and adds it to the currently
     * selected folder in the directory tree view. If no folder is selected, it adds the file to the root.
     */
    void onAddFileClicked();

    /**
     * @brief Slot triggered when the user clicks the Add Folder button.
     *
     * This slot creates a new folder item (with a default name and icon) and adds it as a child of the
     * currently selected folder in the directory tree view. If no folder is selected, the folder is added
     * to the root.
     */
    void onAddFolderClicked();

private:
    Ui::ScaffoldDesigner *ui; ///< Pointer to the UI layout from Qt Designer.
};

#endif // SCAFFOLDDESIGNER_H
