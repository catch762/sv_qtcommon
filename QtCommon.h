#pragma once
#include "sv_common.h"

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QFrame>
#include <QSlider>
#include <QPointer>
#include <QScrollArea>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDir>
#include <QMessageBox>

SV_DECL_OPT(QString)
SV_DECL_OPT(QJsonArray)
SV_DECL_OPT(QJsonObject)
SV_DECL_OPT(QJsonValue)
SV_DECL_OPT(QVariant)
SV_DECL_OPT(QDir)


using QtTypeIndex = int;
SV_DECL_OPT(QtTypeIndex);

//returns QMetaType::UnknownType (0) if unregistered
template <typename T>
inline QtTypeIndex qtTypeId()
{
    return QMetaType::fromType<T>().id();
}

//returns "" if unregistered
template <typename T>
inline QString qtTypeName()
{
    return QMetaType::fromType<T>().name();
}

inline QString qtTypeName(int typeIndex)
{
    return QString::fromLatin1(QMetaType(typeIndex).name());
}

//this is basic check, maybe name for this class is still not set even if id is obtainable
template <typename T>
inline bool qtTypeIsRegistered()
{
    return qtTypeId<T>() != QMetaType::UnknownType;
}

template <typename T>
inline bool qtTypeIsRegisteredAndNamed()
{
    return qtTypeIsRegistered<T>() && !qtTypeName<T>().isEmpty();
}

template <typename T>
inline bool canConvert(const QVariant &qVariant)
{
    return qVariant.canConvert(QMetaType::fromType<T>());
}

template<typename T>
inline bool holdsType(const QVariant& v)
{
    return v.typeId() == qtTypeId<T>();
}

template <typename T>
inline std::optional<T> getValueOpt(const QVariant &qVariant)
{
    if(canConvert<T>(qVariant)) return qVariant.value<T>();
    else return {};
}

template <typename T>
inline T getValueOr(const QVariant &qVariant, const T& defaultVal = {})
{
    if(canConvert<T>(qVariant)) return qVariant.value<T>();
    else return defaultVal;
}

template <typename T>
inline QString qtTypeInfo()
{
    return QString("[type id=%1, name=%2]").arg(qtTypeId<T>()).arg(qtTypeName<T>());
} 

inline QString qtTypeInfo(QtTypeIndex typeIndex)
{
    return QString("[type id=%1, name=%2]").arg(typeIndex).arg(qtTypeName(typeIndex));
}

inline QString qVariantInfo(const QVariant &var)
{
    return QString("QVariant[typeid=%1][typename=%2][tostring=%3]").arg(var.typeId()).arg(var.typeName()).arg(var.toString());
}

inline double getSliderValue01(const QSlider* slider)
{
    if (!slider) return 0.0;
    
    int min = slider->minimum();
    int max = slider->maximum();
    int val = slider->value();
    
    if (min == max) return 0.0;
    
    return static_cast<double>(val - min) / (max - min);
}

inline double getSliderValue11(const QSlider* slider)
{
    return value01To11(getSliderValue01(slider));
}

inline void setSliderValue01(QSlider* slider, double value01)
{
    if (!slider) return;
    value01 = std::clamp(value01, 0.0, 1.0);
    
    int min = slider->minimum();
    int max = slider->maximum();
    
    if (min == max) {
        slider->setValue(min);
        return;
    }
    
    int value = min + static_cast<int>(value01 * (max - min));
    slider->setValue(value);
}

inline void setSliderValue11(QSlider* slider, double value11)
{
    setSliderValue01(slider, value11To01(value11));
}

inline void initLayoutSpacing(QLayout* layout, int margins = 0, int spacing = 0)
{
    layout->setContentsMargins(margins,margins,margins,margins);
    layout->setSpacing(spacing);
}

inline QWidget* makePaletteDisplayWidget(QPalette palette) {
    auto widget = new QWidget();
    auto mainLayout = new QVBoxLayout(widget);

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

//'WidgetT is indeed QWidget-derived, but is not just QWidget'
template <typename WidgetT>
concept IsConcreteWidget =
    std::derived_from<std::decay_t<WidgetT>, QWidget> &&
    !std::same_as    <std::decay_t<WidgetT>, QWidget>;

//overwrites content
inline bool writeStringToFile(const QString &content, const QString &filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        SV_ERROR(std::format("Couldnt write string to file: couldnt open file to write: {}", filepath.toStdString()));
        return false; // failed to open
    }

    auto totalBytes = content.toUtf8().size();
    auto writtenBytes = file.write(content.toUtf8());
    file.close();

    bool writtenOk = writtenBytes == totalBytes;

    if (!writtenOk)
    {
        SV_ERROR(std::format("Couldnt write string to file: only written [{}/{} bytes] to: {}", writtenBytes, totalBytes, filepath.toStdString()));
    }

    return writtenOk;
}

inline void extractAllWidgetsFromLayoutAndDeleteNestedLayouts(QLayout *layout, QList<QWidget*>* outWidgets = nullptr)
{
    while (QLayoutItem *item = layout->takeAt(0))
    {
        if (QWidget *w = item->widget())
        {
            if (outWidgets) outWidgets->push_back(w);
            w->setParent(nullptr);
        }
        else if (QLayout *childLayout = item->layout())
        {
            extractAllWidgetsFromLayoutAndDeleteNestedLayouts(childLayout, outWidgets);
            delete childLayout;
        }
        delete item; // only deletes the layout item
    }
}

//Addition to logger
#define SV_MSGBOX_LOG(text)     {SV_LOG(text);   QMessageBox::information(nullptr, "Information", QString::fromStdString(text));}
#define SV_MSGBOX_WARN(text)    {SV_WARN(text);  QMessageBox::warning(nullptr, "Warning", QString::fromStdString(text));}
#define SV_MSGBOX_ERROR(text)   {SV_ERROR(text); QMessageBox::critical(nullptr, "Error", QString::fromStdString(text));}

inline void applyWidgetBorder(QWidget *widget) {
    widget->setStyleSheet(
        "background-color: transparent;"
        "border: 1px solid red;"
        "border-radius: 0px;"
        "padding: 0px;"
    );
}

inline void printLayoutContents(QLayout *layout)
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