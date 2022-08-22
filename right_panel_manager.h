#ifndef RIGHT_PANEL_MANAGER
#define RIGHT_PANEL_MANAGER

#include <array>
#include <QWidget>
#include <QString>
#include <QTabWidget>

class opengl_view_widget;
class color_scheme;
class palette_t;

enum class panel_type{
	statistic,
	camera_setting,
	cell_color,
	color_scheme,
	enum_size
};

//each object in tool panel should inherit this class and set panel_title in constructor
class right_panel_base : public QWidget{
public:
	right_panel_base(QWidget *parent = nullptr);
	virtual void set_view_widget(opengl_view_widget* new_view_widget);
	QString& get_panel_title();
	void set_panel_title(QString new_panel_title);
	opengl_view_widget* view_widget;
private:
	QString panel_title;
};

struct panel_info{
	panel_info();
	right_panel_base* widget;
};

class right_panel_manager{
public:
	void setup(opengl_view_widget* new_view_widget, QTabWidget* new_right_panel);
public:
	template <typename T1> void open_panel(panel_type type);
	void destroy_panel(panel_type type);
	template <typename T1> T1* get_panel(panel_type type);
private:
	std::array<panel_info, static_cast<int>(panel_type::enum_size)> panel_info_array;
	QTabWidget* right_panel;
	opengl_view_widget* view_widget;
};

template <typename T1> void right_panel_manager::open_panel(panel_type type){
	panel_info& info = panel_info_array[static_cast<int>(type)];
	if(info.widget){
		right_panel->setCurrentIndex(right_panel->indexOf(info.widget));
	}
	else{
		T1* tp = new T1;
		tp->set_view_widget(view_widget);
		info.widget = tp;
		right_panel->addTab(info.widget, info.widget->get_panel_title());
		right_panel->setCurrentIndex(right_panel->indexOf(info.widget));
	}
}
template <typename T1> T1* right_panel_manager::get_panel(panel_type type){
	return static_cast<T1*>(panel_info_array[static_cast<int>(type)].widget);
}

#endif