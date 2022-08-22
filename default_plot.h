#ifndef DEFAULT_PLOT
#define DEFAULT_PLOT

#include "plot_base.h"

class default_plot : public plot_base{
public:
	void fill_VBO() override;
};

#endif