#include "ui_main.h"
#include "ui_connect.h"
#include <VersionControl.hh>
#include <QMessageBox>
#include <QSettings>
#include <QDesktopWidget>
#include "Controller.h"
#include "Exceptions.h"

VersionControl::Root_ptr showConnectDialog(QWidget *mw, CORBA::ORB_var orb) {
	QDialog *cd = new QDialog(mw, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
	Ui::ConnectDialog ui;
	ui.setupUi(cd);
	// Load settings
	QSettings settings("IRC Software Development", "SQL Version Control");
	ui.host->setText(settings.value("repository/host").toString());
	ui.port->setText(settings.value("repository/port").toString());
	ui.username->setText(settings.value("repository/username").toString());
	if (ui.username->text() != "") ui.password->setFocus(Qt::OtherFocusReason);
	while (true) {
		if (cd->exec() != QDialog::Accepted) exit(0);
		QString initref("corbaloc:iiop:");
		initref += ui.host->text() + ":" + ui.port->text() + "/AuthService";
		try {
			CORBA::Object_var obj = orb->string_to_object(initref.toUtf8().constData());
			VersionControl::Auth_var authref = VersionControl::Auth::_narrow(obj);
			VersionControl::Root_ptr retval = authref->login(
				ui.username->text().toUtf8().constData(),
				ui.password->text().toUtf8().constData());
			// Successful login, save settings
			settings.setValue("repository/host", ui.host->text());
			settings.setValue("repository/port", ui.port->text());
			settings.setValue("repository/username", ui.username->text());
			return retval;
		}
		catch(CORBA::TRANSIENT&) {
			QMessageBox mb(QMessageBox::Critical, "Connection error",
				"Unable to contact the server. Check the host/port and try again.",
				QMessageBox::Ok, mw);
			mb.exec();
			CATCH_ACCESSDENIED("Check your username and password and try again.")
			CATCH_DBERROR
		}
	}
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	const char* options[][2] = {
		{ "nativeCharCodeSet", "UTF-8" },
		{ 0, 0 }
	};
	CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "omniORB4", options);
	QMainWindow *mw = new QMainWindow;
	Ui::MainWindow ui;
	ui.setupUi(mw);
	QRect frect = mw->frameGeometry();
	frect.moveCenter(QDesktopWidget().availableGeometry().center());
	mw->move(frect.topLeft());
	mw->show();
	VersionControl::Root_var root = showConnectDialog(mw, orb);
	VersionControl::User_var user = root->getMyUser();
	ui.statusbar->addWidget(new QLabel(user->getName()));
	ui.menuAdministration->menuAction()->setVisible(user->getAdmin());
	QObject::connect(ui.actionAbout_QT, SIGNAL(activated()), &app, SLOT(aboutQt()));
	Controller ctrl(mw, &ui, root);
	QObject::connect(ui.actionNew_model, SIGNAL(activated()), &ctrl, SLOT(addModel()));
	QObject::connect(ui.actionRename_model, SIGNAL(activated()), &ctrl, SLOT(renameModel()));
	QObject::connect(ui.actionRemove_model, SIGNAL(activated()), &ctrl, SLOT(removeModel()));
	QObject::connect(ui.actionManage_users, SIGNAL(activated()), &ctrl, SLOT(manageUsers()));
	QObject::connect(ui.actionEdit_ACL, SIGNAL(activated()), &ctrl, SLOT(editACL()));
	QObject::connect(ui.actionCommit, SIGNAL(activated()), &ctrl, SLOT(commit()));
	QObject::connect(ui.actionCheckout, SIGNAL(activated()), &ctrl, SLOT(checkout()));
	ctrl.buildTree();
	return app.exec();
}
