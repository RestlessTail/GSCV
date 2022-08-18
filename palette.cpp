#include "palette.h"
void palette_t::calculate_solid(float* fill_to){
    memcpy(fill_to, inactive.rgb_channel, 3 * sizeof(float));
}

void palette_t::calculate_gradient_2(float max, float min, float val, float* fill_to){
	fill_to[rgb_color::r_channel()] = (high.r() - low.r()) * (val - min) / (max - min) + low.r();
	fill_to[rgb_color::g_channel()] = (high.g() - low.g()) * (val - min) / (max - min) + low.g();
	fill_to[rgb_color::b_channel()] = (high.b() - low.b()) * (val - min) / (max - min) + low.b();
}

void palette_t::calculate_gradient_3(float max, float min, float midpoint, float val, float* fill_to){
	if(val > midpoint){
		fill_to[rgb_color::r_channel()] = (high.r() - mid.r()) * (val - midpoint) / (max - midpoint) + mid.r();
		fill_to[rgb_color::g_channel()] = (high.g() - mid.g()) * (val - midpoint) / (max - midpoint) + mid.g();
		fill_to[rgb_color::b_channel()] = (high.b() - mid.b()) * (val - midpoint) / (max - midpoint) + mid.b();
	}
	else{
        fill_to[rgb_color::r_channel()] = (mid.r() - low.r()) * (val - min) / (midpoint - min) + low.r();
        fill_to[rgb_color::g_channel()] = (mid.g() - low.g()) * (val - min) / (midpoint - min) + low.g();
        fill_to[rgb_color::b_channel()] = (mid.b() - low.b()) * (val - min) / (midpoint - min) + low.b();
    }
}

rgb_color palette_t::get_color_at(float degree)
{
    float r = 0.0f, g = 0.0f, b = 0.0f;
    if (degree <= 60.0f) {
        r = 255.0f;
    }
    else if(degree <= 120.0f){
        r = (120.0f - degree) / 60.0f * 255.0f;
    }
    else if(degree <= 240.0f){
        r = 0.0f;
    }
    else if (degree <= 300.0f) {
        r = (degree - 240.0f) / 60.0f * 255.0f;
    }
    else {
        r = 255.0f;
    }
    if(degree < 60.0f){
        g = degree / 60.0f * 255.0f;
    }
    else if(degree <= 180.0f){
        g = 255.0f;
    }
    else if(degree < 240.0f){
        g = (240.0f - degree) / 60.0f * 255.0f;
    }
    else{
        g = 0.0f;
    }
    if(degree < 120.0f){
        b = 0.0f;
    }
    else if(degree < 180.0f){
        b = (degree - 120.0f) / 60.0f * 255.0f;
    }
    else if(degree < 300.0f){
        b = 255.0f;
    }
    else{
        b = (360.0f - degree) / 60.0f * 255.0f;
    }
    return rgb_color(r, g, b);
}
QString rgb_color::get_hex_color()
{
    char hex_color[8] = { 0 };
    sprintf(hex_color, "#%02x%02x%02x", r_raw(), g_raw(), b_raw());
    return QString(hex_color);
}
