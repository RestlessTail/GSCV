#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <math.h>
#include "settings.h"
#include "opengl_view_widget.h"
#include "global_variables.h"
#include "palette.h"

opengl_view_widget::opengl_view_widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    point_size = 5.0f;
    active_color_type = color_type::solid;
    palette.high.set_color(255.0f, 0.0f, 0.0f);
    palette.mid.set_color(200.0f, 200.0f, 0.0f);
    palette.low.set_color(0.0f, 200.0f, 200.0f);
    palette.background.set_color(255.0f, 255.0f, 255.0f);
    palette.exp_color_type = color_type::gradient_2;
    palette.inactive.set_color(200.0f, 200.0f, 200.0f);
    palette.meta_colors = {palette.get_color_at(0.0f),
                           palette.get_color_at(60.0f),
                           palette.get_color_at(120.0f),
                           palette.get_color_at(180.0f),
                           palette.get_color_at(240.0f),
                           palette.get_color_at(300.0f)
                          };
    scale = 0.0f;
    translation = {0.0f, 0.0f};
    mouse_status = mouse_status_t::release;
    mouse_position = {0, 0};
    cell_pos_lim = {0.0f, 0.0f, 0.0f, 0.0f};
}
opengl_view_widget::~opengl_view_widget()
{
    VBO.destroy();
    VAO.destroy();
}
void opengl_view_widget::find_exp_extremes()
{
    float *expr_data = reader.get_active_expr_data();
    color_val_max = color_val_min = expr_data[0];
    int n_cell = reader.get_n_cell();
    for (int i = 0; i < n_cell; ++i)
    {
        if (expr_data[i] > color_val_max)
        {
            color_val_max = expr_data[i];
        }
        if (expr_data[i] < color_val_min)
        {
            color_val_min = expr_data[i];
        }
    }
}
void opengl_view_widget::update_cell_pos_lim()
{
    float *pos_data = reader.get_cell_position();
    int array_size = 2 * reader.get_n_cell();
    for (int i = 0; i < array_size;)
    {
        if (pos_data[i] > cell_pos_lim.x_max)
        {
            cell_pos_lim.x_max = pos_data[i];
        }
        if (pos_data[i] < cell_pos_lim.x_min)
        {
            cell_pos_lim.x_min = pos_data[i];
        }
        ++i;
        if (pos_data[i] > cell_pos_lim.y_max)
        {
            cell_pos_lim.y_max = pos_data[i];
        }
        if (pos_data[i] < cell_pos_lim.y_min)
        {
            cell_pos_lim.y_min = pos_data[i];
        }
        ++i;
    }
}
void opengl_view_widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(palette.background.r(), palette.background.g(), palette.background.b(), 1.0f);
    glEnable(GL_PROGRAM_POINT_SIZE);
    QOpenGLShader *vert_shader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    QOpenGLShader *frag_shader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    vert_shader->compileSourceFile(global_variables::default_shader_vert);
    frag_shader->compileSourceFile(global_variables::default_shader_frag);
    VAO.create();
    VBO.create();
    VAO.bind();
    VBO.bind();
    shader_program.create();
    shader_program.addShader(vert_shader);
    shader_program.addShader(frag_shader);
    shader_program.link();
    shader_program.bind();
    shader_program.setAttributeBuffer(0, GL_FLOAT, 0 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
    shader_program.enableAttributeArray(0);
    shader_program.setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(GLfloat), 3, 5 * sizeof(GLfloat));
    shader_program.enableAttributeArray(1);
}
void opengl_view_widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (!reader.is_active)
    {
        return;
    }
    VAO.bind();
    shader_program.bind();
    shader_program.setUniformValue("point_size", point_size);
    shader_program.setUniformValue("view_mat", view_mat);
    glDrawArrays(GL_POINTS, 0, reader.get_n_cell());
}
void opengl_view_widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}
void opengl_view_widget::mouseMoveEvent(QMouseEvent *e)
{
    if (mouse_status == mouse_status_t::left)
    {
        float x = 1.0f - 2.0f * e->pos().x() / width();
        float y = 2.0f * e->pos().y() / height() - 1.0f;
        float dx = mouse_position.first - x;
        float dy = mouse_position.second - y;
        mouse_position.first = x;
        mouse_position.second = y;
        translation.first += dx;
        translation.second += dy;
        float x_max = cell_pos_lim.x_max * pow(2, scale);
        float x_min = cell_pos_lim.x_min * pow(2, scale);
        float y_max = cell_pos_lim.y_max * pow(2, scale);
        float y_min = cell_pos_lim.y_min * pow(2, scale);
        if (x_max - x_min <= 0.0f)
        {
            translation.first = (x_max + x_min) / 2;
        }
        else
        {
            if (x_max < translation.first)
            {
                translation.first = x_max;
            }
            if (x_min > translation.first)
            {
                translation.first = x_min;
            }
        }
        if (y_max - y_min <= 0.0f)
        {
            translation.second = (y_max + y_min) / 2;
        }
        else
        {
            if (y_max < translation.second)
            {
                translation.second = y_max;
            }
            if (y_min > translation.second)
            {
                translation.second = y_min;
            }
        }
        if (global_variables::settings->repaint_instantly)
        {
            auto now = std::chrono::system_clock::now();
            auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update_time);
            if (time_elapsed.count() >= global_variables::settings->frame_delay)
            {
                update_view_mat();
                last_update_time = now;
            }
        }
    }
}
void opengl_view_widget::mousePressEvent(QMouseEvent *e)
{
    Qt::MouseButton b = e->button();
    switch (b)
    {
    case Qt::LeftButton:
        mouse_status = mouse_status_t::left;
        break;
    default:
        break;
    }
    mouse_position.first = 1.0f - 2.0f * e->pos().x() / width();
    mouse_position.second = 2.0f * e->pos().y() / height() - 1.0f;
}
void opengl_view_widget::mouseReleaseEvent(QMouseEvent *e)
{
    mouse_status = mouse_status_t::release;
    update_view_mat();
}
/*
void opengl_view_widget::update_vertex_solid()
{
    makeCurrent();
    glClearColor(palette.background.r(), palette.background.g(), palette.background.b(), 1.0f);
    VAO.bind();
    VBO.bind();
    size_t buffer_size = 5 * reader.get_n_cell();
    GLfloat *vertex_data = new GLfloat[buffer_size];

    active_color_type = color_type::solid;
    fill_VBO_solid(vertex_data);

    VBO.allocate(vertex_data, buffer_size * sizeof(GLfloat));
    delete[] vertex_data;
    update();
}*/
void opengl_view_widget::set_point_size(float size, bool is_update)
{
    point_size = size;
    emit size_changed(get_point_size());
    if (is_update)
    {
        update();
    }
}
void opengl_view_widget::set_scale(float s, bool is_update)
{
    scale = s;
    emit scale_changed(get_scale());
    if (is_update)
    {
        update_view_mat();
    }
}
void opengl_view_widget::update_view_mat()
{
    QMatrix4x4 new_mat;
    new_mat.translate(translation.first, translation.second);
    new_mat.scale(pow(2, scale));
    view_mat = new_mat;
    update();
}
void opengl_view_widget::update_vertex_exp(std::string &gene_name)
{
    // set OpenGL context
    makeCurrent();
    glClearColor(palette.background.r(), palette.background.g(), palette.background.b(), 1.0f);
    VAO.bind();
    VBO.bind();
    // allocate memory
    size_t buffer_size = 5 * reader.get_n_cell();
    GLfloat *vertex_data = new GLfloat[buffer_size];
    // fill VBO
    active_color_type = palette.exp_color_type;
    reader.set_active_expr_data(gene_name);
    fill_VBO_exp(vertex_data);

    VBO.allocate(vertex_data, buffer_size * sizeof(GLfloat));
    delete[] vertex_data;
    update();
}
void opengl_view_widget::update_vertex_meta(int meta_index, bool continuous)
{
    makeCurrent();
    glClearColor(palette.background.r(), palette.background.g(), palette.background.b(), 1.0f);
    VAO.bind();
    VBO.bind();
    size_t buffer_size = 5 * reader.get_n_cell();
    GLfloat *vertex_data = new GLfloat[buffer_size];
    if (continuous)
    {
        active_color_type = palette.exp_color_type;
    }
    else
    {
        active_color_type = color_type::discrete;
    }
    fill_VBO_meta(vertex_data, continuous, meta_index);
    VBO.allocate(vertex_data, buffer_size * sizeof(GLfloat));
    delete[] vertex_data;
    update();
}
void opengl_view_widget::fill_VBO_exp(GLfloat *dest)
{
    switch (active_color_type)
    {
    case color_type::gradient_2:
        fill_VBO_exp_gradient_2(dest);
        break;
    case color_type::gradient_3:
        fill_VBO_exp_gradient_3(dest);
        break;
    default:
        break;
    }
}
void opengl_view_widget::fill_VBO_exp_gradient_2(GLfloat *dest)
{
    find_exp_extremes();
    int offset = 0;
    float *pos_data = reader.get_cell_position();
    float *expr_data = reader.get_active_expr_data();
    size_t array_size = 2 * reader.get_n_cell();
    size_t pos_index = 0;
    size_t expr_index = 0;
    for (; pos_index < array_size;)
    {
        dest[offset++] = pos_data[pos_index++];
        dest[offset++] = pos_data[pos_index++];
        palette.calculate_gradient_2(color_val_max, color_val_min, expr_data[expr_index++], dest + offset);
        offset += 3;
    }
}
void opengl_view_widget::fill_VBO_exp_gradient_3(GLfloat *dest)
{
    find_exp_extremes();
    int offset = 0;
    float midpoint = (color_val_max - color_val_min) / 2 + color_val_min;
    float *pos_data = reader.get_cell_position();
    float *expr_data = reader.get_active_expr_data();
    size_t array_size = 2 * reader.get_n_cell();
    size_t pos_index = 0;
    size_t expr_index = 0;
    for (; pos_index < array_size;)
    {
        dest[offset++] = pos_data[pos_index++];
        dest[offset++] = pos_data[pos_index++];
        palette.calculate_gradient_3(color_val_max, color_val_min, midpoint, expr_data[expr_index++], dest + offset);
        offset += 3;
    }
}
void opengl_view_widget::fill_VBO_meta(GLfloat *dest, bool gradient, int meta_index)
{
    if (gradient)
    {
        switch (active_color_type)
        {
        case color_type::gradient_2:
            fill_VBO_meta_continuous_gradient_2(dest, meta_index);
            break;
        case color_type::gradient_3:
            fill_VBO_meta_continuous_gradient_3(dest, meta_index);
            break;
        default:
            break;
        }
    }
    else
    {
        switch (active_color_type)
        {
        case color_type::discrete:
            fill_VBO_meta_categorical(dest, meta_index);
            break;
        default:
            break;
        }
    }
}
void opengl_view_widget::fill_VBO_meta_continuous_gradient_2(GLfloat *dest, int meta_index)
{
    int n_cell = reader.get_n_cell();
    float *tmp_meta = new float[n_cell];

    float max = safe_atof(reader.get_meta_val(0, meta_index).c_str());
    float min = max;
    for (int i = 0; i < n_cell; ++i)
    {
        tmp_meta[i] = safe_atof(reader.get_meta_val(i, meta_index).c_str());
        if (tmp_meta[i] > max)
        {
            max = tmp_meta[i];
        }
        if (tmp_meta[i] < min)
        {
            min = tmp_meta[i];
        }
    }
    size_t offset = 0;
    float *pos_data = reader.get_cell_position();
    size_t tmp_index = 0;
    size_t pos_index = 0;
    for (int i = 0; i < n_cell; ++i)
    {
        dest[offset++] = pos_data[pos_index++];
        dest[offset++] = pos_data[pos_index++];
        palette.calculate_gradient_2(max, min, tmp_meta[tmp_index++], dest + offset);
        offset += 3;
    }
    delete[] tmp_meta;
}
void opengl_view_widget::fill_VBO_meta_continuous_gradient_3(GLfloat *dest, int meta_index)
{
    int n_cell = reader.get_n_cell();
    float *tmp_meta = new float[n_cell];

    float max = safe_atof(reader.get_meta_val(0, meta_index).c_str());
    float min = max;
    for (int i = 0; i < n_cell; ++i)
    {
        tmp_meta[i] = safe_atof(reader.get_meta_val(i, meta_index).c_str());
        if (tmp_meta[i] > max)
        {
            max = tmp_meta[i];
        }
        if (tmp_meta[i] < min)
        {
            min = tmp_meta[i];
        }
    }
    size_t offset = 0;
    float *pos_data = reader.get_cell_position();
    size_t tmp_index = 0;
    size_t pos_index = 0;
    float midpoint = min + (max - min) / 2;
    for (int i = 0; i < n_cell; ++i)
    {
        dest[offset++] = pos_data[pos_index++];
        dest[offset++] = pos_data[pos_index++];
        palette.calculate_gradient_3(max, min, midpoint, tmp_meta[tmp_index++], dest + offset);
        offset += 3;
    }
    delete[] tmp_meta;
}
void opengl_view_widget::fill_VBO_meta_categorical(GLfloat *dest, int meta_index)
{
    int n_cell = reader.get_n_cell();
    int meta_id = 0;
    QMap<std::string, int> map;
    for (int i = 0; i < n_cell; ++i)
    {
        auto map_itr = map.find(reader.get_meta_val(i, meta_index));
        if (map_itr == map.end())
        {
            map.insert(reader.get_meta_val(i, meta_index), meta_id++);
        }
    }
    int offset = 0;
    float *pos_data = reader.get_cell_position();
    size_t pos_index = 0;
    for (int i = 0; i < n_cell; ++i)
    {
        dest[offset++] = pos_data[pos_index++];
        dest[offset++] = pos_data[pos_index++];
        meta_id = map.find(reader.get_meta_val(i, meta_index)).value();
        if (meta_id >= palette.meta_colors.size())
        {
            dest[offset++] = 0.0f;
            dest[offset++] = 0.0f;
            dest[offset++] = 0.0f;
        }
        else
        {
            rgb_color &color = palette.meta_colors[meta_id];
            dest[offset++] = color.r();
            dest[offset++] = color.g();
            dest[offset++] = color.b();
        }
    }
}
float opengl_view_widget::safe_atof(const char *s)
{
    char c = 0;
    const char *cp = s;
    while ((c = *(cp++)))
    {
        if (!isdigit(c) && c != '.')
        {
            return 0.0f;
        }
    }
    return atof(s);
}
int opengl_view_widget::set_gene(std::string &gene_name)
{
    if (reader.test_gene_name(gene_name))
    {
        return -1;
    }
    else
    {
        if (reader.is_active)
        {
            update_vertex_exp(gene_name);
        }
        return 0;
    }
}
int opengl_view_widget::set_meta(std::string &meta_name, bool continuous)
{
    int meta_index = reader.test_meta_name(meta_name);
    if (meta_index == -1)
    {
        return -1;
    }
    else
    {
        if (reader.is_active)
        {
            update_vertex_meta(meta_index, continuous);
        }
        return 0;
    }
}
std::vector<std::string> &opengl_view_widget::get_meta()
{
    return reader.get_meta_name();
}
void opengl_view_widget::clear_color()
{
    if (reader.is_active)
    {
        //update_vertex_solid();
    }
}
int opengl_view_widget::get_meta_preview(std::string &meta_name, std::vector<std::string> &dest, int top_n)
{
    int meta_index = reader.test_meta_name(meta_name);
    if (meta_index == -1)
    {
        return -1;
    }
    dest.resize(top_n);
    for (int i = 0; i < top_n; ++i)
    {
        dest[i] = reader.get_meta_val(i, meta_index);
    }
    return 0;
}
void opengl_view_widget::open(std::filesystem::path filename)
{
    reader.open(filename);
    update_cell_pos_lim();
    translation.first = 0.0f;
    translation.second = 0.0f;
}
palette_t *opengl_view_widget::get_palette()
{
    return &palette;
}
size_t opengl_view_widget::get_n_cell()
{
    return reader.get_n_cell();
}
size_t opengl_view_widget::get_n_gene()
{
    return reader.get_n_gene();
}
std::string opengl_view_widget::get_archive_name()
{
    return reader.get_archive_name();
}