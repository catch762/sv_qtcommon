#pragma once
#include "sv_common.h"
#include <QString>

SV_DECL_STD_FORMATTER(QString, obj.toStdString());
SV_DECL_STD_FORMATTER(std::optional<QString>, obj ? obj->toStdString() : std::string("QStringNullOpt"));