#include "mainwindow.h"
#include "./ui_mainwindow.h"

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

    // Create the glow effect, but don't apply it yet
    hoverGlowEffect = new QGraphicsDropShadowEffect(this);
    hoverGlowEffect->setBlurRadius(15);
    hoverGlowEffect->setOffset(0, 0);
    hoverGlowEffect->setColor(QColor(255, 255, 255, 180));

    // Install the event filter to handle hover
    ui->beginButton->installEventFilter(this);

    // Connect begin to action
    connect(ui->beginButton, &QPushButton::clicked, this, &MainWindow::beginApp);

    // Set background colour to match image
    this->setStyleSheet("background-color: #0a0a0a;");

    // Global font fix
    QFont appFont("Segoe UI", 10);
    appFont.setBold(true);
    QApplication::setFont(appFont);

}

MainWindow::~MainWindow()
{
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

void MainWindow::beginApp()
{
    // Placeholder logic – replace with whatever you want to do
    qDebug() << "BEGIN clicked! Launching next phase...";

    // Example: unlock window resizing
    setMinimumSize(800, 600);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    resize(1024, 768);

    // TODO: Show new screen / change central widget
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->beginButton) {
        if (event->type() == QEvent::Enter) {
            // Apply glow on hover
            ui->beginButton->setGraphicsEffect(hoverGlowEffect);
        } else if (event->type() == QEvent::Leave) {
            // Remove glow when not hovered
            ui->beginButton->setGraphicsEffect(nullptr);
        }
    }
    return QMainWindow::eventFilter(watched, event);
}
