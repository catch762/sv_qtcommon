#include "StylesheetUtils.h"

QString str(const QColor& color)
{
    return color.name(QColor::HexArgb);
}

QColor slightlyDiffColor(QColor in, const int diff)
{
    auto change = [&](int in)
    {
        auto normal = in + diff;
        auto alt    = in - diff;

        int acceptedOutside = diff / 2;

        bool tooHigh    = normal > 255  && normal - 255     > acceptedOutside;
        bool tooLow     = normal < 0    && std::abs(normal) > acceptedOutside;

        int final = (tooHigh || tooLow) ? normal : alt;
        return std::clamp(final, 0, 255);
    };

    return QColor(change(in.red()), change(in.green()), change(in.blue()));
}

void setQWidgetBackground(QWidget* widget, QColor backgroundColor, bool withObjectName)
{
    static const QString stylePattern =
        "QWidget%1{background-color: %2;}";

    QString finalStyle = stylePattern
        .arg(withObjectName ? "#" + widget->objectName() : "")
        .arg(backgroundColor.name(QColor::HexArgb));
    widget->setStyleSheet(finalStyle);
}


QString makeStyleFromButtonSpec(const PushButtonSpec& spec)
{
    return QString(R"(
        QPushButton {
            background: %1;
            color: %2;
            border: 1px solid %3;
            border-radius: 2px;
            padding: 0px;
            margin: 0px;
        }

        QPushButton:hover:!pressed {
            background: %4;
        }

        QPushButton:pressed {
            background: %5;
        }

        QPushButton:checked {
            background: %6;
            color: %7;
            border-color: %8;
        }

        QPushButton:checked:hover {
            background: %9;
        }

        QPushButton:checked:pressed {
            background: %10;
        }
    )")
    .arg(str(spec.background))
    .arg(str(spec.font))
    .arg(str(spec.border))
    .arg(str(spec.hover))
    .arg(str(spec.pressed))
    .arg(str(spec.checkedBackground))
    .arg(str(spec.checkedFont))
    .arg(str(spec.checkedBorder))
    .arg(str(spec.checkedHover))
    .arg(str(spec.checkedPressed));
}

const QString& myDefaultPushButtonStyle()
{
    static const QString style = makeStyleFromButtonSpec({});
    return style;
}
