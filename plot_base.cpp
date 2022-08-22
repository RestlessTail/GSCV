#include "plot_base.h"

plot_base::plot_base(){
	view_widget = nullptr;
}

void plot_base::set_view_widget(opengl_view_widget *new_view_widget){
	view_widget = new_view_widget;
}