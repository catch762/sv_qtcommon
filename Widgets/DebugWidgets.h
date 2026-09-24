#pragma once
#include "QtCommon.h"

inline QWidget* makePaletteDisplayWidget(QPalette palette) {
    auto widget = new QWidget();
    auto mainLayout = new QVBoxLayout(widget);

#ifdef ADD_COLOR_ROLE
    static_assert(false, "i expect this macro to be undefined");
#endif

    // Macro to add a color role display
#define ADD_COLOR_ROLE(role) \
    do { \
        QColor color = palette.color(QPalette::role); \
        auto hLayout = new QHBoxLayout(); \
        auto label = new QLabel(#role); \
        hLayout->addWidget(label); \
        auto colorFrame = new QFrame(); \
        colorFrame->setFixedSize(50, 30); \
        colorFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken); \
        colorFrame->setPalette(QPalette(color)); \
        colorFrame->setAutoFillBackground(true); \
        hLayout->addWidget(colorFrame); \
        auto hexLabel = new QLabel(color.name(QColor::HexRgb)); \
        hLayout->addWidget(hexLabel); \
        hLayout->addStretch(); \
        mainLayout->addLayout(hLayout); \
    } while (0)

    // Iterate over all valid ColorRoles (0 to NColorRoles-1, skipping NoRole=17)
    for (int i = 0; i < QPalette::NColorRoles; ++i) {
        QPalette::ColorRole role = static_cast<QPalette::ColorRole>(i);
        if (role == QPalette::NoRole) continue;

        switch (role) {
            case QPalette::Window: ADD_COLOR_ROLE(Window); break;
            case QPalette::WindowText: ADD_COLOR_ROLE(WindowText); break;
            case QPalette::Base: ADD_COLOR_ROLE(Base); break;
            case QPalette::AlternateBase: ADD_COLOR_ROLE(AlternateBase); break;
            case QPalette::ToolTipBase: ADD_COLOR_ROLE(ToolTipBase); break;
            case QPalette::ToolTipText: ADD_COLOR_ROLE(ToolTipText); break;
            case QPalette::PlaceholderText: ADD_COLOR_ROLE(PlaceholderText); break;
            case QPalette::Text: ADD_COLOR_ROLE(Text); break;
            case QPalette::Button: ADD_COLOR_ROLE(Button); break;
            case QPalette::ButtonText: ADD_COLOR_ROLE(ButtonText); break;
            case QPalette::BrightText: ADD_COLOR_ROLE(BrightText); break;
            case QPalette::Light: ADD_COLOR_ROLE(Light); break;
            case QPalette::Midlight: ADD_COLOR_ROLE(Midlight); break;
            case QPalette::Dark: ADD_COLOR_ROLE(Dark); break;
            case QPalette::Mid: ADD_COLOR_ROLE(Mid); break;
            case QPalette::Shadow: ADD_COLOR_ROLE(Shadow); break;
            case QPalette::Highlight: ADD_COLOR_ROLE(Highlight); break;
            case QPalette::HighlightedText: ADD_COLOR_ROLE(HighlightedText); break;
            case QPalette::Link: ADD_COLOR_ROLE(Link); break;
            case QPalette::LinkVisited: ADD_COLOR_ROLE(LinkVisited); break;
#ifdef QPaletteAccent // Qt 6.6+
            case QPalette::Accent: ADD_COLOR_ROLE(Accent); break;
#endif
            default: break;
        }
    }

#undef ADD_COLOR_ROLE

    mainLayout->addStretch();
    widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    return widget;
}

inline QWidget* createThemeIconsWidget() {
    auto widget = new QWidget();
    auto mainLayout = new QVBoxLayout(widget);

    auto scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    auto scrollWidget = new QWidget();
    scroll->setWidget(scrollWidget);
    mainLayout->addWidget(scroll);

    auto layout = new QVBoxLayout(scrollWidget);

    const int cols = 8;
    auto rowLayout = new QHBoxLayout();
    int col = 0;

    // Iterate over all QIcon::ThemeIcon values (0 to 149 as per Qt 6.10.2 docs)
    for (int i = 0; i < 150; ++i) {
        QIcon icon = QIcon::fromTheme(static_cast<QIcon::ThemeIcon>(i));
        if (!icon.isNull()) {
            auto label = new QLabel();
            label->setPixmap(icon.pixmap(32, 32));
            label->setFixedSize(48, 48);
            label->setAlignment(Qt::AlignCenter);
            label->setToolTip(QString("ThemeIcon(%1)").arg(i));
            rowLayout->addWidget(label);
            ++col;
            if (col >= cols) {
                layout->addLayout(rowLayout);
                rowLayout = new QHBoxLayout();
                col = 0;
            }
        }
    }
    if (col > 0) {
        layout->addLayout(rowLayout);
    }

    return widget;
}