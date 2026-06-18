#include "HorizontalScrollAreaWidget.h"
#include <QPainter>
#include <QApplication>

HorizontalWheelScrollArea::HorizontalWheelScrollArea(QWidget *parent) : QScrollArea(parent)
{
    SV_ASSERT(horizontalScrollBar());

    connect(horizontalScrollBar(), &QScrollBar::valueChanged, 
            this, &HorizontalWheelScrollArea::emitCanScrollUpdated);
}

void HorizontalWheelScrollArea::emitCanScrollUpdated()
{
    SV_ASSERT(horizontalScrollBar());

    auto value = horizontalScrollBar()->value();
    auto maximum = horizontalScrollBar()->maximum();
    bool canScrollLeft = value > 0;
    bool canScrollRight = value < maximum;

    emit canScrollUpdated(canScrollLeft, canScrollRight);
}

void HorizontalWheelScrollArea::wheelEvent(QWheelEvent *event)
{
    // Always forward to horizontal scrollbar, ignoring vertical delta
    if (horizontalScrollBar()) {
        QCoreApplication::sendEvent(horizontalScrollBar(), event);
    }
    event->accept();  // Prevent default vertical handling
}

void HorizontalWheelScrollArea::resizeEvent(QResizeEvent *event)
{
    QScrollArea::resizeEvent(event);
    emitCanScrollUpdated();
}




namespace
{
constexpr int VerticalMargin = 0;

// cant really obtain it in a sane fashion (have to wait until widget is done resizing,
// then query scrollarea viewport size, but i need it earlier... i ll just leave it as constant)
constexpr int VerticalAdditionalFuckingImplicitScrollAreaViewportFuckingMargin = 2;
}

HorizontalScrollAreaWidget::HorizontalScrollAreaWidget(int FixedContentHeight, QWidget *parent)
    : QWidget(parent), ContentHeight(FixedContentHeight)
{
    setFixedHeight(ContentHeight + VerticalMargin * 2 + VerticalAdditionalFuckingImplicitScrollAreaViewportFuckingMargin * 2);

    layout = new QHBoxLayout(this);
    layout->setContentsMargins(2,VerticalMargin,2,VerticalMargin);
    layout->setSpacing(2);

    presetContainer = new QWidget(this);
    
    //presetContainer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    //presetContainer->setFixedHeight(ContentHeight + VerticalAdditionalFuckingImplicitScrollAreaViewportFuckingMargin * 2);
    presetLayout = new QHBoxLayout(presetContainer);
    presetLayout->setContentsMargins(0,0,0,0);
    presetLayout->setSpacing(1);
    
    /*for (int i = 0; i < 10; ++i) {
        QPushButton *btn = new QPushButton(QString("%1").arg(i), presetContainer);
        btn->setFixedSize(Height-4, Height-4);
        presetLayout->addWidget(btn);
    }
    presetLayout->addStretch();*/

    buttonScrollLeft = makeScrollButton(true);
    buttonScrollRight = makeScrollButton(false);

    presetScrollArea = new HorizontalWheelScrollArea();
    {
        presetScrollArea->setWidget(presetContainer);
        //presetScrollArea->setWidgetResizable(false);
        presetScrollArea->setWidgetResizable(true);
        presetScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        presetScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        presetScrollArea->setFixedHeight(ContentHeight + VerticalAdditionalFuckingImplicitScrollAreaViewportFuckingMargin * 2);

        connect(presetScrollArea, &HorizontalWheelScrollArea::canScrollUpdated, this, [this](bool canScrollLeft, bool canScrollRight)
        {
            buttonScrollLeft->setEnabled(canScrollLeft);
            buttonScrollRight->setEnabled(canScrollRight);
        });
    }

    {
        connect(buttonScrollLeft, &QPushButton::clicked, this, [this]()
        {
            doScrollBy(-45);
        });

        connect(buttonScrollRight, &QPushButton::clicked, this, [this]()
        {
            doScrollBy(45);
        });
    }


    layout->addWidget(buttonScrollLeft);
    layout->addWidget(presetScrollArea);
    layout->addWidget(buttonScrollRight);

    presetScrollArea->emitCanScrollUpdated();
}

HorizontalWheelScrollArea *HorizontalScrollAreaWidget::getScrollArea()
{
    return presetScrollArea;
}

QHBoxLayout *HorizontalScrollAreaWidget::getContentLayout()
{
    return presetLayout;
}

QPushButton *HorizontalScrollAreaWidget::makeScrollButton(bool isLeft)
{
    //In my opinion this is retarded and should be easier, but nothing else worked
    static QString btnStylesheet = [this]()->QString
    {
        QColor activeHighlight = palette().color(QPalette::Active, QPalette::Highlight);

        QColor hover = activeHighlight;
        hover.setAlpha(hover.alpha() * 0.1);

        auto stylesheet = QString("QPushButton { border: none; } QPushButton:hover { background-color: rgba(%1, %2, %3, %4); }")
                .arg(hover.red()).arg(hover.green()).arg(hover.blue()).arg(hover.alpha());
        return stylesheet;
    }();

    auto makeIcon = [&](bool isLeft)->QIcon
    {
        auto makeArrowPixmap = [&](bool isLeft, QColor color)
        {
            QIcon originalIcon = style()->standardIcon(isLeft ? QStyle::SP_ArrowLeft : QStyle::SP_ArrowRight);

            //Its black by default and issue is, it looks almost same as when its disabled.
            //So we repaint it
            QPixmap pixmap = originalIcon.pixmap(QSize(10, 10));
            {
                QPainter painter(&pixmap);
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                painter.fillRect(pixmap.rect(), color);
                painter.end();
            }
            return pixmap;
        };


        QColor activeColor = palette().color(QPalette::Active, QPalette::Highlight); 
        QColor disabledColor = QColor(215,215,215);
        
        QIcon icon;
        icon.addPixmap(makeArrowPixmap(isLeft, activeColor), QIcon::Normal);
        icon.addPixmap(makeArrowPixmap(isLeft, disabledColor), QIcon::Disabled);

        return icon;
    };

    static QIcon iconLeft = makeIcon(true);
    static QIcon iconRight = makeIcon(false);

    auto btn = new QPushButton(this);
    btn->setFixedSize(15, ContentHeight);

    btn->setIcon(isLeft ? iconLeft : iconRight);
    btn->setStyleSheet(btnStylesheet);

    return btn;
}

void HorizontalScrollAreaWidget::doScrollBy(int pixels)
{
    auto *bar = presetScrollArea->horizontalScrollBar();
    SV_ASSERT(bar);

    bar->setValue(bar->value() + pixels);
}
