#pragma once
#include "QtCommon.h"
#include <QMetaEnum>
#include "StdFormattersForQt.h"

inline std::string widgetInfo(QWidget* w)
{
    return std::format(
        "[{}]: name='{}', size=[{}; {}], minSize=[{}; {}], maxSize=[{}; {}], "
        "sizeHint[{}; {}], minSizeHint[{}; {}], "
        "sizePolicy[hor {}; ver {}], stretch[hor {}; ver {}]",

        w->metaObject()->className(),
        w->objectName(),
        w->width(), 
        w->height(),
        w->minimumWidth(), 
        w->minimumHeight(),
        w->maximumWidth(), 
        w->maximumHeight(),

        w->sizeHint().width(), 
        w->sizeHint().height(),
        w->minimumSizeHint().width(), 
        w->minimumSizeHint().height(),

        QMetaEnum::fromType<QSizePolicy::Policy>().valueToKey(w->sizePolicy().horizontalPolicy()),
        QMetaEnum::fromType<QSizePolicy::Policy>().valueToKey(w->sizePolicy().verticalPolicy()),
        w->sizePolicy().horizontalStretch(), 
        w->sizePolicy().verticalStretch()
    );
}

//************
//  QSlider
//************

inline double getSliderValue01(const QSlider* slider)
{
    SV_ASSERT(slider);

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
//Means we will use it with functions like 'getSliderValue01' above,
//so we dont care about actual int values.
inline QSlider* makeSliderForNormDouble(QWidget* parent = nullptr, double initialVal01 = 0.0)
{
    QSlider* slider = new QSlider(Qt::Horizontal, parent);
    slider->setMinimum(-10000);
    slider->setMaximum(+10000);
    setSliderValue01(slider, initialVal01);
    return slider;
}

//returns leftmost and rightmost pixel
inline std::pair<int, int> sliderHorizontalHandleCenterRange(const QSlider* slider)
{
    QStyleOptionSlider opt;
    opt.initFrom(slider);

    opt.orientation = Qt::Horizontal;
    opt.minimum = slider->minimum();
    opt.maximum = slider->maximum();
    opt.sliderPosition = slider->sliderPosition();
    opt.upsideDown = slider->layoutDirection() == Qt::RightToLeft;

    const QStyle* style = slider->style();

    const QRect handle = style->subControlRect(
        QStyle::CC_Slider,
        &opt,
        QStyle::SC_SliderHandle,
        slider);

    const int handleLength = handle.width();

    // This is the same basic span used by QSlider/QStyle for horizontal sliders.
    const int sliderMin = slider->rect().left();
    const int sliderMax = slider->rect().right() - handleLength + 1;

    //yes its not a typo, + both times
    return {
        sliderMin + handleLength / 2,
        sliderMax + handleLength / 2
    };
}

//******************
//  QDoubleSpinBox
//******************

inline QDoubleSpinBox* makeStandardSpinbox(QWidget* parent = nullptr, double min = -DBL_MAX, double max = DBL_MAX, double initialVal = 0, int decimals = 3)
{
    normalizeRange(min, max, initialVal);

    QDoubleSpinBox* spinbox = new QDoubleSpinBox(parent);

    spinbox->setKeyboardTracking(false);
    spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    spinbox->setDecimals(decimals);
    spinbox->setSingleStep(0.1);

    spinbox->setRange(min, max);
    spinbox->setValue(initialVal);

    return spinbox;
    //spinbox->setRange(MinFloatInUI, MaxFloatInUI);
}

inline double getSpinboxValue01(const QDoubleSpinBox* spinbox)
{
    return getValue01Clamped(spinbox->value(), spinbox->minimum(), spinbox->maximum());
}
inline double getSpinboxValue11(const QDoubleSpinBox* spinbox)
{
    return getValue11Clamped(spinbox->value(), spinbox->minimum(), spinbox->maximum());
}
inline void setSpinboxValue01(QDoubleSpinBox* spinbox, double value01)
{
    double val = mix(spinbox->minimum(), spinbox->maximum(), value01);
    spinbox->setValue(val);
}
inline void setSpinboxValue11(QDoubleSpinBox* spinbox, double value11)
{
    setSpinboxValue01(spinbox, value11To01(value11));
}