#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QAction>
#include <QMenu>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , clipboard_container(this)
    , autoSaver(new QTimer(this))
    , settings(QSettings("PiotrQ Soft", "DeepClippy"))
{
    ui->setupUi(this);
    createActions();
    createTray();
    setIcon();
    trayIcon->show();

    clipboard = QGuiApplication::clipboard();

    connect(clipboard, &QClipboard::dataChanged, this, &MainWindow::dataChanged);

    connect(autoSaver, &QTimer::timeout, &clipboard_container, &ClipboardContainer::saveToFile);
    connect(ui->autobackup_checkbox, &QCheckBox::stateChanged, this, [this](auto state){
        if (state) {
            autoSaver->start(1000 * 60 * 60);
        } else
        {
            autoSaver->stop();
        }

    });
    connect(qApp, &QCoreApplication::aboutToQuit, this, [this](){
        saveAppSettings();
        if (ui->autobackup_checkbox_2->isChecked()) {
        clipboard_container.saveToFile();
        };

    });

    readAppSettings();
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
        historyCounterUpdate();
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void MainWindow::historyCounterUpdate()
{
    lastMenu->setTitle("History ("+QString::number(clipboard_container.getActions().size())+")");
}

void MainWindow::saveAppSettings()
{
    settings.beginGroup("Main settings");
    settings.setValue("autobackup_hour", ui->autobackup_checkbox->isChecked());
    settings.setValue("autobackup_app_close", ui->autobackup_checkbox_2->isChecked());
    settings.endGroup();
}

void MainWindow::readAppSettings()
{
     settings.beginGroup("Main settings");
     ui->autobackup_checkbox->setChecked(settings.value("autobackup_hour").toBool());
     ui->autobackup_checkbox_2->setChecked(settings.value("autobackup_app_close").toBool());
     settings.endGroup();
}

void MainWindow::createTray()
{
    trayIconMenu = new QMenu(this);
    lastMenu = new QMenu(this);

    lastMenu->clear();
    lastMenu->addActions(clipboard_container.getActions());

    historyCounterUpdate();
    trayIconMenu->addMenu(lastMenu);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(clearAction);
    trayIconMenu->addAction(saveAction);
    trayIconMenu->addSeparator();
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
        showNormal();
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
    clearAction = new QAction(tr("Clear history"), this);
    connect(clearAction, &QAction::triggered, this, [this](){

        for (auto& entry: clipboard_container.getActions())
        {
            disconnect(entry, &QAction::triggered, this, &MainWindow::entryTriggered);
        }

        clipboard_container.clear();
        lastMenu->clear();
        historyCounterUpdate();
    }
    );

    saveAction = new QAction(tr("&Save history"), this);
    connect(saveAction, &QAction::triggered, this,  [this](){
        clipboard_container.saveToFile();
    });

    restoreAction = new QAction(tr("&Settings"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit app"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::setIcon()
{
    Q_INIT_RESOURCE(general);
    QIcon icon(":/icons/notebook.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

