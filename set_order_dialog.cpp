#include "set_order_dialog.h"
#include "ui_set_order_dialog.h"

set_order_dialog::set_order_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::set_order_dialog)
{
    ui->setupUi(this);
}
set_order_dialog::~set_order_dialog() {
    delete ui;
}
void set_order_dialog::on_up_clicked() {
	int index = ui->preview->currentRow();
	int size = data.size();
	if(index <= 0){ return; }
    std::swap(data[index], data[index - 1]);
	ui->preview->setCurrentRow(index - 1);
	for(int i = 0; i < size; ++i){
        ui->preview->item(i)->setText(QString::fromStdString(data[i]));
	}
}
void set_order_dialog::on_down_clicked() {
	int index = ui->preview->currentRow();
	int size = data.size();
	if(index < 0 || index >= size - 1){ return; }
    std::swap(data[index], data[index + 1]);
	ui->preview->setCurrentRow(index + 1);
	for(int i = 0; i < size; ++i){
        ui->preview->item(i)->setText(QString::fromStdString(data[i]));
	}
}
void set_order_dialog::attach_data(std::vector<std::string>* new_data){
    data.clear();
    std::copy(new_data->begin(), new_data->end(), std::back_inserter(data));
    ui->preview->clear();
    int size = data.size();
    for(int i = 0; i < size; ++i){
        ui->preview->addItem(QString::fromStdString(data[i]));
    }
    raw_data = new_data;
}
void set_order_dialog::accept_data(){
    raw_data->clear();
    std::copy(data.begin(), data.end(), std::back_inserter(*raw_data));
}