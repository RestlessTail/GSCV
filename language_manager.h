#ifndef LANGUAGE_MANAGER
#define LANGUAGE_MANAGER

#include <QVector>
#include <QString>
#include <filesystem>

class QTranslator;
class QApplication;

class language_manager{
public:
	language_manager(QApplication* a);
	~language_manager();
	void scan(std::filesystem::path directory);
	QVector<QString>& get_languages();
	void update_language(QString& name);
private:
	QVector<QString> languages;
	QTranslator* translator;
	QApplication* app;
};

#endif