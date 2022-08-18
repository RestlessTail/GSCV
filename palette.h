#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>

class rgb_color{
	public:
		rgb_color(){
			rgb_raw[r_channel()] = 0;
			rgb_raw[g_channel()] = 0;
			rgb_raw[b_channel()] = 0;
			rgb_channel[r_channel()] = 0.0f;
			rgb_channel[g_channel()] = 0.0f;
			rgb_channel[b_channel()] = 0.0f;
		}
		rgb_color(int r, int g, int b) {
			rgb_raw[r_channel()] = r;
			rgb_raw[g_channel()] = g;
			rgb_raw[b_channel()] = b;
			rgb_channel[r_channel()] = 1.0f * r / 256.0f;
			rgb_channel[g_channel()] = 1.0f * g / 256.0f;
			rgb_channel[b_channel()] = 1.0f * b / 256.0f;
		}
		void set_color(float r, float g, float b){
			rgb_raw[r_channel()] = r;
			rgb_raw[g_channel()] = g;
			rgb_raw[b_channel()] = b;
			rgb_channel[r_channel()] = r / 256.0f;
			rgb_channel[g_channel()] = g / 256.0f;
			rgb_channel[b_channel()] = b / 256.0f;
		}
		float rgb_channel[3];
		int rgb_raw[3];
		inline float r(){ return rgb_channel[r_channel()]; }
		inline float g(){ return rgb_channel[g_channel()]; }
		inline float b(){ return rgb_channel[b_channel()]; }
		inline int r_raw(){ return rgb_raw[r_channel()]; }
		inline int g_raw(){ return rgb_raw[g_channel()]; }
		inline int b_raw(){ return rgb_raw[b_channel()]; }
		static constexpr int r_channel(){ return 0; }
		static constexpr int g_channel(){ return 1; }
		static constexpr int b_channel(){ return 2; }
		QString get_hex_color();
};

enum class color_type{
	solid,
	gradient_2,
	gradient_3,
	discrete,
};

class color_set{
	public:
		rgb_color& operator[](int index){ return data[index]; }
		void operator=(std::vector<rgb_color>& init){ data = std::move(init); }
		void operator=(std::vector<rgb_color>&& init){ data = init; }
		size_t size(){ return data.size(); }
		void add(){
			rgb_color new_color;
			data.push_back(new_color);
		}
		void remove(int index){
			std::vector<rgb_color>::iterator itr = data.begin() + index;
			data.erase(itr);
		}
		void move_up(int index){
			if(index > 0){
				rgb_color tmp = data[index];
				data[index] = data[index - 1];
				data[index - 1] = tmp;
			}
		}
		void move_down(int index){
			if(index < data.size() - 1){
				rgb_color tmp = data[index];
				data[index] = data[index + 1];
				data[index + 1] = tmp;
			}
		}
	private:
		std::vector<rgb_color> data;
};

class palette_t{
	public:
		rgb_color high;
		rgb_color mid;
		rgb_color low;
		rgb_color background;
		rgb_color inactive;
		color_type exp_color_type;
		color_set meta_colors;
		void calculate_solid(float* fill_to);
		void calculate_gradient_2(float max, float min, float val, float* fill_to);
		void calculate_gradient_3(float max, float min, float midpoint, float val, float* fill_to);
		void gen_discrete_color(int size);
		//calculate the rgb value according to the hue ring
		rgb_color get_color_at(float degree);
};

#endif // PALETTE_H
