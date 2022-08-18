#ifndef SETTINGS
#define SETTINGS

#include <QString>

class QJsonObject;

class settings_t{
public:
	settings_t();
	int open(QString filename);
	void save(QString filename);
	int read_option(QJsonObject& obj, const char* key, bool& dest);
	int read_option(QJsonObject& obj, const char* key, int& dest);
	int read_option(QJsonObject& obj, const char* key, QString& dest);
public:
	//values that can be modified by users
	bool repaint_instantly;
	int viewport_max_frame_rate;
	QString language;
public:
	//default values
	static bool repaint_instantly_default;
	static int viewport_max_frame_rate_default;
	static QString language_default;
public:
	//values that cannot modified by users, but are essential to other class
	int frame_delay;
};

#endif