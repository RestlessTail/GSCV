#include "right_panel_manager.h"
#include <QTabWidget>

right_panel_base::right_panel_base(QWidget* parent) :
	QWidget(parent)
{
	view_widget = nullptr;
}

void right_panel_base::set_view_widget(opengl_view_widget* new_view_widget){
	view_widget = new_view_widget;
}

QString& right_panel_base::get_panel_title(){
	return panel_title;
}

void right_panel_base::set_panel_title(QString new_panel_title){
	panel_title = new_panel_title;
}

void right_panel_manager::setup(opengl_view_widget *new_view_widget, QTabWidget *new_right_panel){
	view_widget = new_view_widget;
	right_panel = new_right_panel;
}

panel_info::panel_info(){
	widget = nullptr;
}

void right_panel_manager::destroy_panel(panel_type type){
	panel_info& info = panel_info_array[static_cast<int>(type)];
    right_panel->removeTab(right_panel->indexOf(info.widget));
	info.widget->deleteLater();
	info.widget = nullptr;
}