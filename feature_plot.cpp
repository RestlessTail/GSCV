#include "feature_plot.h"
#include "opengl_view_widget.h"

int feature_plot_expr::set_gene(std::string &new_gene_name)
{
    if (view_widget->reader.test_gene_name(new_gene_name)) {
        return -1;
    }
    else {
        gene_name = new_gene_name;
        return 0;
    }
}

void feature_plot_expr::fill_VBO() {
    size_t buffer_size = 5 * view_widget->reader.get_n_cell();
    GLfloat *vertex_data = new GLfloat[buffer_size];
    view_widget->active_color_type = view_widget->palette.exp_color_type;
    view_widget->reader.set_active_expr_data(gene_name);

    /******************************/
    switch (view_widget->active_color_type)
    {
    case color_type::gradient_2:
        fill_VBO_exp_gradient_2(vertex_data);
        break;
    case color_type::gradient_3:
        fill_VBO_exp_gradient_3(vertex_data);
        break;
    default:
        break;
    }
    /******************************/

    view_widget->VBO.allocate(vertex_data, buffer_size * sizeof(GLfloat));
    delete[] vertex_data;
}

void feature_plot_expr::fill_VBO_exp_gradient_2(GLfloat *dest)
{
    find_exp_extremes();
    int offset = 0;
    float *pos_data = view_widget->reader.get_cell_position();
    float *expr_data = view_widget->reader.get_active_expr_data();
    size_t array_size = 2 * view_widget->reader.get_n_cell();
    size_t pos_index = 0;
    size_t expr_index = 0;
    for (; pos_index < array_size;)
    {
        dest[offset++] = pos_data[pos_index++];
        dest[offset++] = pos_data[pos_index++];
        view_widget->palette.calculate_gradient_2(color_val_max, color_val_min, expr_data[expr_index++], dest + offset);
        offset += 3;
    }
}
void feature_plot_expr::fill_VBO_exp_gradient_3(GLfloat *dest) {
    find_exp_extremes();
    int offset = 0;
    float midpoint = (color_val_max - color_val_min) / 2 + color_val_min;
    float *pos_data = view_widget->reader.get_cell_position();
    float *expr_data = view_widget->reader.get_active_expr_data();
    size_t array_size = 2 * view_widget->reader.get_n_cell();
    size_t pos_index = 0;
    size_t expr_index = 0;
    for (; pos_index < array_size;)
    {
        dest[offset++] = pos_data[pos_index++];
        dest[offset++] = pos_data[pos_index++];
        view_widget->palette.calculate_gradient_3(color_val_max, color_val_min, midpoint, expr_data[expr_index++], dest + offset);
        offset += 3;
    }
}
void feature_plot_expr::find_exp_extremes() {
    float *expr_data = view_widget->reader.get_active_expr_data();
    color_val_max = color_val_min = expr_data[0];
    int n_cell = view_widget->reader.get_n_cell();
    for (int i = 0; i < n_cell; ++i) {
        if (expr_data[i] > color_val_max) {
            color_val_max = expr_data[i];
        }
        if (expr_data[i] < color_val_min) {
            color_val_min = expr_data[i];
        }
    }
}

int feature_plot_meta::set_meta(std::string& new_meta_name, bool new_is_continuous) {
    int new_meta_index = view_widget->reader.test_meta_name(new_meta_name);
    if (new_meta_index == -1) {
        return -1;
    }
    else {
        meta_index = new_meta_index;
        is_continuous = new_is_continuous;
        return 0;
    }
}

void feature_plot_meta::fill_VBO() {
    size_t buffer_size = 5 * view_widget->reader.get_n_cell();
    GLfloat *vertex_data = new GLfloat[buffer_size];
    if (is_continuous) {
        view_widget->active_color_type = view_widget->palette.exp_color_type;
    }
    else {
        view_widget->active_color_type = color_type::discrete;
    }
    /******************************/
    if (is_continuous) {
        switch (view_widget->active_color_type) {
        case color_type::gradient_2:
            fill_VBO_meta_continuous_gradient_2(vertex_data, meta_index);
            break;
        case color_type::gradient_3:
            fill_VBO_meta_continuous_gradient_3(vertex_data, meta_index);
            break;
        default:
            break;
        }
    }
    else {
        switch (view_widget->active_color_type) {
        case color_type::discrete:
            fill_VBO_meta_categorical(vertex_data, meta_index);
            break;
        default:
            break;
        }
    }
    /******************************/
    view_widget->VBO.allocate(vertex_data, buffer_size * sizeof(GLfloat));
    delete[] vertex_data;
}
void feature_plot_meta::fill_VBO_meta_continuous_gradient_2(GLfloat *dest, int meta_index)
{
    int n_cell = view_widget->reader.get_n_cell();
    float *tmp_meta = new float[n_cell];

    float max = safe_atof(view_widget->reader.get_meta_val(0, meta_index).c_str());
    float min = max;
    for (int i = 0; i < n_cell; ++i)
    {
        tmp_meta[i] = safe_atof(view_widget->reader.get_meta_val(i, meta_index).c_str());
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
    float *pos_data = view_widget->reader.get_cell_position();
    size_t tmp_index = 0;
    size_t pos_index = 0;
    for (int i = 0; i < n_cell; ++i)
    {
        dest[offset++] = pos_data[pos_index++];
        dest[offset++] = pos_data[pos_index++];
        view_widget->palette.calculate_gradient_2(max, min, tmp_meta[tmp_index++], dest + offset);
        offset += 3;
    }
    delete[] tmp_meta;
}
void feature_plot_meta::fill_VBO_meta_continuous_gradient_3(GLfloat *dest, int meta_index)
{
    int n_cell = view_widget->reader.get_n_cell();
    float *tmp_meta = new float[n_cell];

    float max = safe_atof(view_widget->reader.get_meta_val(0, meta_index).c_str());
    float min = max;
    for (int i = 0; i < n_cell; ++i)
    {
        tmp_meta[i] = safe_atof(view_widget->reader.get_meta_val(i, meta_index).c_str());
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
    float *pos_data = view_widget->reader.get_cell_position();
    size_t tmp_index = 0;
    size_t pos_index = 0;
    float midpoint = min + (max - min) / 2;
    for (int i = 0; i < n_cell; ++i)
    {
        dest[offset++] = pos_data[pos_index++];
        dest[offset++] = pos_data[pos_index++];
        view_widget->palette.calculate_gradient_3(max, min, midpoint, tmp_meta[tmp_index++], dest + offset);
        offset += 3;
    }
    delete[] tmp_meta;
}
void feature_plot_meta::fill_VBO_meta_categorical(GLfloat *dest, int meta_index)
{
    int n_cell = view_widget->reader.get_n_cell();
    int meta_id = 0;
    QMap<std::string, int> map;
    for (int i = 0; i < n_cell; ++i)
    {
        auto map_itr = map.find(view_widget->reader.get_meta_val(i, meta_index));
        if (map_itr == map.end())
        {
            map.insert(view_widget->reader.get_meta_val(i, meta_index), meta_id++);
        }
    }
    int offset = 0;
    float *pos_data = view_widget->reader.get_cell_position();
    size_t pos_index = 0;
    for (int i = 0; i < n_cell; ++i)
    {
        dest[offset++] = pos_data[pos_index++];
        dest[offset++] = pos_data[pos_index++];
        meta_id = map.find(view_widget->reader.get_meta_val(i, meta_index)).value();
        if (meta_id >= view_widget->palette.meta_colors.size())
        {
            dest[offset++] = 0.0f;
            dest[offset++] = 0.0f;
            dest[offset++] = 0.0f;
        }
        else
        {
            rgb_color &color = view_widget->palette.meta_colors[meta_id];
            dest[offset++] = color.r();
            dest[offset++] = color.g();
            dest[offset++] = color.b();
        }
    }
}