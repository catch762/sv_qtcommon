#pragma once
#include "sv_qtcommon.h"

class ColorWidget : public QWidget
{
	Q_OBJECT
public:

private:
	QGridLayout*	layout		= nullptr;
	QLabel*				labelH	= nullptr;
	QLabel*				labelS	= nullptr;
	QLabel*				labelV	= nullptr;
	QLabel*				labelA	= nullptr;
	SliderAndSpinbox*	h		= nullptr;
	SliderAndSpinbox*	s		= nullptr;
	SliderAndSpinbox*	v		= nullptr;
	SliderAndSpinbox*	a		= nullptr;
};