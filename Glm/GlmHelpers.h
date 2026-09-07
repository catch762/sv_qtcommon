#pragma once
#include "../JsonUtils.h"

template <GlmUnderlyingType T, int VecSize>
    requires ValidGlmVecSize<VecSize>
QJsonArray glmVecToJson(const glm::vec<VecSize, T>& v)
{
    QJsonArray arr;

    for (int i = 0; i < VecSize; ++i)
    {
        arr.append(QJsonValue(static_cast<double>(v[i])));
    }

    return arr;
}

template <GlmUnderlyingType T, int VecSize>
    requires ValidGlmVecSize<VecSize>
std::optional<glm::vec<VecSize, T>> glmVecFromJson(const QJsonValue& json)
{
    if (!json.isArray())
        return std::nullopt;

    const QJsonArray arr = json.toArray();
    if (arr.size() != VecSize)
        return std::nullopt;

    glm::vec<VecSize, T> res{};

    for (int i = 0; i < VecSize; ++i)
    {
        if (doubleOpt v = convertJson<double>(arr[i]))
        {
            res[i] = *v;
        }
        else return std::nullopt;
    }

    return res;
}