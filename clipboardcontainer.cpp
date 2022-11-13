#include "clipboardcontainer.h"
#include <QFile>
#include <QDateTime>

void ClipboardContainer::saveToFile()
{
    if (entryAction.isEmpty()) {return;};
    QDateTime date_time(QDateTime::currentDateTime());

    QFile file("dumps/clipboard_dump_"
               +QString::number(date_time.date().year())
               +QString::number(date_time.date().month())
               +QString::number(date_time.date().day())
               +QString::number(date_time.time().hour())
               +QString::number(date_time.time().minute())
               +QString::number(date_time.time().second())
               +".txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    out << date_time.date().toString() + " | " + date_time.time().toString() + "\n";

    for (auto& entry : entryAction)
    {
        out << "\n ##################################################################### \n";
        out << entry.full_data;
        out << "\n ##################################################################### \n";
    }

    file.close();
}

ClipboardContainer::ClipboardContainer(QWidget *parent):object_parent(parent)
{

}

QString ClipboardContainer::getShortDataStr(QAction *action) const
{
return entryAction[action].short_data;
}

QString ClipboardContainer::getLongDataStr(QAction *action) const
{
return entryAction[action].full_data;
}

void ClipboardContainer::addData(QAction *action, const QString &long_data)
{
    ClipboardMenuItem new_item{long_data.left(128) + "...", long_data};
    entryAction.insert(action, new_item);
    action->setText(new_item.short_data);
}

void ClipboardContainer::clear()
{
entryAction.clear();
}

QList<QAction *> ClipboardContainer::getActions() const
{
    return entryAction.keys();
}
