#pragma once
#include "QtCommon.h"
#include "StdFormattersForQt.h"

inline std::string widgetInfo(QWidget* w)
{
    return std::format("[{}]: name='{}', w=[min {}; cur {}; max {}], h=[min {}; cur {}; max {}], sizeHint[{}; {}], minSizeHint[{}; {}]",
        w->metaObject()->className(),
        w->objectName(),
        w->minimumWidth(), w->width(), w->maximumWidth(),
        w->minimumHeight(), w->height(), w->maximumHeight(),
        w->sizeHint().width(), w->sizeHint().height(),
        w->minimumSizeHint().width(), w->minimumSizeHint().height());
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


//******************
//  QDoubleSpinBox
//******************

inline QDoubleSpinBox* makeStandardSpinbox(QWidget* parent = nullptr, double min = -DBL_MAX, double max = DBL_MAX, double initialVal = 0)
{
    normalizeRange(min, max, initialVal);

    QDoubleSpinBox* spinbox = new QDoubleSpinBox(parent);

    spinbox->setKeyboardTracking(false);
    spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    spinbox->setDecimals(3);
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