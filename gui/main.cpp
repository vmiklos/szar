#include "ui_main.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QMainWindow *mw = new QMainWindow;
	Ui::MainWindow ui;
	ui.setupUi(mw);

	mw->show();
	return app.exec();
}
