#include "ui_main.h"
#include "ui_connect.h"

QWidget *showMainWindow() {
	QMainWindow *mw = new QMainWindow;
	Ui::MainWindow ui;
	ui.setupUi(mw);
	mw->show();
	return mw;
}

void showConnectDialog(QWidget *mw) {
	QDialog *cd = new QDialog(mw, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
	Ui::ConnectDialog ui;
	ui.setupUi(cd);
	if (cd->exec() != QDialog::Accepted) exit(0);
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QWidget *mw = showMainWindow();
	showConnectDialog(mw);
	return app.exec();
}
