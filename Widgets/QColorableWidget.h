#pragma once
#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QPaintEvent>

class QColorableWidget : public QWidget
{
public:
    QColorableWidget(QColor initialColor = Qt::white, QWidget* parent = nullptr)
        : QWidget(parent), color(initialColor)
    {
    }

    QColor getColor() const
    {
        return color;
    }

    //call update() afterwards to apply
    void setColor(const QColor& newColor)
    {
        if (newColor == color)
            return;

        color = newColor;
    }

protected:
    void paintEvent(QPaintEvent* /*event*/) override
    {
        QPainter painter(this);
        painter.fillRect(rect(), color);
    }

private:
    QColor color = Qt::white;
};