#include <QTranslator>
#include <QApplication>
#include "language_manager.h"
#include "settings.h"
#include "global_variables.h"

language_manager::language_manager(QApplication* a){
	translator = nullptr;
	app = a;
}
language_manager::~language_manager(){
	delete translator;
	translator = nullptr;
}
void language_manager::scan(std::filesystem::path directory){
	languages.clear();
	if(!std::filesystem::exists(directory)){
		return;
	}
	for(auto& i : std::filesystem::directory_iterator(directory)){
		std::string filename = i.path().string();
		auto pos = filename.rfind(".qm");
		if(pos == std::string::npos){
			continue;
		}
		else if(pos + 3 == filename.size()){
			auto slash_pos = filename.rfind('/');
			auto extension_pos = filename.rfind('.');
			filename = filename.substr(slash_pos + 1, extension_pos - slash_pos - 1);
			languages.push_back(QString::fromStdString(filename));
		}
	}
}
QVector<QString>& language_manager::get_languages(){
	return languages;
}
void language_manager::update_language(QString& name){
	if(translator){
		app->removeTranslator(translator);
		delete translator;
		translator = nullptr;
	}
	translator = new QTranslator(app);
	QString filename = QApplication::applicationDirPath() + "/language/" + name + ".qm";
	if(translator->load(filename)){
		app->installTranslator(translator);
	}
}