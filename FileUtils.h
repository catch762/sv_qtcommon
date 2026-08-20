#pragma once
#include "QtCommon.h"
#include <QDirIterator>

//filter extension string in format "*.ext"
inline std::vector<QString> getAbsPathsOfAllFilesInDirectory(   const QString&              folderPath,
                                                                const std::vector<QString>& extensionFilters = {},
                                                                bool                        recursive        = false)
{
    std::vector<QString> result;

    QStringList nameFilters;
    for (const auto& extension : extensionFilters)
    {
        nameFilters << "*." + extension;
    }

    QDirIterator it(
        folderPath,
        nameFilters,
        QDir::Files | QDir::Readable,
        recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags
    );

    while (it.hasNext()) 
    {
        it.next();
        result.push_back(it.filePath());  // absolute path
    }

    return result;
}