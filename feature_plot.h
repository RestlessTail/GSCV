#ifndef FEATURE_PLOT
#define FEATURE_PLOT

#include <string>
#include <QOpenGLFunctions_3_3_Core>
#include "plot_base.h"

class feature_plot_expr : public plot_base{
public:
	void fill_VBO() override;
	int set_gene(std::string& new_gene_name);
private:
	std::string gene_name;
	float color_val_max;
	float color_val_min;
	void find_exp_extremes();
	void fill_VBO_exp_gradient_2(GLfloat* dest);
	void fill_VBO_exp_gradient_3(GLfloat* dest);
};

class feature_plot_meta : public plot_base{
public:
	void fill_VBO() override;
	int set_meta(std::string& new_meta_name, bool new_is_continuous);
private:
	int meta_index;
	bool is_continuous;
	void fill_VBO_meta_continuous_gradient_2(GLfloat* dest, int meta_index);
	void fill_VBO_meta_continuous_gradient_3(GLfloat* dest, int meta_index);
	void fill_VBO_meta_categorical(GLfloat* dest, int meta_index);
};

#endif