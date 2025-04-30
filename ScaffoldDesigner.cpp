// ScaffoldDesigner.cpp

#include "ScaffoldDesigner.h"
#include "ui_ScaffoldDesigner.h"
#include "TreeViewWidget.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QMenu>
#include <QAction>

// Constructor
ScaffoldDesigner::ScaffoldDesigner(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ScaffoldDesigner)
{
    ui->setupUi(this);

    // Assign icons to the Add buttons
    ui->addFileButton->setIcon(QIcon(":/imgs/images/addFileButton.png"));
    ui->addFolderButton->setIcon(QIcon(":/imgs/images/addFolderButton.png"));

    // TreeView visual settings
    ui->directoryTreeView->setHeaderHidden(true);
    ui->directoryTreeView->setIndentation(20);

    // Button signal-slot connections
    connect(ui->addFileButton, &QPushButton::clicked,
            this, &ScaffoldDesigner::onAddFileClicked);
    connect(ui->addFolderButton, &QPushButton::clicked,
            this, &ScaffoldDesigner::onAddFolderClicked);
}

// Destructor
ScaffoldDesigner::~ScaffoldDesigner()
{
    delete ui;
}

// Initializes the tree with a top-level project node and optional dependencies
void ScaffoldDesigner::initializeWithProject(const QString &projectName,
                                             const QList<QString> &dependencies)
{
    qDebug() << "ScaffoldDesigner initialized with project:" << projectName;

    // Ensure the tree view is cast to the correct subclass
    auto *tree = qobject_cast<TreeViewWidget*>(ui->directoryTreeView);
    if (tree) {
        tree->setRootFolderName(projectName);
        // Dependencies could be handled here in the future if needed
    } else {
        qFatal("Failed to cast ui->directoryTreeView to TreeViewWidget*");
    }
}

// Slot for Add File button
void ScaffoldDesigner::onAddFileClicked()
{
    auto *tree = qobject_cast<TreeViewWidget*>(ui->directoryTreeView);
    if (!tree) return;

    const QModelIndex index = tree->currentIndex();
    QStandardItem *selectedItem = tree->getModel()->itemFromIndex(index);

    // Default to root if no selection
    if (!selectedItem) {
        selectedItem = tree->getRootItem();
    }

    // Prevent adding a file under another file
    if (!selectedItem->data(IsFolderRole).toBool()) {
        qDebug() << "Selected item is a file — cannot add to it.";
        return;
    }

    tree->addNewItem(selectedItem, false); // Add file
}

// Slot for Add Folder button
void ScaffoldDesigner::onAddFolderClicked()
{
    auto *tree = qobject_cast<TreeViewWidget*>(ui->directoryTreeView);
    if (!tree) return;

    const QModelIndex index = tree->currentIndex();
    QStandardItem *selectedItem = tree->getModel()->itemFromIndex(index);

    // Default to root if no selection
    if (!selectedItem) {
        selectedItem = tree->getRootItem();
    }

    // Prevent adding a folder under a file
    if (!selectedItem->data(IsFolderRole).toBool()) {
        qDebug() << "Selected item is a file — cannot add to it.";
        return;
    }

    tree->addNewItem(selectedItem, true); // Add folder
}
