#pragma once
#include "sv_qtcommon.h"

template<int Count>
class SpinboxesHelper
{
public:
	using VecT = glm::vec<Count, float, glm::qualifier::defaultp>;

	static constexpr int LayoutSpacing = 2;

	SpinboxesHelper(VecT val, QWidget* spinboxesParent)
	{
		layout = new QHBoxLayout(spinboxesParent);
		initLayoutSpacing(layout, 0, LayoutSpacing);

		for (int i = 0; i < Count; ++i)
		{
			QDoubleSpinBox* spinbox = makeStandardSpinbox(spinboxesParent);
			spinboxes[i] = spinbox;

			layout->addWidget(spinbox);
		}

		spinboxesParent->setMinimumWidth(minimumWidth());

		setValue(val);
	}

	int minimumWidth() const
	{
		return Count * 30 + LayoutSpacing * (Count - 1);
	}
	int sizeHintWidth() const
	{
		return minimumWidth(); //idk, its fine
	}
	int minimumSizeHintWidth() const
	{
		return minimumWidth(); //idk, its fine
	}

	template<typename Visitor>
		requires std::is_invocable_v<Visitor, QDoubleSpinBox*>
	void visitSpinboxes(const Visitor& visitor)
	{
		for (auto spinbox : spinboxes) visitor(spinbox);
	}

	QDoubleSpinBox* getSpinbox(int idx)
	{
		SV_ASSERT(idx >= 0 && idx < Count);
		return spinboxes[idx];
	}

	VecT getValue() const
	{
		VecT value;

		for (int i = 0; i < Count; ++i)
		{
			value[i] = spinboxes[i]->value();
		}

		return value;
	}

	//Does it silently - spinboxes will not emit signals
	void setValue(VecT value)
	{
		for (int i = 0; i < Count; ++i)
		{
			QSignalBlocker block(spinboxes[i]);
			spinboxes[i]->setValue(value[i]);
		}
	}

private:
	QHBoxLayout* layout = nullptr;
	std::array<QDoubleSpinBox*, Count> spinboxes;
};

class SpinboxesVec3 : public QWidget
{
	Q_OBJECT
public:
	using VecT = glm::vec3;

	SpinboxesVec3(VecT val = {}, QWidget* parent = nullptr) : QWidget(parent), helper(val, this)
	{
		helper.visitSpinboxes([this](QDoubleSpinBox* spin)
		{
			connect(spin, &QDoubleSpinBox::valueChanged, this, [this]()
			{
				emit valueChanged(getValue());
			});
		});
	}

	VecT getValue() const
	{
		return helper.getValue();
	}

	void setValue(VecT value)
	{
		helper.setValue(value);
		emit valueChanged(getValue());
	}

	QSize sizeHint() const override
	{
		return{ helper.sizeHintWidth(), QWidget::sizeHint().height() };
	}
	QSize minimumSizeHint() const override
	{
		return{ helper.minimumSizeHintWidth(), QWidget::sizeHint().height() };
	}

signals:
	void valueChanged(VecT value);

private:
	SpinboxesHelper<3> helper;
};