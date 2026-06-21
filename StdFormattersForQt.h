#pragma once
#include "sv_common.h"
#include <QString>
#include <QVariant>
#include "QtCommon.h"

SV_DECL_STD_FORMATTER(QString, obj.toStdString());
SV_DECL_STD_FORMATTER(QVariant, qVariantInfo(obj));
SV_DECL_STD_FORMATTER(std::optional<QString>, obj ? obj->toStdString() : std::string(""));