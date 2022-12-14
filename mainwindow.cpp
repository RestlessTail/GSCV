#include <QClipboard>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QDir>
#include <QDesktopServices>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "camera_setting.h"
#include "cell_color.h"
#include "color_scheme.h"
#include "global_variables.h"
#include "gscv_import_wizard.h"
#include "opengl_view_widget.h"
#include "sc_reader.h"
#include "settings_dialog.h"
#include "statistic.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->zoom_in->setIcon(QIcon(global_variables::icon_zoom_in));
    ui->zoom_out->setIcon(QIcon(global_variables::icon_zoom_out));
    ui->save_viewport->setIcon(QIcon(global_variables::icon_save_viewport));
    ui->copy_viewport->setIcon(QIcon(global_variables::icon_copy_viewport));
    ui->right_splitter->setSizes({3,1});
    view_widget = new opengl_view_widget(this);
    statistic_panel = new statistic;
    statistic_panel->set_view_widget(view_widget);
	cell_color_panel = nullptr;
    color_scheme_panel = nullptr;
	camera_setting_panel = nullptr;
    ui->opengl_view_layout->addWidget(view_widget);
    ui->right_panels->addTab(statistic_panel, tr("Statistic"));
    progress_dlg = nullptr;
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::destroy_progress_dlg()
{
    delete progress_dlg;
    progress_dlg = nullptr;
}
void MainWindow::on_actionOpen_triggered()
{
	//select an archive by using a dialogue
    QString filename = QFileDialog::getOpenFileName(this,
			QString(tr("Select a data set")),
			//QDir::homePath(),
			QString(),
			QString(tr("GSCV Archive (*.gscvArchive)")));
    if(filename.isEmpty()){ return; }
    
	//initiate a progress bar
	//progress_dlg = new QProgressDialog;
    //progress_dlg->setWindowTitle("Reading data");
    //progress_dlg->setLabelText("This can take several minutes.");
    //progress_dlg->setRange(0, 0);
    //progress_dlg->setAutoReset(true);
    //progress_dlg->setModal(true);
    //progress_dlg->show();

	//read data by sc_reader (sync)
    //connect(reader, &sc_reader::task_done, this, &MainWindow::destroy_progress_dlg);
    //connect(reader, &sc_reader::send_data, this, &MainWindow::send_data_to_opengl_view);
    //connect(reader, &sc_reader::finished, reader, &QObject::deleteLater);
    view_widget->open(filename.toStdString());
    view_widget->clear_color();
    statistic_panel->update_statistic();
    if (cell_color_panel) {
        cell_color_panel->update_data();
    }
}
void MainWindow::on_actionReport_bug_triggered() {
    QMessageBox::information(this, tr("Bug report"), tr("For bug reporting, please email your complaints to Shi Chengge<1826930551@qq.com>"));
}
void MainWindow::on_zoom_in_clicked() {
	float new_scale = view_widget->get_scale() + 1.0f;
	if(new_scale > view_widget->get_scale_lim_max()){ new_scale = view_widget->get_scale_lim_max(); }
	if(new_scale < view_widget->get_scale_lim_min()){ new_scale = view_widget->get_scale_lim_min(); }
	view_widget->set_scale(new_scale, true);
}
void MainWindow::on_zoom_out_clicked() {
	float new_scale = view_widget->get_scale() - 1.0f;
	if(new_scale > view_widget->get_scale_lim_max()){ new_scale = view_widget->get_scale_lim_max(); }
	if(new_scale < view_widget->get_scale_lim_min()){ new_scale = view_widget->get_scale_lim_min(); }
    view_widget->set_scale(new_scale, true);
}
void MainWindow::on_actionCell_color_triggered()
{
	if(cell_color_panel){
        ui->right_panels->setCurrentIndex(ui->right_panels->indexOf(cell_color_panel));
	}
	else{
        cell_color_panel = new cell_color;
		cell_color_panel->set_view_widget(view_widget);
        cell_color_panel->set_tab_widget_master(ui->right_panels);
        cell_color_panel->set_to_this(&cell_color_panel);
        ui->right_panels->addTab(cell_color_panel, tr("Cell color"));
        ui->right_panels->setCurrentIndex(ui->right_panels->indexOf(cell_color_panel));
	}
}
void MainWindow::on_actionColor_scheme_triggered()
{
    if(color_scheme_panel){
        ui->right_panels->setCurrentIndex(ui->right_panels->indexOf(color_scheme_panel));
    }
    else{
        color_scheme_panel = new color_scheme;
        color_scheme_panel->set_palette(view_widget->get_palette());
        color_scheme_panel->set_tab_widget_master(ui->right_panels);
        color_scheme_panel->set_to_this(&color_scheme_panel);
        ui->right_panels->addTab(color_scheme_panel, tr("Color scheme"));
        ui->right_panels->setCurrentIndex(ui->right_panels->indexOf(color_scheme_panel));
    }
}
void MainWindow::on_actionCamera_settings_triggered()
{
	if(camera_setting_panel){
		ui->right_panels->setCurrentIndex(ui->right_panels->indexOf(camera_setting_panel));
	}
	else{
		camera_setting_panel = new camera_setting;
		camera_setting_panel->set_view_widget(view_widget);
		camera_setting_panel->set_tab_widget_master(ui->right_panels);
		camera_setting_panel->set_to_this(&camera_setting_panel);
		ui->right_panels->addTab(camera_setting_panel, tr("Camera settings"));
		ui->right_panels->setCurrentIndex(ui->right_panels->indexOf(camera_setting_panel));
	}
}

void MainWindow::on_actionImport_triggered() {
	GSCV_import_wizard wizard;
	wizard.exec();
}
void MainWindow::on_actionSave_as_File_triggered() {
    QString caption = tr("Save viewport");
    QString filter = tr("JPEG (*.jpg);;BMP (*.bmp);;PNG (*.png)");
    QString filename;
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
    filename = QFileDialog::getSaveFileName(this, caption, QString(), filter);
    if(filename.isEmpty()){
        return;
    }
#else
    //QFileDialog::getSaveFileName doesn't add extension to a filename without extension on Linux
    //It is required to check extension manually
    QFileDialog dlg(this, caption);
    dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setNameFilter(filter);
    if(dlg.exec()){
        QStringList files = dlg.selectedFiles();
        if(files.size() < 1){
            return;
        }
        filename = files[0];
    }
    //check extension
    if(filename.isEmpty()){
        return;
    }
    QString selected_filter = dlg.selectedNameFilter();
    int extension_begin = selected_filter.indexOf('*') + 1;
    int extension_size = selected_filter.size() - extension_begin - 1;
    selected_filter = selected_filter.mid(extension_begin, extension_size);
    if(!filename.endsWith(selected_filter)){
        filename = filename + selected_filter;
    }
#endif
    QImage viewport = view_widget->grabFramebuffer();
    viewport.save(filename);
}
void MainWindow::on_actionCopy_to_clipboard_triggered() {
    QImage viewport = view_widget->grabFramebuffer();
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setImage(viewport);
}
void MainWindow::on_save_viewport_clicked() {
    on_actionSave_as_File_triggered();
}
void MainWindow::on_copy_viewport_clicked() {
    on_actionCopy_to_clipboard_triggered();
}
void MainWindow::on_actionSettings_triggered() {
    settings_dialog dlg;
    dlg.load_settings();
    if(dlg.exec() == QDialog::Accepted){
        dlg.get_settings();
        global_variables::settings->save(global_variables::config_filename);
    }
}
void MainWindow::on_actionView_help_triggered(){
    QString url = QApplication::applicationDirPath() + "/doc/" + global_variables::settings->language + "/0introduction.html";
    if(!QDesktopServices::openUrl(url)){
        QMessageBox::critical(this, tr("Error"), tr("Failed to open documents in browser. You can find documents in directory \"doc\" in the installation directory."));
    }
}