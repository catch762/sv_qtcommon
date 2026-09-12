#include "ColorWidget.h"

HSVAColorWidget::HSVAColorWidget(QColor initialColor, QWidget* parent) : QWidget(parent)
{
	layout = new QGridLayout(this);
	initLayoutSpacing(layout, 2, 0);

	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

	auto hueBackgroundPaintFunc = [](QCustomPaintedSlider * slider, QPainter & p, QCustomPaintedSlider::PaintInfo info)
	{
		QCustomPaintedSlider::fillStdBackground(slider, p, info);

		QCustomPaintedSlider::fillWithVerticalLines(p, info.controlRangeRect, [](double place01)
		{
			return QColor::fromHsvF(place01, 1.0, 1.0);
		});
	};

	auto satBackgroundPaintFunc = [this](QCustomPaintedSlider * slider, QPainter & p, QCustomPaintedSlider::PaintInfo info)
	{
		QCustomPaintedSlider::fillStdBackground(slider, p, info);

		QCustomPaintedSlider::fillWithVerticalLines(p, info.controlRangeRect, [this](double place01)
		{
			return QColor::fromHsvF(color.hueF(), place01, color.valueF());
		});
	};

	auto valBackgroundPaintFunc = [this](QCustomPaintedSlider * slider, QPainter & p, QCustomPaintedSlider::PaintInfo info)
	{
		QCustomPaintedSlider::fillStdBackground(slider, p, info);

		QCustomPaintedSlider::fillWithVerticalLines(p, info.controlRangeRect, [this](double place01)
		{
			return QColor::fromHsvF(color.hueF(), color.saturationF(), place01);
		});
	};

	auto alphaBackgroundPaintFunc = [this](QCustomPaintedSlider * slider, QPainter & p, QCustomPaintedSlider::PaintInfo info)
	{
		QCustomPaintedSlider::fillStdBackground(slider, p, info);

		drawCheckerboard(p, info.controlRangeRect);

		QCustomPaintedSlider::fillWithVerticalLines(p, info.controlRangeRect, [this](double place01)
		{
			return QColor::fromHsvF(color.hueF(), color.saturationF(), color.valueF(), place01);
		});
	};

	hue = makeCompAndAddToLayout("H", hueBackgroundPaintFunc, [this](double hue01)
	{
		setColor(QColor::fromHsvF(hue01, color.saturationF(), color.valueF(), color.alphaF()));
	});

	sat = makeCompAndAddToLayout("S", satBackgroundPaintFunc, [this](double sat01)
	{
		setColor(QColor::fromHsvF(color.hueF(), sat01, color.valueF(), color.alphaF()));
	});

	value = makeCompAndAddToLayout("V", valBackgroundPaintFunc, [this](double val01)
	{
		setColor(QColor::fromHsvF(color.hueF(), color.saturationF(), val01, color.alphaF()));
	});

	alpha = makeCompAndAddToLayout("A", alphaBackgroundPaintFunc, [this](double alpha01)
	{
		setColor(QColor::fromHsvF(color.hueF(), color.saturationF(), color.valueF(), alpha01));
	});

	setColor(initialColor);
}

void HSVAColorWidget::setColor(QColor newColor)
{
	color = newColor;

	//hue background is static and never changes; everything else does
	sat.slider->setBackgroundCacheDirty();
	value.slider->setBackgroundCacheDirty();
	alpha.slider->setBackgroundCacheDirty();

	hue.setValueSilently	(color.hueF());
	sat.setValueSilently	(color.saturationF());
	value.setValueSilently	(color.valueF());
	alpha.setValueSilently	(color.alphaF());

	update();
}

void HSVAColorWidget::Component::setValueSilently(double value01)
{
	QSignalBlocker block1(slider), block2(spinbox);
	setSliderValue01(slider, value01);
	setSpinboxValue01(spinbox, value01);
}