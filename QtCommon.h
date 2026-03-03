#include "sv_common.h"

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

SV_DECL_OPT(QString)
SV_DECL_OPT(QJsonArray)

inline QStringOpt jsonGetStringOpt(const QJsonObject& json, const QString& key)
{
    auto value = json[key];
    if (value.isString()) return value.toString();
    else return {};
};

inline QJsonArrayOpt jsonGetArrayOpt(const QJsonObject& json, const QString& key)
{
    auto value = json[key];
    if (value.isArray()) return value.toArray();
    else return {};
};