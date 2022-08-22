#ifndef PLOT_BASE
#define PLOT_BASE

class opengl_view_widget;

class plot_base{
public:
	plot_base();
	void set_view_widget(opengl_view_widget* new_view_widget);
public:
	virtual void fill_VBO() = 0;
public:
	opengl_view_widget* view_widget;
};

#endif