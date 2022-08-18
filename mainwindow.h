#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class opengl_view_widget;
class statistic;
class QProgressDialog;
class cell_color;
class color_scheme;
class camera_setting;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    opengl_view_widget* view_widget;
    QProgressDialog* progress_dlg;
    statistic* statistic_panel;
	cell_color* cell_color_panel;
    color_scheme* color_scheme_panel;
	camera_setting* camera_setting_panel;
    void destroy_progress_dlg();
private slots:
    void on_actionOpen_triggered();
    void on_actionReport_bug_triggered();
	//void send_data_to_opengl_view(sc_data* data);
    void on_zoom_in_clicked();
    void on_zoom_out_clicked();
    void on_actionCell_color_triggered();
    void on_actionColor_scheme_triggered();
    void on_actionCamera_settings_triggered();
    void on_actionImport_triggered();
    void on_actionSave_as_File_triggered();
    void on_actionCopy_to_clipboard_triggered();
    void on_save_viewport_clicked();
    void on_copy_viewport_clicked();
    void on_actionSettings_triggered();
    void on_actionView_help_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
