#pragma once
#include "../QtCommon.h"

class SliderAndSpinbox : public QWidget
{
	Q_OBJECT
public:
	SliderAndSpinbox(	QWidget*	parent				= nullptr,
						double		min					= 0,
						double		max					= 1,
						double		initialVal			= 0,
						bool		sliderOnTheLeft		= true,
						QMargins	layoutMargins		= QMargins(0,0,0,0),
						int			layoutSpacing		= 4 );

	double getValue() const;

	//will be clamped to min,max
	void setValue(double val);

	//may change value, obviously
	void setRange(double min, double max);

	double min();
	double max();

	QSlider* getSlider();

	QDoubleSpinBox* getSpinbox();

signals:
	void valueChanged(double val);

private:
	QHBoxLayout*	layout		= nullptr;
	QSlider*			slider	= nullptr; //slave representation, holds val01 between spinbox's min&max
	QDoubleSpinBox*		spinbox = nullptr; //master representation
};