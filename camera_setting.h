#ifndef CAMERA_SETTING_H
#define CAMERA_SETTING_H
#include <QWidget>

class opengl_view_widget;
class QTabWidget;

namespace Ui {
class camera_setting;
}
class camera_setting : public QWidget
{
    Q_OBJECT
public:
    explicit camera_setting(QWidget *parent = nullptr);
    ~camera_setting();
	void set_to_this(camera_setting** new_to_this);
	void set_tab_widget_master(QTabWidget* new_tab_widget_master);
	void set_view_widget(opengl_view_widget* new_view_widget);
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
	opengl_view_widget* view_widget;
    Ui::camera_setting *ui;
	QTabWidget* tab_widget_master;
	camera_setting** to_this;
};
#endif // CAMERA_SETTING_H

