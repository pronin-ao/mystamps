#include <QApplication>
#include <QLabel>
#include <QMessageBox>

#include "datamanager.h"

int main(int argc, char* argv[]) {

	QApplication a(argc, argv);

	try {
	  DataManager db{};
	  QLabel l("mystamp-22:: "+ QString::number(db.size()));
	  l.setStyleSheet("background-color: red; color: blue");
	  l.show();
	  return a.exec();
	} catch(const std::exception& ex){
	  QMessageBox::critical(nullptr, "Error loading DB",
	      QString::fromStdString(ex.what()));
	  return 0;
	}
}
