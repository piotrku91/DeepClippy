#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QAction>
#include <QMenu>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), entryAction(64)
{
    ui->setupUi(this);
    createActions();
    createTray();
    setIcon();
    trayIcon->show();

    clipboard = QGuiApplication::clipboard();

    connect(clipboard, &QClipboard::dataChanged, this, &MainWindow::dataChanged);
}

void MainWindow::dataChanged()
{
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasText()) {
        QAction* new_action = new QAction("", this);
        connect(new_action, &QAction::triggered, this, &MainWindow::entryTriggered);

        clipboard_container.addData(new_action, mimeData->text());
        lastMenu->clear();
        lastMenu->addActions(clipboard_container.getActions());
    }
}

void MainWindow::entryTriggered()
{
    QAction* action = qobject_cast<QAction*>(sender());

    if (action)
    {
        clipboard->setText(clipboard_container.getLongDataStr(action), QClipboard::Clipboard);
    }

}

void MainWindow::createTray()
{
    trayIconMenu = new QMenu(this);
    lastMenu = new QMenu(this);

    lastMenu->setTitle("Last");
    lastMenu->clear();
    lastMenu->addActions(clipboard_container.getActions());

    trayIconMenu->addMenu(lastMenu);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:{
        break;
    }
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}

void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    restoreAction = new QAction(tr("&Settings"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit app"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::setIcon()
{
    Q_INIT_RESOURCE(general);
    QIcon icon(":/icons/heart.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

