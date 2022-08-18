#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "mainwindow.h"
#include "language_manager.h"
#include "global_variables.h"
#include "settings.h"

void init_path();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    init_path();

    language_manager lang_manager(&a);
    lang_manager.scan(global_variables::lang_dir.toStdString());
    global_variables::lang_manager = &lang_manager;

    settings_t settings;
    settings.open(global_variables::config_filename);
    global_variables::settings = &settings;

    if(!global_variables::settings->language.isEmpty()){
		global_variables::lang_manager->update_language(global_variables::settings->language);
    }

    MainWindow w;
    w.show();

	return a.exec();
}

void init_path(){
    QString app_dir = QApplication::applicationDirPath();
    global_variables::default_shader_vert = app_dir + "/shaders/default/default.vert";
    global_variables::default_shader_frag = app_dir + "/shaders/default/default.frag";
    global_variables::config_filename = app_dir + "/config.json";
    global_variables::icon_zoom_in = app_dir + "/icon/zoom_in.ico";
    global_variables::icon_zoom_out = app_dir + "/icon/zoom_out.ico";
    global_variables::icon_copy_viewport = app_dir + "/icon/copy_viewport.ico";
    global_variables::icon_save_viewport = app_dir + "/icon/save_viewport.ico";
    global_variables::lang_dir = app_dir + "/language/";
}