#include "right_panel_manager.h"
#include <QTabWidget>

void right_panel_base::set_view_widget(opengl_view_widget* new_view_widget){
	view_widget = new_view_widget;
}

void right_panel_manager::set_view_widget(opengl_view_widget* new_view_widget){
	view_widget = new_view_widget;
}

void right_panel_manager::set_right_panel(QTabWidget* new_right_panel){
	right_panel = new_right_panel;
}

panel_info::panel_info(){
	widget = nullptr;
}

template <typename T1> void right_panel_manager::open_panel(panel_type type){
	panel_info& info = panel_info_array[static_cast<int>(type)];
	if(info.widget){
		right_panel->setCurrentIndex(right_panel->indexOf(info.widget));
	}
	else{
		info.widget = new T1;
		info.widget->set_view_widget(view_widget);
	}
}
void right_panel_manager::destroy_panel(panel_type type){
	panel_info& info = panel_info_array[static_cast<int>(type)];
    right_panel->removeTab(right_panel->indexOf(info.widget));
	info.widget->deleteLater();
	info.widget = nullptr;
}