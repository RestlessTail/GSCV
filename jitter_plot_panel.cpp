#include "jitter_plot_panel.h"
#include "ui_jitter_plot_panel.h"
#include "set_order_dialog.h"

jitter_plot_panel::jitter_plot_panel(QWidget *parent) :
    right_panel_base(parent),
    ui(new Ui::jitter_plot_panel)
{
    ui->setupUi(this);
}
jitter_plot_panel::~jitter_plot_panel(){
    delete ui;
}
void jitter_plot_panel::on_x_customize_clicked() {
}
void jitter_plot_panel::on_y_customize_clicked() {
}
void jitter_plot_panel::on_update_clicked() {
}
void jitter_plot_panel::on_close_clicked() {
}
void jitter_plot_panel::update_preview_x() {
}
void jitter_plot_panel::update_preview_y() {
}