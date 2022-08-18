#version 330 core
in vec3 cell_color;

void main()
{
    gl_FragColor = vec4(cell_color.x, cell_color.y, cell_color.z, 1.0f);
} 
