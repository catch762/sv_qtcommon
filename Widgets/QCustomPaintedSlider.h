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

        setMinimum(-10000);
        setMaximum(+10000);
        setValue(0);

        //calculate left/right margins, we only do it once:
        {
            auto [leftmostPixel, rightmostPixel] = sliderHorizontalHandleCenterRange(this);
            controlRangeMargins.setLeft(leftmostPixel);
            controlRangeMargins.setRight(width() - rightmostPixel - 1);
        }
    }

    void setBackgroundCacheDirty()
    {
        backgroundCacheIsDirty = true;
    }

    //you only can set vertical ones, because horizontal ones are set by Qt (8 px) and i cant change them
    void setControlRangeVMargins(int top, int bottom)
    {
        controlRangeMargins.setTop(top);
        controlRangeMargins.setBottom(bottom);
    }

//Example:
public:
    
    template <typename PlaceToColorFunc>
        requires std::is_invocable_r_v<QColor, PlaceToColorFunc, double /*place01*/>
    static void fillWithVerticalLines(QPainter& p, QRect rect, const PlaceToColorFunc& placeToColorFunc)
    {
        for (int x = rect.left(); x <= rect.right(); ++x)
        {
            double place01 = getValue01Clamped(x, rect.left(), rect.right());
            QColor color = placeToColorFunc(place01);

            // Draw a 1-pixel wide vertical line for this color column
            p.fillRect(x, rect.top(), 1, rect.height(), color);
        }
    }
    
    static void fillStdBackground(QCustomPaintedSlider* slider, QPainter& p, PaintInfo info)
    {
        p.fillRect(info.fullRect, slider->palette().color(QPalette::Window));
    }

    static void exampleGrayscaleBackgroundPaintFunc(QCustomPaintedSlider* slider, QPainter& p, PaintInfo info)
    {
        SV_LOG("exampleGrayscaleBackgroundPaintFunc() called;");

        QCustomPaintedSlider::fillStdBackground(slider, p, info);

        QCustomPaintedSlider::fillWithVerticalLines(p, info.controlRangeRect, [](double place01)
        {
            int gray = std::clamp(int(255.0 * place01), 0, 255);

            return QColor(gray, gray, gray);
        });
    }

    static void defaultHandlePaintFunc(QCustomPaintedSlider* slider, QPainter& p, PaintInfo info)
    {
        p.setPen(QPen(QColor(138,138,138), 1));
        p.setBrush(QColor(240, 240, 240));

        //3-pixel wide rect, including 1-pixel border. Exactly centered at handle position.
        //I fucking love that code that draws it doesnt make any fucking sense

        QRect handleRect = {
            int(info.handleCenterX) - 2, //like, why -2?
            0,                           //ok
            2,                           //???
            info.fullRect.height() - 1   //???
        };

        p.drawRect(handleRect);
    }

    static QCustomPaintedSlider* makeExampleSlider(QWidget* parent = nullptr)
    {
        return new QCustomPaintedSlider(true,
                                        &QCustomPaintedSlider::exampleGrayscaleBackgroundPaintFunc,
                                        &QCustomPaintedSlider::defaultHandlePaintFunc,
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
        else
        {
            defaultHandlePaintFunc(this, painter, paintInfo);
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
        QRect   fullrect            = rect();
        QRect   controlRangeRect    = fullrect.marginsRemoved(controlRangeMargins);
        double  handleX             = coord01ToPixelRange(  controlRangeRect.left(),
                                                            controlRangeRect.right(), 
                                                            getSliderValue01(this) );
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

    QMargins controlRangeMargins = {};
};