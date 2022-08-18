#include <QApplication>
#include "global_variables.h"

int global_variables::GSCV_ARCHIVE_MAGIC = 0x6215;
int global_variables::slider_accuracy = 10;
size_t global_variables::zlib_chunk_size = 16384;
QString global_variables::config_filename = QString();
QString global_variables::icon_zoom_in = QString();
QString global_variables::icon_zoom_out = QString();
QString global_variables::icon_copy_viewport = QString();
QString global_variables::icon_save_viewport = QString();
QString global_variables::default_shader_vert = QString();
QString global_variables::default_shader_frag = QString();
QString global_variables::lang_dir = QString();
language_manager* global_variables::lang_manager = nullptr;
settings_t* global_variables::settings = nullptr;