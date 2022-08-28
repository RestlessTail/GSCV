#ifndef OPENGL_VIEW_WIDGET_H
#define OPENGL_VIEW_WIDGET_H

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#include <windows.h>
#endif

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <filesystem>
#include <chrono>
#include "sc_reader.h"
#include "palette.h"

class string;
class vector;
class plot_base;

enum class mouse_status_t {
	left,
	right,
	mid,
	release
};

struct cell_pos_lim_t {
	float x_max;
	float y_max;
	float x_min;
	float y_min;
};

class opengl_view_widget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	opengl_view_widget(QWidget *parent);
	~opengl_view_widget();
	void open(std::filesystem::path filename);
	template <typename T1> void update_view(plot_base* plot_type);
public:
	float get_point_size(){ return point_size; }
	float get_scale(){ return scale; }
	int get_meta_preview(std::string& meta_name, std::vector<std::string>& dest, int top_n);
	size_t get_n_cell();
	size_t get_n_gene();
	std::string get_archive_name();
	std::vector<std::string>& get_meta();
	void set_scale(float scale, bool is_update);
	void set_point_size(float size, bool is_update);
	constexpr float get_scale_lim_max(){ return 10.0f; }
	constexpr float get_scale_lim_min(){ return -10.0f; }
	constexpr float get_size_lim_max(){ return 20.0f; }
	constexpr float get_size_lim_min(){ return 1.0f; }
public:
	sc_reader reader;
	color_type active_color_type;
	QOpenGLBuffer VBO;
	palette_t palette;
	std::chrono::system_clock::time_point last_update_time;
private:
	void update_cell_pos_lim();
	void update_view_mat();
	QOpenGLVertexArrayObject VAO;
	QOpenGLShaderProgram shader_program;
	QMatrix4x4 view_mat;
private:
	float point_size;
	float scale;
	std::pair<float, float> translation;
	cell_pos_lim_t cell_pos_lim;
private:
	mouse_status_t mouse_status;
	std::pair<float, float> mouse_position;
protected:
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void mouseMoveEvent(QMouseEvent* e) override;
	virtual void mousePressEvent(QMouseEvent* e) override;
	virtual void mouseReleaseEvent(QMouseEvent* e) override;
signals:
	void scale_changed(float val);
	void size_changed(float val);
};

template <typename T1> void opengl_view_widget::update_view(plot_base* plot_type){
	makeCurrent();
	glClearColor(palette.background.r(), palette.background.g(), palette.background.b(), 1.0f);
	VAO.bind();
	VBO.bind();
	T1* tp = static_cast<T1*>(plot_type);
	tp->fill_VBO();
	update();
}

#endif // OPENGL_VIEW_WIDGET_H