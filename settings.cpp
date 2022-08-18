#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include "settings.h"
#include "global_variables.h"
#include "language_manager.h"

bool settings_t::repaint_instantly_default = false;
int settings_t::viewport_max_frame_rate_default = 30;
QString settings_t::language_default = "en_US";

settings_t::settings_t(){
	//default settings
	repaint_instantly = settings_t::repaint_instantly_default;
	viewport_max_frame_rate = settings_t::viewport_max_frame_rate_default;
	language = settings_t::language_default;
}

int settings_t::read_option(QJsonObject& obj, const char* key, bool& dest){
	QString _key(key);
	if(obj.contains(_key)){
		QJsonValue val = obj.value(_key);
		if(val.isBool()){
			dest = val.toBool();
		}
		else{ return -1; }
	}
	else{ return -1; }
	return 0;
}
int settings_t::read_option(QJsonObject& obj, const char* key, int& dest){
	QString _key(key);
	if(obj.contains(_key)){
		QJsonValue val = obj.value(_key);
		if(val.isDouble()){
			dest = static_cast<int>(val.toDouble());
		}
		else{ return -1; }
	}
	else{ return -1; }
	return 0;
}
int settings_t::read_option(QJsonObject& obj, const char* key, QString& dest){
	QString _key(key);
	if(obj.contains(_key)){
		QJsonValue val = obj.value(_key);
		if(val.isString()){
			dest = val.toString();
		}
		else{ return -1; }
	}
	else{ return -1; }
	return 0;
}
int settings_t::open(QString filename){
	//read json
	QFile f(filename);
	if(!f.exists()){
		save(filename);
		return -2;
	}
	f.open(QIODevice::ReadOnly | QIODevice::Text);
	QString json = f.readAll();
	f.close();

	//parse json
	QJsonParseError err;
	QJsonDocument doc = QJsonDocument::fromJson(json.toLocal8Bit());
	if(err.error != QJsonParseError::NoError){ return -1; }
	QJsonObject root = doc.object();

	//read settings
	read_option(root, "repaint_instantly", repaint_instantly);
	read_option(root, "viewport_max_frame_rate", viewport_max_frame_rate);
	frame_delay = 1000.0f / viewport_max_frame_rate;
	read_option(root, "language", language);

	return 0;
}
void settings_t::save(QString filename){
	//generate root object
	QJsonObject root;

	//insert settings
	root.insert("repaint_instantly", repaint_instantly);
	root.insert("viewport_max_frame_rate", viewport_max_frame_rate);
	root.insert("language", language);

	//generate json
	QJsonDocument doc;
	doc.setObject(root);
	QString json = doc.toJson();

	//save json
	QFile f(filename);
	f.open(QIODevice::WriteOnly | QIODevice::Text);
	f.write(json.toLocal8Bit());
	f.close();
}