#pragma once
#include "QtCommon.h"

qsizetype firstPosOfAnyCharFromList(const QString& string, const QString& charList);

// Example:
//      hello\" "tralala\"kek" hel"lo
// Return indexes:
//              ^            ^
//
// It assumes that \" INSIDE the string is just escaped charachter which will be ignored.
// If input starts with escaped quote, its still ignored, the string starts with first unescaped quote.
// Function does NOT account for escaped slashes \\"
std::pair<int, int> getFirstTwoUnescapedQuotesIndexes(const QString& text);

using QStringVec = std::vector<QString>;
SV_DECL_OPT(QStringVec);

// Lets say, you pass {"HELLO", "WORLD"} in separators.
// Function then assumes, the 'string' is expected to contain:
//      <anything0>HELLO<anything1>WORLD<anything2>
// And will return:
//      { <anything0>, <anything1>, <anything2> }
//
// Resulting parts may be empty.
// However, if any separator is not found, it is error, and nullopt is returned.
QStringVecOpt splitStringBySeparators(const QString &string, const QStringVec& separators, bool trimResults);