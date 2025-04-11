#include "ScaffoldDesigner.h"
#include "ui_ScaffoldDesigner.h"
#include "TreeViewWidget.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QMenu>
#include <QAction>

ScaffoldDesigner::ScaffoldDesigner(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ScaffoldDesigner)
{
    ui->setupUi(this);

    // Add style to insertion buttons
    ui->addFileButton->setIcon(QIcon(":/imgs/images/addFileButton.png"));
    ui->addFolderButton->setIcon(QIcon(":/imgs/images/addFolderButton.png"));

    // Set tree styles
    ui->directoryTreeView->setHeaderHidden(true);
    ui->directoryTreeView->setIndentation(20);

    // Connect buttons
    connect(ui->addFileButton, &QPushButton::clicked, this, &ScaffoldDesigner::onAddFileClicked);
    connect(ui->addFolderButton, &QPushButton::clicked, this, &ScaffoldDesigner::onAddFolderClicked);
}

ScaffoldDesigner::~ScaffoldDesigner()
{
    delete ui;
}

void ScaffoldDesigner::initializeWithProject(const QString &projectName, const QList<QString> &dependencies)
{
    // Log the initialization
    qDebug() << "ScaffoldDesigner initialized with project:" << projectName;

    // Load parent with project top level
    TreeViewWidget *tree = qobject_cast<TreeViewWidget*>(ui->directoryTreeView);
    if (tree) {
        tree->setRootFolderName(projectName);
    } else {
        qFatal("Failed to cast ui->treeView to TreeViewWidget*");
    }
}


void ScaffoldDesigner::onAddFileClicked()
{
    qDebug() << "ADD FILE";
}

void ScaffoldDesigner::onAddFolderClicked()
{
    qDebug() << "ADD FOLDER";
}

