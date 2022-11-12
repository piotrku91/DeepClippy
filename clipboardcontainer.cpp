#include "clipboardcontainer.h"

ClipboardContainer::ClipboardContainer()
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

QList<QAction *> ClipboardContainer::getActions() const
{
    return entryAction.keys();
}
