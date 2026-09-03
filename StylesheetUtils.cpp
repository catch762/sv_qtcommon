#include "StylesheetUtils.h"

void setQWidgetBackground(QWidget* widget, QColor backgroundColor, bool withObjectName)
{
    static const QString stylePattern =
        "QWidget%1{background-color: %2;}";

    QString finalStyle = stylePattern
        .arg(withObjectName ? "#" + widget->objectName() : "")
        .arg(backgroundColor.name(QColor::HexArgb));
    widget->setStyleSheet(finalStyle);
}

QString defaultPushButtonStyleWithoutMargins()
{
    return R"(
        QPushButton {
            background: rgb(251, 251, 251);
            border: 1px solid rgb(231, 231, 231);
            border-radius: 2px;
            padding: 0px;
            margin: 0px;
        }

        QPushButton:hover:!pressed {
            background: rgb(245, 245, 245);
        }

        QPushButton:pressed {
            background: rgb(238, 238, 238);
        }

        QPushButton:checked {
            background: rgb(0, 120, 215);
            color: white;
            border-color: rgb(0, 100, 180);
        }

        QPushButton:checked:hover {
            background: rgb(0, 130, 235);
        }

        QPushButton:checked:pressed {
            background: rgb(0, 100, 180);
        }
    )";
}

//adds diff to all r/g/b components, but if it would go beyound 0 or 255, diff is negated.
//so we can be sure resulting color is truly different by that much (maybe different in other direction but whatever)
QColor slightlyDiffColor(QColor in, const int diff = 7)
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

QString sanePushButtonStyleWithoutMargins(  QColorOpt _background,
                                            QColorOpt _border,
                                            QColorOpt _hoverColor,
                                            QColorOpt _checkedColor,
                                            QColorOpt _checkedFontColor,
                                            QColorOpt _checkedHoverColor)
{
    QColor background       = _background.value_or(QColor(251, 251, 251));
    QColor border           = _border.value_or(QColor(231, 231, 231));

    QColor hover            = _hoverColor.value_or( slightlyDiffColor(background, -6) );

    QColor pressed          = slightlyDiffColor(background, -13); //i dont give a fuck

    QColor checked          = _checkedColor.value_or(QColor(0,120,215));
    QColor checkedFont      = _checkedFontColor.value_or(Qt::white);
    QColor checkedHover     = _checkedHoverColor.value_or(QColor(0, 130, 235)); //i wouldnt give a fuck

    QColor checkedPressed   = slightlyDiffColor(background, -13); //i dont give a fuck

    return QString(R"(
        QPushButton {
            background: %1;
            border: 1px solid %2;
            border-radius: 2px;
            padding: 0px;
            margin: 0px;
        }

        QPushButton:hover:!pressed {
            background: %3;
        }

        QPushButton:pressed {
            background: %4;
        }

        QPushButton:checked {
            background: %5;
            color: %6;
            border-color: %2;
        }

        QPushButton:checked:hover {
            background: %7;
        }

        QPushButton:checked:pressed {
            background: %8;
        }
    )")
    .arg(background.name(QColor::HexArgb))
    .arg(border.name(QColor::HexArgb))
    .arg(hover.name(QColor::HexArgb))
    .arg(pressed.name(QColor::HexArgb))
    .arg(checked.name(QColor::HexArgb))
    .arg(checkedFont.name(QColor::HexArgb))
    .arg(checkedHover.name(QColor::HexArgb))
    .arg(checkedPressed.name(QColor::HexArgb));
}