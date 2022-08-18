#ifndef GLOBAL_VARIABLES
#define GLOBAL_VARIABLES

#include <QString>

class language_manager;
class settings_t;

class global_variables{
public:
	static int GSCV_ARCHIVE_MAGIC;
	static int slider_accuracy;
	static size_t zlib_chunk_size;
	static QString config_filename;
	static QString icon_zoom_in;
	static QString icon_zoom_out;
	static QString icon_copy_viewport;
	static QString icon_save_viewport;
	static QString default_shader_vert;
	static QString default_shader_frag;
	static QString lang_dir;
	static language_manager* lang_manager;
	static settings_t* settings;
};

#endif