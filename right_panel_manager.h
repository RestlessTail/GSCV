#ifndef RIGHT_PANEL_MANAGER
#define RIGHT_PANEL_MANAGER

#include <array>
#include <QWidget>

class QTabWidget;
class opengl_view_widget;

enum class panel_type{
	cell_color,
	enum_size
};

class right_panel_base : public QWidget{
public:
	void set_view_widget(opengl_view_widget* new_view_widget);
private:
	opengl_view_widget* view_widget;
};

struct panel_info{
	panel_info();
	right_panel_base* widget;
};

class right_panel_manager{
public:
	template <typename T1> void open_panel(panel_type type);
	void destroy_panel(panel_type type);
public:
	void set_view_widget(opengl_view_widget* new_view_widget);
	void set_right_panel(QTabWidget* new_right_panel);
private:
	std::array<panel_info, static_cast<int>(panel_type::enum_size)> panel_info_array;
	QTabWidget* right_panel;
	opengl_view_widget* view_widget;
};

#endif