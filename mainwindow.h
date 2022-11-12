#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QClipboard>
#include <QCloseEvent>
#include "clipboardcontainer.h"

#ifndef QT_NO_SYSTEMTRAYICON

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void createTray();
    void createActions();
    void setIcon();
    void dataChanged();
    void entryTriggered();


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    ClipboardContainer clipboard_container;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QMenu *lastMenu;
    QAction *saveAction;
    QAction *clearAction;
    QAction *restoreAction;
    QAction *quitAction;

    QClipboard *clipboard;

    void historyCounterUpdate();

public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);


};

#endif // QT_NO_SYSTEMTRAYICON
#endif // MAINWINDOW_H
