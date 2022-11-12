#ifndef CLIPBOARDCONTAINER_H
#define CLIPBOARDCONTAINER_H

#include <QObject>
#include <QAction>
#include <QList>

struct ClipboardMenuItem
{
    QString short_data;
    QString full_data;
};

class ClipboardContainer : public QObject
{
    Q_OBJECT

private:
    QMap<QAction*, ClipboardMenuItem> entryAction;
    QWidget* object_parent;

public:
    ClipboardContainer(QWidget *parent);

    QString getShortDataStr(QAction* action) const;
    QString getLongDataStr(QAction* action) const;

    void addData(QAction* action, const QString& long_data);
    void clear();
    void saveToFile();

    QList<QAction*> getActions() const;
};

#endif // CLIPBOARDCONTAINER_H
