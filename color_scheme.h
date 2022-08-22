#ifndef COLOR_SCHEME_H
#define COLOR_SCHEME_H

#include "right_panel_manager.h"
#include "palette.h"

class QHBoxLayout;
class QLabel;
class QToolButton;
class QTabWidget;

namespace Ui {
class color_scheme;
}
class color_scheme : public right_panel_base
{
    Q_OBJECT
public:
    explicit color_scheme(QWidget *parent = nullptr);
    ~color_scheme();
    void set_view_widget(opengl_view_widget *new_view_widget) override;
    void set_gradient_high(rgb_color& color);
    void set_gradient_mid(rgb_color& color);
    void set_gradient_low(rgb_color& color);
    void set_discrete(color_set& color);
    void set_inactive(rgb_color& color);
    void set_background(rgb_color& color);
	void add_discrete_color(rgb_color& color, int index);
private slots:
    void on_gradient_bicolored_clicked();
    void on_gradient_tricolored_clicked();
    void on_gradient_high_select_clicked();
    void on_gradient_mid_select_clicked();
    void on_gradient_low_select_clicked();
    void on_inactive_select_clicked();
    void on_background_select_clicked();
    void on_discrete_add_clicked();
    void on_discrete_remove_clicked();
    void on_discrete_up_clicked();
    void on_discrete_down_clicked();
    void on_close_clicked();
private:
    Ui::color_scheme *ui;
    palette_t* palette;
};
class discrete_preview_item : public QWidget{
    Q_OBJECT
public:
    explicit discrete_preview_item();
	void set_color(rgb_color& color);
	rgb_color get_color();
	int color_index;
	palette_t* palette;
	void update_color();
private:
	rgb_color color_value;
	QHBoxLayout* layout;
	QLabel* hex_label;
	QToolButton* color_select;
};
#endif // COLOR_SCHEME_H

