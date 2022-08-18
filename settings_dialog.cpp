#include <QMessageBox>
#include "settings_dialog.h"
#include "ui_settings_dialog.h"
#include "settings.h"
#include "global_variables.h"
#include "language_manager.h"

settings_dialog::settings_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings_dialog)
{
    ui->setupUi(this);
}

settings_dialog::~settings_dialog()
{
    delete ui;
}
void settings_dialog::load_settings(){
    ui->repaint_instantly->setChecked(global_variables::settings->repaint_instantly);
    ui->viewport_max_frame_rate->setText(QString::number(global_variables::settings->viewport_max_frame_rate));
    QVector<QString>& languages = global_variables::lang_manager->get_languages();
    for(auto& i : languages){
        ui->language->addItem(i);
    }
    ui->language->setCurrentText(global_variables::settings->language);
}
void settings_dialog::get_settings(){
    global_variables::settings->repaint_instantly = ui->repaint_instantly->isChecked();
    global_variables::settings->viewport_max_frame_rate = ui->viewport_max_frame_rate->text().toInt();
    global_variables::settings->frame_delay = static_cast<int>(1000.0f / global_variables::settings->viewport_max_frame_rate);
    if(global_variables::settings->language != ui->language->currentText()){
        QMessageBox::information(this, tr("Information"), tr("Preferred language is changed. Please restart GSCV to apply the new language."));
        global_variables::settings->language = ui->language->currentText();
    }
    
    global_variables::lang_manager->update_language(global_variables::settings->language);
}