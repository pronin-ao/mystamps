#include <QApplication>
#include <QLabel>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	
	QLabel l("HELLo");
	l.setStyleSheet("background-color: red; color: blue");
	l.show();


	return a.exec();
}
