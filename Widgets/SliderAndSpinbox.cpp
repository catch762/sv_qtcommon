#include "SliderAndSpinbox.h"

SliderAndSpinbox::SliderAndSpinbox(QWidget* parent, double min, double max, double initialVal, bool sliderOnTheLeft, int spinboxFixedWidth, QMargins layoutMargins, int layoutSpacing)
	: QWidget(parent)
{
	normalizeRange(min, max, initialVal);
	double val01 = getValue01Clamped(initialVal, min, max);

	layout = new QHBoxLayout(this);
	layout->setContentsMargins(layoutMargins);
	layout->setSpacing(layoutSpacing);

	spinbox = makeStandardSpinbox(this, min, max, initialVal);
	spinbox->setFixedWidth(spinboxFixedWidth);
	slider = makeSliderForNormDouble(this, val01);

	//slave sets master value loudly
	connect(slider, &QSlider::valueChanged, this, [this](int)
		{
			setSpinboxValue01(spinbox, getSliderValue01(slider));
		});

	//master sets slave value silently
	connect(spinbox, &QDoubleSpinBox::valueChanged, this, [this](double value)
		{
			QSignalBlocker block(slider);
			setSliderValue01(slider, getSpinboxValue01(spinbox));

			emit valueChanged(value);
		});

	if (sliderOnTheLeft)
	{
		layout->addWidget(slider);
		layout->addWidget(spinbox);
	}
	else
	{
		layout->addWidget(spinbox);
		layout->addWidget(slider);
	}
}

double SliderAndSpinbox::getValue() const
{
	return spinbox->value();
}

//will be clamped to min,max
void SliderAndSpinbox::setValue(double val)
{
	spinbox->setValue(val);
}

//may change value, obviously
void SliderAndSpinbox::setRange(double min, double max)
{
	normalizeRange(min, max);
	spinbox->setRange(min, max);
}

double SliderAndSpinbox::min()
{
	return spinbox->minimum();
}

double SliderAndSpinbox::max()
{
	return spinbox->maximum();
}

QSlider* SliderAndSpinbox::getSlider()
{
	return slider;
}

QDoubleSpinBox* SliderAndSpinbox::getSpinbox()
{
	return spinbox;
}
