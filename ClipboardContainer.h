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

public:
    ClipboardContainer();

    QString getShortDataStr(QAction* action) const;
    QString getLongDataStr(QAction* action) const;

    void addData(QAction* action, const QString& long_data);
    QList<QAction*> getActions() const;
};

#endif // CLIPBOARDCONTAINER_H
