#pragma once
#include "QtCommon.h"
#include <QDirIterator>

//overwrites content
inline bool writeStringToFile(const QString &content, const QString &filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        SV_ERROR(std::format("Couldnt write string to file: couldnt open file to write: {}", filepath.toStdString()));
        return false; // failed to open
    }

    auto totalBytes = content.toUtf8().size();
    auto writtenBytes = file.write(content.toUtf8());
    file.close();

    bool writtenOk = writtenBytes == totalBytes;

    if (!writtenOk)
    {
        SV_ERROR(std::format("Couldnt write string to file: only written [{}/{} bytes] to: {}", writtenBytes, totalBytes, filepath.toStdString()));
    }

    return writtenOk;
}

inline QByteArrayOpt readByteArrayFromFile(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        SV_ERROR(std::format("readByteArrayFromFile: cant open file to read, path = [{}], error = [{}]",
            filePath.toStdString(), file.errorString().toStdString()));
        return {};
    }

    return file.readAll();
}

inline bool writeByteArrayToFile(const QString& filePath, const QByteArray& data)
{
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly))
    {
        SV_ERROR(std::format("writeByteArrayToFile: cant open file to write, path = [{}], error = [{}]",
            filePath.toStdString(), file.errorString().toStdString()));
        return false;
    }

    auto bytesWritten = file.write(data);

    if (bytesWritten != data.size())
    {
        SV_ERROR(std::format("writeByteArrayToFile: error, written only [{} / {}] bytes",
            bytesWritten, data.size()));
        return false;
    }

    return true;
}

//filter extension strings in format "*.ext"
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

//accepts both absolute paths like "C:/filename.tar.gz" and relative like "filename.tar.gz"
//and returns just "filename.tar"
inline QString getFileNameWithoutLastExtension(const QString& filePathOrFileName)
{
    return QFileInfo(filePathOrFileName).completeBaseName();
}