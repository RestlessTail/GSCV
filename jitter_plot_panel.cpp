#include "jitter_plot_panel.h"
#include "ui_jitter_plot_panel.h"

jitter_plot_panel::jitter_plot_panel(QWidget *parent) :
    right_panel_base(parent),
    ui(new Ui::jitter_plot_panel)
{
    ui->setupUi(this);
}
jitter_plot_panel::~jitter_plot_panel(){
    delete ui;
}
void jitter_plot_panel::on_x_set_order_clicked(){
}
void jitter_plot_panel::on_y_set_order_clicked(){
}
void jitter_plot_panel::on_update_clicked(){
}
void jitter_plot_panel::on_close_clicked(){
}