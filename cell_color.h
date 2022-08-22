#ifndef CELL_COLOR_H
#define CELL_COLOR_H

#include "right_panel_manager.h"

class QTabWidget;
class opengl_view_widget;

namespace Ui {
class cell_color;
}

class cell_color : public right_panel_base
{
    Q_OBJECT
public:
    explicit cell_color(QWidget *parent = nullptr);
    ~cell_color();
    void set_view_widget(opengl_view_widget *new_view_widget) override;
    void update_data();

private slots:
    void on_search_gene_go_clicked();
    void on_meta_go_clicked();
    void on_clear_color_clicked();

    void on_meta_combobox_currentTextChanged(const QString &arg1);
    void on_close_panel_clicked();

private:
    opengl_view_widget* view_widget;
    Ui::cell_color *ui;
};

#endif // CELL_COLOR_H