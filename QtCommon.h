#pragma once
#include "sv_common.h"

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QFrame>
#include <QSlider>
#include <QPointer>
#include <QScrollArea>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDir>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>

SV_DECL_OPT(QString)
SV_DECL_OPT(QJsonArray)
SV_DECL_OPT(QByteArray)
SV_DECL_ERR(QByteArray)
SV_DECL_OPT(QJsonObject)
SV_DECL_OPT(QJsonValue)
SV_DECL_OPT(QVariant)
SV_DECL_OPT(QDir)
SV_DECL_OPT(QColor)

//Checks that 'WidgetT is indeed QWidget-derived, but is not just QWidget'
template <typename WidgetT>
concept IsConcreteWidget =
    std::derived_from<std::decay_t<WidgetT>, QWidget> &&
    !std::same_as    <std::decay_t<WidgetT>, QWidget>;


//Addition to logger
#define SV_MSGBOX_LOG(text)     {SV_INFO(text);   QMessageBox::information(nullptr, "Information", QString::fromStdString(text));}
#define SV_MSGBOX_WARN(text)    {SV_WARN(text);  QMessageBox::warning(nullptr, "Warning", QString::fromStdString(text));}
#define SV_MSGBOX_ERROR(text)   {SV_ERROR(text); QMessageBox::critical(nullptr, "Error", QString::fromStdString(text));}


//coord01: 0.0 corresponds to middle of leftmost pixel, 1.0 corresponds to middle of rightmost pixel
inline double coord01ToPixelRange(int leftmostPixel, int rightmostPixel, double coord01)
{
    int actualPixelRange = rightmostPixel - leftmostPixel;

    return double(leftmostPixel) + 0.5 + double(actualPixelRange) * coord01;
}

inline QPointF pixCoordOfNdcCoord(QRect rect, glm::vec2 ndc11)
{
    double pix_x = coord01ToPixelRange(rect.left(), rect.right (), value11To01( ndc11.x));
    double pix_y = coord01ToPixelRange(rect.top (), rect.bottom(), value11To01(-ndc11.y));
    return { pix_x, pix_y };
}

inline QColor mixColors(QColor a, QColor b, double ratio01)
{
    ratio01 = std::clamp(ratio01, 0.0, 1.0);

    mix(a.redF(), b.redF(), ratio01);

    QColor res;
    res.setRedF     (mix(a.redF  (), b.redF  (), ratio01));
    res.setGreenF   (mix(a.greenF(), b.greenF(), ratio01));
    res.setBlueF    (mix(a.blueF (), b.blueF (), ratio01));
    res.setAlphaF   (mix(a.alphaF(), b.alphaF(), ratio01));

    return res;
}

inline void drawCheckerboard(QPainter& painter, const QRect& rect, QColor a = Qt::white, QColor b = QColor(204,204,204), int squareSize = 8)
{
    if (squareSize <= 0) return;

    painter.fillRect(rect, a);

    painter.setPen(Qt::NoPen);
    painter.setBrush(b);

    // Calculate how many rows and columns fit the bounding box
    int cols = (rect.width() + squareSize - 1) / squareSize;
    int rows = (rect.height() + squareSize - 1) / squareSize;

    // Loop through and draw the second color squares
    for (int row = 0; row < rows; ++row) {
        int top = rect.y() + (row * squareSize);

        int startCol = (row % 2 == 0) ? 1 : 0;

        for (int col = startCol; col < cols; col += 2) {
            int left = rect.x() + (col * squareSize);

            QRect cellRect(left, top, squareSize, squareSize);

            QRect clippedRect = cellRect.intersected(rect);

            if (!clippedRect.isEmpty()) {
                painter.drawRect(clippedRect);
            }
        }
    }
}

inline glm::vec4 colorToVec(QColor c)
{
    return { c.redF(), c.greenF(), c.blueF(), c.alphaF() };
}
inline QColor colorFromVec(glm::vec4 vec)
{
    vec = glm::clamp(vec, { 0.0, 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0, 1.0 });
    QColor c;
    c.setRedF   (vec.x);
    c.setGreenF (vec.y);
    c.setBlueF  (vec.z);
    c.setAlphaF (vec.w);
    return c;
}

//always between 0 and 1. Cause default hueF() will return -1 for grayscale colors
inline float safeHueF(QColor c)
{
    return std::clamp(c.hueF(), 0.0f, 1.0f);
}
inline QColor safeFromHsvF(float h, float s, float v, float a = 1.0f)
{
    return QColor::fromHsvF(std::clamp(h, 0.0f, 1.0f), s, v, a);
}