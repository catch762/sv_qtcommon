#pragma once
#include "sv_qtcommon.h"

//if theres a nested layout, it gets deleted, but its contents are not.
void deleteLastNItemsInLayout(QLayout* layout, int itemsToDelete);