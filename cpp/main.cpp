#include <QApplication>
#include <QQmlApplicationEngine>
#include <QMessageBox>

#include "datamanager.h"
#include "catalogue.h"

int main(int argc, char* argv[]) {

	QApplication a(argc, argv);

	try {
	  DataManager db{};
	  QQmlApplicationEngine engine(":/main.qml");
	  qDebug() << "qml app started";
	  const auto& roots = engine.rootObjects();
	  if(roots.empty() || roots.at(0)->children().empty())
	    throw std::runtime_error("Empty QML root object");
	  Catalogue* catalogue =
	      roots.at(0)->children().at(0)->findChild<Catalogue*>();
	  if(catalogue == nullptr)
	    throw std::runtime_error("Null qml catalogue pointer");
	  qDebug() << "about to set";
	  db.setCatalogue(catalogue);

	  return a.exec();
	} catch(const std::exception& ex){
	  QMessageBox::critical(nullptr, "Error loading DB",
	      QString::fromStdString(ex.what()));
	  return 0;
	}
}
