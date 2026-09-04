#pragma once
#include "sv_qtcommon.h"

//if theres a nested layout, it gets deleted, but its contents are not.
void deleteLastNItemsInLayout(QLayout* layout, int itemsToDelete);

//This ignores layout items and their subwidgets, only visits immediate widget items.
template<typename WidgetIterateFunc>
	requires std::is_invocable_v< WidgetIterateFunc, QWidget* >
void iterateImmediateWidgets(QLayout* layout, const WidgetIterateFunc& iterateFunc)
{
	for (int i = 0; i < layout->count(); ++i)
	{
		if (auto* item = layout->itemAt(i))
		{
			if (auto* widget = item->widget())
			{
				iterateFunc(widget);
			}
		}
	}
}