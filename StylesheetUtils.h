#include "QtCommon.h"

inline void setQWidgetBackground(QWidget* widget, QColor backgroundColor, bool withObjectName = false)
{
    static const QString stylePattern =
        "QWidget%1{background-color: %2;}";

    QString finalStyle = stylePattern
        .arg(withObjectName ? "#" + widget->objectName() : "")
        .arg(backgroundColor.name(QColor::HexArgb));
    widget->setStyleSheet(finalStyle);
}