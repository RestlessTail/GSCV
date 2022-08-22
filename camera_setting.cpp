#include <QTabWidget>
#include "camera_setting.h"
#include "ui_camera_setting.h"
#include "opengl_view_widget.h"
#include "global_variables.h"
#include "settings.h"

camera_setting::camera_setting(QWidget *parent) :
    right_panel_base(parent),
    ui(new Ui::camera_setting)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
	set_panel_title(tr("Camera settings"));
}
camera_setting::~camera_setting()
{
    delete ui;
}
void camera_setting::on_scale_slider_sliderReleased()
{
	float position = 1.0f * ui->scale_slider->value() / global_variables::slider_accuracy;
	ui->scale_edit->setText(QString::number(position));
	view_widget->set_scale(position, true);
}
void camera_setting::on_scale_slider_valueChanged(int value) {
	float position = 1.0f * value / global_variables::slider_accuracy;
	ui->scale_edit->setText(QString::number(position));
	bool is_update = settings_t::repaint_instantly_default;
	is_update = global_variables::settings->repaint_instantly;
	if(is_update){
        auto now = std::chrono::system_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - view_widget->last_update_time);
        if (time_elapsed.count() >= global_variables::settings->frame_delay)
        {
            view_widget->last_update_time = now;
			view_widget->set_scale(position, true);
			return;
        }
	}
	view_widget->set_scale(position, false);
}
void camera_setting::on_scale_edit_editingFinished()
{
	float scale = ui->scale_edit->text().toFloat();
	int slider_min = ui->scale_slider->minimum();
	int slider_max = ui->scale_slider->maximum();
	if(scale < slider_min){ ui->scale_slider->setValue(slider_min); }
	else if(scale > slider_max){ ui->scale_slider->setValue(slider_max); }
	else{ ui->scale_slider->setValue(static_cast<int>(scale)); }
	view_widget->set_scale(scale, true);
}
void camera_setting::on_size_slider_sliderReleased()
{
	float position = 1.0f * ui->size_slider->value() / global_variables::slider_accuracy;
	ui->size_edit->setText(QString::number(position));
	view_widget->set_point_size(position, true);
}
void camera_setting::on_size_slider_valueChanged(int value){
	float position = 1.0f * value / global_variables::slider_accuracy;
	ui->size_edit->setText(QString::number(position));
	bool is_update = settings_t::repaint_instantly_default;
	is_update = global_variables::settings->repaint_instantly;
	if(is_update){
        auto now = std::chrono::system_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - view_widget->last_update_time);
        if (time_elapsed.count() >= global_variables::settings->frame_delay)
        {
            view_widget->last_update_time = now;
			view_widget->set_point_size(position, true);
			return;
        }
	}
	view_widget->set_point_size(position, false);
}
void camera_setting::on_size_edit_editingFinished()
{
	float size = ui->size_edit->text().toFloat();
	int slider_min = ui->size_slider->minimum();
	int slider_max = ui->size_slider->maximum();
	if(size < slider_min){ ui->size_slider->setValue(slider_min); }
	else if(size > slider_max){ ui->size_slider->setValue(slider_max); }
	else{ ui->size_slider->setValue(static_cast<int>(size)); }
	view_widget->set_point_size(size, true);
}
void camera_setting::on_close_clicked()
{
	global_variables::panel_manager->destroy_panel(panel_type::camera_setting);
}
void camera_setting::set_view_widget(opengl_view_widget* new_view_widget){
	view_widget = new_view_widget;
	connect(view_widget, &opengl_view_widget::scale_changed, this, [&](){ update_scale(); });
	update_scale();
	update_size();
}
void camera_setting::update_scale(){
	int scale_max = static_cast<int>(view_widget->get_scale_lim_max()) * global_variables::slider_accuracy;
	int scale_min = static_cast<int>(view_widget->get_scale_lim_min()) * global_variables::slider_accuracy;
	float scale_val_raw = view_widget->get_scale();
	float scale_val = scale_val_raw * global_variables::slider_accuracy;
	ui->scale_slider->setMaximum(scale_max);
	ui->scale_slider->setMinimum(scale_min);
	if(scale_val > scale_max){ ui->scale_slider->setValue(scale_max); }
	else if(scale_val < scale_min){ ui->scale_slider->setValue(scale_min); }
	else{ ui->scale_slider->setValue(static_cast<int>(scale_val)); }
	ui->scale_edit->setText(QString::number(scale_val_raw));
}
void camera_setting::update_size(){
	int size_max = static_cast<int>(view_widget->get_size_lim_max()) * global_variables::slider_accuracy;
	int size_min = static_cast<int>(view_widget->get_size_lim_min()) * global_variables::slider_accuracy;
	float size_val_raw = view_widget->get_point_size();
	float size_val = size_val_raw * global_variables::slider_accuracy;
	ui->size_slider->setMaximum(size_max);
	ui->size_slider->setMinimum(size_min);
	if(size_val > size_max){ ui->size_slider->setValue(size_max); }
	else if(size_val < size_min){ ui->size_slider->setValue(size_min); }
	else{ ui->size_slider->setValue(static_cast<int>(size_val)); }
	ui->size_edit->setText(QString::number(size_val_raw));
}

