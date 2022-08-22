#ifndef CAMERA_SETTING_H
#define CAMERA_SETTING_H

#include "right_panel_manager.h"

class opengl_view_widget;
class QTabWidget;

namespace Ui {
class camera_setting;
}
class camera_setting : public right_panel_base
{
    Q_OBJECT
public:
    explicit camera_setting(QWidget *parent = nullptr);
    ~camera_setting();
    void set_view_widget(opengl_view_widget *new_view_widget) override;
private slots:
    void on_scale_slider_sliderReleased();
    void on_scale_edit_editingFinished();
    void on_size_slider_sliderReleased();
    void on_size_edit_editingFinished();
    void on_close_clicked();
    void on_scale_slider_valueChanged(int value);
    void on_size_slider_valueChanged(int value);

private:
	void update_scale();
	void update_size();
    Ui::camera_setting *ui;
};
#endif // CAMERA_SETTING_H

