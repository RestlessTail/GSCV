#ifndef COLOR_SCHEME_H
#define COLOR_SCHEME_H

#include <QWidget>
#include "palette.h"

class QHBoxLayout;
class QLabel;
class QToolButton;
class QTabWidget;

namespace Ui {
class color_scheme;
}
class color_scheme : public QWidget
{
    Q_OBJECT
public:
    explicit color_scheme(QWidget *parent = nullptr);
    ~color_scheme();
    void set_palette(palette_t* p);
    void set_gradient_high(rgb_color& color);
    void set_gradient_mid(rgb_color& color);
    void set_gradient_low(rgb_color& color);
    void set_discrete(color_set& color);
    void set_inactive(rgb_color& color);
    void set_background(rgb_color& color);
    void set_tab_widget_master(QTabWidget *newTab_widget_master);
    void set_to_this(color_scheme **newTo_this);
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
    QTabWidget* tab_widget_master;
    color_scheme** to_this;
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

