#include "ParseUtils.h"
#include "StdFormattersForQt.h"

qsizetype firstPosOfAnyCharFromList(const QString& string, const QString& charList)
{
    for (qsizetype i = 0; i < string.size(); ++i)
    {
        if (charList.contains(string[i]))
        {
            return i;
        }
    }
    return -1;
}

std::pair<int, int> getFirstTwoUnescapedQuotesIndexes(const QString& text)
{
    std::pair<int, int> res{-1,-1};

    for (int i = 0; i < text.size(); ++i)
    {
        bool isQuoteChar            = text[i] == '"';
        bool hasEscapeSlashBefore   = i > 0 && text[i-1] == '\\';
        bool isUnescapedQuote       = isQuoteChar && !hasEscapeSlashBefore;

        if (isUnescapedQuote)
        {
            if (res.first == -1)
            {
                res.first = i;
            }
            else
            {
                res.second = i;
                return res;
            }
        }
    }

    return res;
}

QStringVecOpt splitStringBySeparators(const QString &string, const QStringVec& separators, bool trimResults)
{
    QStringVec parts;
    parts.reserve(separators.size() + 1);

    qsizetype pos = 0;

    auto saveObtainedPart = [&](QString&& part)
    {
        if(trimResults)
        {
            part = part.trimmed();
        }

        parts.push_back(std::move(part));
    };

    for (const QString& sep : separators)
    {
        const qsizetype found = string.indexOf(sep, pos);
        if (found < 0)
        {
            SV_ERROR(std::format("Split string error, separator [{}] not found in string [{}]", sep, string));
            return {};
        }

        QString part = string.mid(pos, found - pos);
        saveObtainedPart(std::move(part));

        pos = found + sep.size();
    }

    QString tailPart = string.mid(pos);
    saveObtainedPart(std::move(tailPart));

    return parts;
}