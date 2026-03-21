#pragma once
#include "QtCommon.h"
#include <QWheelEvent>
#include <QScrollBar>
#include <QScrollArea>
#include <QCoreApplication>
#include <QToolButton>

// Its like normal QScrollArea, but:
//  - wheel scroll always scrolls it horizontally. Not vertically.
//  - it emits 'canScrollUpdated' (shows whether we can scroll left/right)
class HorizontalWheelScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    HorizontalWheelScrollArea(QWidget *parent = nullptr);

public slots:
    void emitCanScrollUpdated();

signals: 
    void canScrollUpdated(bool canScrollLeft, bool canScrollRight);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;
};

// Simply contains: [<] [HorizontalWheelScrollArea] [>]
// Buttons allow scrolling left/right, and are disabled if we cant scroll any more.
class HorizontalScrollAreaWidget : public QWidget
{
    Q_OBJECT
public:
    HorizontalScrollAreaWidget(QWidget *parent = nullptr);

private:
    QPushButton* makeScrollButton(bool isLeft);
    void doScrollBy(int pixels); //negative will scroll left

private:
    QHBoxLayout*                layout                = nullptr;
    QPushButton*                  buttonScrollLeft    = nullptr;
    HorizontalWheelScrollArea*    presetScrollArea    = nullptr;
    QWidget*                        presetContainer   = nullptr;
    QHBoxLayout*                    presetLayout      = nullptr;
    QPushButton*                  buttonScrollRight   = nullptr;
private:
    int Height = 30;
};