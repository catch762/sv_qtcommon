#pragma once
#include <QSlider>
#include <QPainter>
#include "sv_qtcommon.h"

class QCustomPaintedSlider : public QSlider
{
public:

    struct PaintInfo
    {
        QRect   fullRect;
        QRect   controlRangeRect;
        double  handleCenterX;
    };

    // If 'usingBackgroundCache' is enabled, this will actually draw on QPixmap
    // But you shoudlnt care.
    using BackgroundPaintFunc = std::function<void(QCustomPaintedSlider* slider, QPainter& p, PaintInfo info)>;

    // On the contrary, this is not cached and is called on every paint event.
    // This is to draw that -----|-- handle
    using HandlePaintFunc = std::function<void(QCustomPaintedSlider* slider, QPainter& p, PaintInfo info)>;

    //Not supplying paint funcs will simply draw it as normal QSlider
    QCustomPaintedSlider(   bool                cachingBackgroundEnabled,
                            BackgroundPaintFunc theBackgroundPaintFunc  = nullptr,
                            HandlePaintFunc     theHandlePaintFunc      = nullptr,
                            Qt::Orientation     orientation             = Qt::Horizontal, 
                            QWidget*            parent                  = nullptr) : QSlider(orientation, parent)
    {
        usingBackgroundCache    = cachingBackgroundEnabled;
        backgroundPaintFunc     = std::move(theBackgroundPaintFunc);
        handlePaintFunc         = std::move(theHandlePaintFunc);
    }

    void setBackgroundCacheDirty()
    {
        backgroundCacheIsDirty = true;
    }

//Example:
public:
    

    

    static void exampleGrayscaleBackgroundPaintFunc(QCustomPaintedSlider* slider, QPainter& p, PaintInfo info)
    {
        SV_LOG("exampleGrayscaleBackgroundPaintFunc() called;");

        QLinearGradient gradient(   info.controlRangeRect.left(), 
                                    info.controlRangeRect.top(), 
                                    info.controlRangeRect.right(), 
                                    info.controlRangeRect.top());
        gradient.setColorAt(0.0, Qt::black);
        gradient.setColorAt(1.0, Qt::white);

        p.fillRect(info.fullRect,           Qt::blue);
        p.fillRect(info.controlRangeRect,   gradient);
    }

    static void exampleRedHandlePaintFunc(QCustomPaintedSlider* slider, QPainter& p, PaintInfo info)
    {
        const int handleWidth   = 1;

        auto    handle  = QLineF{
            QPointF(info.handleCenterX, 0),
            QPointF(info.handleCenterX, info.fullRect.height()-1)
        };

        p.setPen(QPen(Qt::red, handleWidth));

        p.drawLine(handle);
    }

    static QCustomPaintedSlider* makeExampleSlider(QWidget* parent = nullptr)
    {
        return new QCustomPaintedSlider(true,
                                        &QCustomPaintedSlider::exampleGrayscaleBackgroundPaintFunc,
                                        &QCustomPaintedSlider::exampleRedHandlePaintFunc,
                                        Qt::Horizontal,
                                        parent);
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        if (!backgroundPaintFunc)
        {
            QSlider::paintEvent(event);
            return;
        }

        auto paintInfo = getPaintInfo();

        if (usingBackgroundCache)
        {
            const bool backgroundCacheResized       = ensureBackgroundCacheHasCurrentSize();
            const bool backgroundCacheNeedsRedraw   = backgroundCacheIsDirty || backgroundCacheResized;

            if (backgroundCacheNeedsRedraw)
            {
                QPainter painter(&backgroundCache);
                backgroundPaintFunc(this, painter, paintInfo);
                backgroundCacheIsDirty = false;
            }
        }

        QPainter painter(this);

        if (usingBackgroundCache)
        {
            painter.drawPixmap(QPoint(0, 0), backgroundCache);
        }
        else
        {
            backgroundPaintFunc(this, painter, paintInfo);
        }

        if (handlePaintFunc)
        {
            handlePaintFunc(this, painter, paintInfo);
        }
    }

private:
    //returns true if cache was resized
    bool ensureBackgroundCacheHasCurrentSize()
    {
        const auto requiredSize = rect().size();
        if (!backgroundCache || backgroundCache.size() != requiredSize)
        {
            backgroundCache = QPixmap(requiredSize);
            return true;
        }
        else return false;
    }

    PaintInfo getPaintInfo() const
    {
        auto fullrect = rect();
        auto [leftmostPixel, rightmostPixel] = sliderHorizontalHandleCenterRange(this);
        int controlRangeWidth = rightmostPixel - leftmostPixel + 1;
        double  handleX = coord01ToPixelRange(leftmostPixel, rightmostPixel, getSliderValue01(this));

        QRect controlRangeRect = QRect(leftmostPixel, fullrect.top(), controlRangeWidth, fullrect.height());

        return PaintInfo{
            fullrect,
            controlRangeRect,
            handleX
        };
    }

private:
    BackgroundPaintFunc backgroundPaintFunc;
    HandlePaintFunc     handlePaintFunc;

    bool    usingBackgroundCache    = true;
    bool    backgroundCacheIsDirty  = true;
    QPixmap backgroundCache; 

};