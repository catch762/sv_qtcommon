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









HorizontalScrollAreaWidget::HorizontalScrollAreaWidget(QWidget *parent)
{
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(2,2,2,2);
    layout->setSpacing(2);

    presetContainer = new QWidget(this);
    
    //presetContainer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    //presetContainer->setFixedHeight(Height);
    presetLayout = new QHBoxLayout(presetContainer);
    presetLayout->setContentsMargins(0,0,0,0);
    presetLayout->setSpacing(2);
    
    for (int i = 0; i < 10; ++i) {
        QPushButton *btn = new QPushButton(QString("%1").arg(i), presetContainer);
        btn->setFixedSize(Height-4, Height-4);
        presetLayout->addWidget(btn);
    }
    presetLayout->addStretch();

    buttonScrollLeft = makeScrollButton(true);
    buttonScrollRight = makeScrollButton(false);

    presetScrollArea = new HorizontalWheelScrollArea();
    {
        presetScrollArea->setWidget(presetContainer);
        //presetScrollArea->setWidgetResizable(false);
        presetScrollArea->setWidgetResizable(true);
        presetScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        presetScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        presetScrollArea->setFixedHeight(Height);

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

    auto makeIcon = [this](bool isLeft)->QIcon
    {
        QIcon originalIcon = style()->standardIcon(isLeft ? QStyle::SP_ArrowLeft : QStyle::SP_ArrowRight);

        //something like bright solid blue for me
        QColor activeColor = palette().color(QPalette::Active, QPalette::Highlight); 

        //Its black by default and issue is, it looks almost same as when its disabled.
        //So we repaint it with 'activeColor'. (this replaces all black pixels to blue)
        QPixmap pixmap = originalIcon.pixmap(QSize(10, 10));
        {
            QPainter painter(&pixmap);
            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            painter.fillRect(pixmap.rect(), activeColor);
            painter.end();
        }

        //It keeps vanilla gray color for disabled state
        return QIcon(pixmap);
    };

    static QIcon iconLeft = makeIcon(true);
    static QIcon iconRight = makeIcon(false);

    auto btn = new QPushButton(this);
    btn->setFixedSize(15, Height);

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
