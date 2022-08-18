#include <QMessageBox>
#include <string>
#include <vector>
#include "cell_color.h"
#include "ui_cell_color.h"
#include "utils.h"
#include "opengl_view_widget.h"

cell_color::cell_color(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cell_color)
{
    ui->setupUi(this);
    ui->radio_continous->setChecked(true);
    ui->radio_categorical->setChecked(false);
    setAttribute(Qt::WA_DeleteOnClose, true);
}
cell_color::~cell_color()
{
    delete ui;
}
void cell_color::on_search_gene_go_clicked()
{
    std::string gene_name = ui->gene_symbol->text().toStdString();
    if(gene_name.empty()){
        return;
    }
    if(view_widget->set_gene(gene_name)){
        QString q_gene_name = QString::fromStdString(gene_name);
        QMessageBox::information(this, QString(tr("Information")), QString(tr("Failed to find ")) + q_gene_name + QString(tr(".")));
    }
}
void cell_color::on_meta_go_clicked()
{
    std::string meta_name = ui->meta_combobox->currentText().toStdString();
    bool continuous = ui->radio_continous->isChecked();
    if(meta_name.empty()){
        return;
    }
    if(view_widget->set_meta(meta_name, continuous)){
        QMessageBox::critical(this, QString(tr("Critical")), QString(tr("Internal error.")));
    }
}
void cell_color::on_clear_color_clicked()
{
    view_widget->clear_color();
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
    tab_widget_master->removeTab(tab_widget_master->indexOf(this));
    *to_this = nullptr;
    deleteLater();
}
void cell_color::set_to_this(cell_color **newTo_this){
    to_this = newTo_this;
}
void cell_color::update_data()
{
    ui->meta_combobox->clear();
    std::vector<std::string>& meta_dict = view_widget->get_meta();
    for (auto& i : meta_dict) {
        ui->meta_combobox->addItem(QString::fromStdString(i));
    }
}
void cell_color::set_tab_widget_master(QTabWidget *newTab_widget_master){
    tab_widget_master = newTab_widget_master;
}

