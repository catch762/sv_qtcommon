#pragma once
#include "sv_qtcommon.h"

class HSVAColorWidget : public QWidget
{
	Q_OBJECT
public:
	HSVAColorWidget(QColor initialColor, QWidget* parent = nullptr);

	void setColor(QColor newColor);

private:
	struct Component
	{
		QLabel*					label	= nullptr;
		QCustomPaintedSlider*	slider	= nullptr; //slave representation
		QDoubleSpinBox*			spinbox	= nullptr; //master representation. holds value01

		void setValueSilently(double value01);
	};

	template <typename BackgroundPaintFunc, typename OnValueChangedFunc>
	Component makeCompAndAddToLayout(	const QString&			labelText,
										BackgroundPaintFunc&&	backgroundFunc,
										OnValueChangedFunc&&	onValChangedFunc );

private:
	QGridLayout*	layout			= nullptr;
	Component			hue			= {};
	Component			sat			= {};
	Component			value		= {};
	Component			alpha		= {};

private:
	QColor color = Qt::white;
};


template <typename BackgroundPaintFunc, typename OnValueChangedFunc>
HSVAColorWidget::Component HSVAColorWidget::makeCompAndAddToLayout(	const QString&			labelText,
																	BackgroundPaintFunc&&	backgroundFunc,
																	OnValueChangedFunc&&	onValChangedFunc )
{
	auto* label = new QLabel(labelText, this);
	label->setAlignment(Qt::AlignCenter);

	auto* slider = new QCustomPaintedSlider(true, backgroundFunc, nullptr, Qt::Horizontal, this);
	slider->setControlRangeVMargins(2, 2);

	auto* spinbox = makeStandardSpinbox(this, 0.0, 1.0, 1.0);
	spinbox->setSingleStep(0.01);
	spinbox->setFixedWidth(42);

	//slave sets master value loudly
	connect(slider, &QSlider::valueChanged, this, [slider, spinbox](int)
	{
		spinbox->setValue( getSliderValue01(slider) );
	});

	//master sets slave value silently
	connect(spinbox, &QDoubleSpinBox::valueChanged, this, [slider, onValChangedFunc](double value01)
	{
		QSignalBlocker block(slider);
		setSliderValue01(slider, value01);

		onValChangedFunc(value01);
	});


	{
		label->setFixedHeight(22);
		slider->setFixedHeight(20);
		spinbox->setFixedHeight(22);
	}

	int nextRow = layout->rowCount();
	layout->addWidget(label,	nextRow, 0);
	layout->addWidget(slider,	nextRow, 1);
	layout->addWidget(spinbox, nextRow, 2);

	return Component{ label, slider, spinbox };
}