#pragma once
#include "QtCommon.h"

inline qsizetype firstPosOfAnyCharFromList(const QString& string, const QString& charList)
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

// Example:
//      hello\" "tralala\"kek" hel"lo
// Return indexes:
//              ^            ^
//
// It assumes that \" INSIDE the string is just escaped charachter which will be ignored.
// If input starts with escaped quote, its still ignored, the string starts with first unescaped quote.
// Function does NOT account for escaped slashes \\"
inline std::pair<int, int> getFirstTwoUnescapedQuotesIndexes(const QString& text)
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