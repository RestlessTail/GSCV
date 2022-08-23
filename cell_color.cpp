#include <QMessageBox>
#include <string>
#include <vector>
#include "cell_color.h"
#include "ui_cell_color.h"
#include "utils.h"
#include "opengl_view_widget.h"
#include "global_variables.h"
#include "default_plot.h"
#include "feature_plot.h"

cell_color::cell_color(QWidget *parent) :
    right_panel_base(parent),
    ui(new Ui::cell_color)
{
    ui->setupUi(this);
    ui->radio_continous->setChecked(true);
    ui->radio_categorical->setChecked(false);
    setAttribute(Qt::WA_DeleteOnClose, true);
    set_panel_title(tr("Cell color"));
}
cell_color::~cell_color()
{
    delete ui;
}
void cell_color::set_view_widget(opengl_view_widget* widget){
    view_widget = widget;
    update_data();
}
void cell_color::on_search_gene_go_clicked(){
    std::string gene_name = ui->gene_symbol->text().toStdString();
    if(gene_name.empty()){
        return;
    }
    feature_plot_expr p;
    p.set_view_widget(view_widget);
    if(p.set_gene(gene_name)){
        QString q_gene_name = QString::fromStdString(gene_name);
        QMessageBox::information(this, QString(tr("Information")), QString(tr("Failed to find ")) + q_gene_name + QString(tr(".")));
    }
    view_widget->update_view<feature_plot_expr>(&p);
}
void cell_color::on_meta_go_clicked()
{
    std::string meta_name = ui->meta_combobox->currentText().toStdString();
    bool continuous = ui->radio_continous->isChecked();
    if(meta_name.empty()){
        return;
    }
    feature_plot_meta p;
    p.set_view_widget(view_widget);
    if(p.set_meta(meta_name, continuous)){
        QMessageBox::critical(this, QString(tr("Critical")), QString(tr("Internal error.")));
    }
    view_widget->update_view<feature_plot_meta>(&p);
}
void cell_color::on_clear_color_clicked()
{
    default_plot p;
    p.set_view_widget(view_widget);
    view_widget->update_view<default_plot>(&p);
}
void cell_color::on_meta_combobox_currentTextChanged(const QString &arg1)
{
    ui->meta_preview_widget->clear();
    std::vector<std::string> preview;
    std::string meta_name = arg1.toStdString();
    view_widget->get_meta_preview(meta_name, preview, 20);
    for(auto& i : preview){
        ui->meta_preview_widget->addItem(QString::fromStdString(i));
    }
}
void cell_color::on_close_panel_clicked(){
    global_variables::panel_manager->destroy_panel(panel_type::cell_color);
}
void cell_color::update_data()
{
    ui->meta_combobox->clear();
    std::vector<std::string>& meta_dict = view_widget->get_meta();
    for (auto& i : meta_dict) {
        ui->meta_combobox->addItem(QString::fromStdString(i));
    }
}