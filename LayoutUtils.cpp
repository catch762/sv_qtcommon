#include "LayoutUtils.h"

void printLayoutContents(QLayout *layout)
{
    if (!layout) {
        qDebug() << "layout: nullptr";
        return;
    }

    qDebug().noquote() << "Layout" << layout
                       << "type:" << layout->metaObject()->className()
                       << "itemCount:" << layout->count();

    for (int i = 0; i < layout->count(); ++i)
    {
        QLayoutItem *item = layout->itemAt(i);
        if (!item) {
            qDebug().noquote() << " [" << i << "] <null item>";
            continue;
        }

        if (QWidget *w = item->widget()) {
            qDebug().noquote() << " [" << i << "] widget:"
                               << w->metaObject()->className()
                               << "objectName=" << w->objectName()
                               << "ptr=" << w;
        } else if (item->layout()) {
            qDebug().noquote() << " [" << i << "] nested layout:"
                               << item->layout()->metaObject()->className()
                               << "ptr=" << item->layout();
        } else if (QSpacerItem *s = item->spacerItem()) {
            qDebug().noquote() << " [" << i << "] spacer:"
                               << "sizeHint=" << s->sizeHint()
                               << "minSize=" << s->minimumSize()
                               << "expandingDirections=" << s->expandingDirections();
        } else {
            qDebug().noquote() << " [" << i << "] unknown QLayoutItem";
        }
    }
}

void initLayoutSpacing(QLayout* layout, int margins, int spacing)
{
	layout->setContentsMargins(margins, margins, margins, margins);
	layout->setSpacing(spacing);
}

void deleteLastNItemsInLayout(QLayout *layout, int itemsToDelete)
{
    if (!layout || itemsToDelete <= 0) return;

    for(int i = 0; i < itemsToDelete; ++i)
    {
        int index = layout->count() - 1;
        if (index < 0)
        {
            break;
        }

        QLayoutItem* item = layout->takeAt(index);
        if (!item)
        {
            break; //if that happens, then there are no more items i guess
        }

        if (auto widget = item->widget())
        {
            widget->deleteLater();
        }

        if (auto spacer = item->spacerItem())
        {
            delete spacer;
        }

        if (auto childLayout = item->layout())
        {
            SV_WARN("deleteLastNItemsInLayout actually found nested layout. Its items will not be deleted, are u sure u wanted this?");
            childLayout->deleteLater();
        }

        delete item;
    }
}

void extractAllWidgetsFromLayoutWithNoSublayouts(QLayout *layout, QList<QWidget*>* outWidgets)
{
    while (QLayoutItem *item = layout->takeAt(0))
    {
        if (QWidget* w = item->widget())
        {
            if (outWidgets) outWidgets->push_back(w);
            w->setParent(nullptr);
        }
        else if (QLayout* childLayout = item->layout())
        {
            SV_ASSERT(false && "i thought this layout only contains widgets/spacers, and heres layout");
        }
        else if (QSpacerItem* spacer = item->spacerItem())
        {
            //nothing to do, only delete item in the end
        }
        else SV_UNREACHABLE();

        delete item; // only deletes the layout item
    }
}