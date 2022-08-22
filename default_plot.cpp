#include "default_plot.h"
#include "opengl_view_widget.h"

//TODO: detach active_color_type

void default_plot::fill_VBO(){
    size_t buffer_size = 5 * view_widget->reader.get_n_cell();
    GLfloat *vertex_data = new GLfloat[buffer_size];

    view_widget->active_color_type = color_type::solid;

	/******************************/
    int offset = 0;
    float *pos_data = view_widget->reader.get_cell_position();
    size_t array_size = 2 * view_widget->reader.get_n_cell();
    for (int i = 0; i < array_size;){
        vertex_data[offset++] = pos_data[i++];
        vertex_data[offset++] = pos_data[i++];
        view_widget->palette.calculate_solid(vertex_data + offset);
        offset += 3;
    }
	/******************************/

    view_widget->VBO.allocate(vertex_data, buffer_size * sizeof(GLfloat));
    delete[] vertex_data;
}