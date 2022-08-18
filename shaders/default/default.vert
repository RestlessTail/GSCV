#version 330 core
layout (location = 0) in vec2 cell_pos;
layout (location = 1) in vec3 cell_color_raw;

uniform float point_size;
uniform mat4 view_mat;

out vec3 cell_color;

void main()
{
    gl_Position = view_mat * vec4(cell_pos.x, cell_pos.y, 0.0f, 1.0f);
	gl_PointSize = point_size;
	cell_color = cell_color_raw;
}
