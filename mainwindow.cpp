#include "mainwindow.h"
#include "ProjectConfigurator.h"
#include "./ui_mainwindow.h"
#include "LandingPage.h"
#include "ScaffoldDesigner.h"

#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Set up the UI from Designer
    ui->setupUi(this);

    // Start the window in fixed-size landing page mode
    resize(1280, 720);                      // Set the window size
    setMinimumSize(1280, 720);              // Lock min
    setMaximumSize(1280, 720);              // Lock max (== fixed size)
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // Just in case

    // Connect the About action in the menu bar
    QAction* aboutAction = new QAction("About", this);
    ui->menuHelp->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);

    // Set background colour to match image
    this->setStyleSheet("background-color: #0a0a0a;");

    // Global font fix
    QFont appFont("Segoe UI", 10);
    appFont.setBold(true);
    QApplication::setFont(appFont);

    // Connect LandingPage signal to MainWindow slot:
    LandingPage *landingPage = qobject_cast<LandingPage*>(findChild<QWidget*>("landingPage"));
    if (landingPage) {
        connect(landingPage, &LandingPage::projectConfigured,
                this, &MainWindow::onProjectConfigured);
    }

    // Retrieve the pointer to your Forge view
    ScaffoldDesigner *forgeView = qobject_cast<ScaffoldDesigner*>(findChild<QWidget*>("forgeView"));
    if (forgeView) {
        connect(this, &MainWindow::projectConfigurationReady,
                forgeView, &ScaffoldDesigner::initializeWithProject);
    }
    else {
        qDebug() << "Forge view not found in stacked widget!";
    }

    // Set view to landing page on construction
    ui->stackedWidget->setCurrentWidget(landingPage);

}

MainWindow::~MainWindow()
{
    qDebug() << "Main Close";
    delete ui;
}

void MainWindow::showAboutDialog()
{
    QString message =
        "<b>Scaffold Forge</b><br>"
        "<i>Pre-Release v0.1</i><br><br>"
        "ScaffoldDesignerUI is an intuitive Qt-based tool that lets you visually design "
        "and configure your software project structure. Organize projects, libraries, "
        "folders, classes, functions, and namespaces to automatically generate a complete "
        "scaffolding file for rapid setup.";

    QMessageBox::about(this, "About Scaffold Forge", message);
}

void MainWindow::onProjectConfigured(const QString &projectName, const QList<QString> &dependencies)
{
    qDebug() << "Received configuration:" << projectName << dependencies;
    // Retrieve the ForgeView widget by its object name.
    QWidget *forgeView = ui->stackedWidget->findChild<QWidget*>("forgeView");

    // Set new view
    ui->stackedWidget->setCurrentWidget(forgeView);

    // Forward the configuration data via the signal
    emit projectConfigurationReady(projectName, dependencies);
}
