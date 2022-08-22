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
	float get_scale(){ return scale; }
	void set_scale(float scale, bool is_update);
    constexpr float get_scale_lim_max(){ return 10.0f; }
    constexpr float get_scale_lim_min(){ return -10.0f; }
    int get_meta_preview(std::string& meta_name, std::vector<std::string>& dest, int top_n);
    int set_gene(std::string& gene_name);
    int set_meta(std::string& meta_name, bool continuous);
    std::vector<std::string>& get_meta();
    void clear_color();
    void open(std::filesystem::path filename);
    palette_t* get_palette();
    size_t get_n_cell();
    size_t get_n_gene();
    std::string get_archive_name();
	void set_point_size(float size, bool is_update);
	float get_point_size(){ return point_size; }
    constexpr float get_size_lim_max(){ return 20.0f; }
    constexpr float get_size_lim_min(){ return 1.0f; }
    std::chrono::system_clock::time_point last_update_time;
	sc_reader reader;
	color_type active_color_type;
	QOpenGLBuffer VBO;
    palette_t palette;
    template <typename T1> void update_view(plot_base* plot_type);
private:
    float color_val_max;
    float color_val_min;
    void find_exp_extremes();
    void update_cell_pos_lim();
    //void update_vertex_solid();
    void update_vertex_exp(std::string& gene_name);
    void update_vertex_meta(int meta_index, bool continuous);
	void update_view_mat();
	QOpenGLVertexArrayObject VAO;
	QOpenGLShaderProgram shader_program;
	QMatrix4x4 view_mat;
signals:
	void scale_changed(float val);
	void size_changed(float val);
private:
	//void fill_VBO_solid(GLfloat* dest);
    void fill_VBO_exp(GLfloat* dest);
	void fill_VBO_exp_gradient_2(GLfloat* dest);
	void fill_VBO_exp_gradient_3(GLfloat* dest);
    void fill_VBO_meta(GLfloat* dest, bool gradient, int gene_index);
	void fill_VBO_meta_continuous_gradient_2(GLfloat* dest, int meta_index);
	void fill_VBO_meta_continuous_gradient_3(GLfloat* dest, int meta_index);
    void fill_VBO_meta_categorical(GLfloat* dest, int meta_index);
	float safe_atof(const char* s);
    float point_size;
	float scale;
    std::pair<float, float> translation;
    cell_pos_lim_t cell_pos_lim;
protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void mouseMoveEvent(QMouseEvent* e) override;
    virtual void mousePressEvent(QMouseEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;
private:
    mouse_status_t mouse_status;
    std::pair<float, float> mouse_position;
private:
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