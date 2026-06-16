#include "LayoutUtils.h"

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