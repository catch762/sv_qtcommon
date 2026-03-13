#pragma once
#include "sv_common.h"

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

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

SV_DECL_OPT(QString)
SV_DECL_OPT(QJsonArray)
SV_DECL_OPT(QJsonObject)

using QtTypeIndex = int;

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

//we have to wrap it in array or object, this wraps in array
inline QString jsonValueToString(const QJsonValue &value)
{
    QJsonArray myArray;
    myArray.append(value);

    QJsonDocument doc;
    doc.setArray(myArray);

    return doc.toJson(QJsonDocument::Indented).trimmed();
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