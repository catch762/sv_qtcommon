#include "QtCommon.h"

// Produces string in stylesheet format
QString str(const QColor& color);

// Adds diff to all r/g/b components, but if it would go too far beyound 0 or 255,
// diff is negated. So we can be sure resulting color is truly different by that much
// (maybe different in other direction but whatever)
QColor slightlyDiffColor(QColor in, const int diff = 7);

void setQWidgetBackground(QWidget* widget, QColor backgroundColor, bool withObjectName = false);

// I hate setting colors up via fucking stylesheets, i d rather do this.
// Default values here are the same as default Qt colors.
struct PushButtonSpec
{
    QColor background           = QColor(251, 251, 251);
    QColor font                 = QColor(27, 27, 27);
    QColor border               = QColor(231, 231, 231);
    QColor hover                = QColor(245, 245, 245);
    QColor pressed              = QColor(238, 238, 238);

    QColor checkedBackground    = QColor(0, 120, 215);
    QColor checkedFont          = QColor(255, 255, 255);
    QColor checkedBorder        = QColor(0, 95, 177);
    QColor checkedHover         = QColor(0, 130, 235);
    QColor checkedPressed       = QColor(0, 100, 180);
};

// Besides applying spec, it produces what i feel should be default button - without stupid margins 
QString makeStyleFromButtonSpec(const PushButtonSpec& spec);

const QString& myDefaultPushButtonStyle();