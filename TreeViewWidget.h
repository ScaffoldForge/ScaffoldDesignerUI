#ifndef TREEVIEWWIDGET_H
#define TREEVIEWWIDGET_H

#include <QTreeView>
#include <QStandardItemModel>

/**
 * @brief A QTreeView subclass supporting context-menu driven creation of folders and files.
 *
 * This class extends QTreeView to let the user add folders or files via a right-click context menu.
 * Each new item is created with a default name ("NewFolder" or "NewFile") and marked as "new" so that
 * when its name is edited, the change is validated. If the new name is invalid (empty or duplicates an
 * existing sibling), the item is reverted to its previous valid name.
 *
 * Use setRootFolderName() to define the externally provided root folder name.
 */
class TreeViewWidget : public QTreeView {
    Q_OBJECT
public:
    /**
     * @brief Constructs a TreeViewWidget.
     * @param parent The parent widget (default is nullptr).
     */
    explicit TreeViewWidget(QWidget *parent = nullptr);

    /**
     * @brief Sets or updates the name of the root folder.
     *
     * If the root item does not exist it is created; otherwise, its text is updated.
     * The root item is protected from editing.
     *
     * @param name The desired root folder name.
     */
    void setRootFolderName(const QString &name);

    /**
     * @brief Adds a new folder or file as a child of the specified parent item.
     *
     * The new item is created with a default name ("NewFolder" or "NewFile") and flagged as "new"
     * for validation once the user edits the name.
     *
     * @param parentItem The parent item under which to add the new item.
     * @param isFolder If true, the new item is treated as a folder; otherwise, as a file.
     */
    void addNewItem(QStandardItem *parentItem, bool isFolder);

    /**
     * @brief Overridden edit() method that saves the previous text before inline editing starts.
     *
     * @param index The model index being edited.
     * @param trigger The edit trigger.
     * @param event The associated event (default is nullptr).
     * @return True if editing was successfully started.
     */
    bool edit(const QModelIndex &index,
              QAbstractItemView::EditTrigger trigger,
              QEvent *event = nullptr) override;

protected:
    /**
     * @brief Overridden context menu event to provide a right-click menu.
     *
     * This method displays a context menu with options to add a folder or file.
     *
     * @param event The context menu event.
     */
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    /**
     * @brief Validates an item when its text changes.
     *
     * If the new name is empty or duplicates a sibling, the item's text is reverted to its previous value.
     *
     * @param item The item whose text was changed.
     */
    void onItemChanged(QStandardItem *item);

private:
    QStandardItemModel *model;      ///< The underlying data model.
    QStandardItem *rootItem;        ///< The externally defined root item.

    // Custom data roles.
    static constexpr int NewItemRole = Qt::UserRole + 1;       ///< Marks items that are new (and under validation).
    static constexpr int PreviousNameRole = Qt::UserRole + 2;    ///< Holds the previous (valid) name for reverting.
};

#endif // TREEVIEWWIDGET_H
