#include "ui_main.h"
#include "ui_connect.h"
#include <VersionControl.hh>
#include <QMessageBox>

VersionControl::Root_ptr showConnectDialog(QWidget *mw, CORBA::ORB_var orb) {
	QDialog *cd = new QDialog(mw, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
	Ui::ConnectDialog ui;
	ui.setupUi(cd);
	while (true) {
		if (cd->exec() != QDialog::Accepted) exit(0);
		QString initref("corbaloc:iiop:");
		initref += ui.host->text() + ":" + ui.port->text() + "/AuthService";
		try {
			CORBA::Object_var obj = orb->string_to_object(initref.toUtf8().constData());
			VersionControl::Auth_var authref = VersionControl::Auth::_narrow(obj);
			return authref->login(
				ui.username->text().toUtf8().constData(),
				ui.password->text().toUtf8().constData());
		}
		catch (VersionControl::AccessDenied& e) {
			QMessageBox mb(QMessageBox::Critical, "Access Denied",
				"Check your username and password and try again.",
				QMessageBox::Ok, mw);
			mb.exec();
		}
		catch (VersionControl::DbError& e) {
			QMessageBox mb(QMessageBox::Critical, "Database error",
				"A database error occured on the server, please try again.",
				QMessageBox::Ok, mw);
			mb.exec();
		}
		catch(CORBA::TRANSIENT&) {
			QMessageBox mb(QMessageBox::Critical, "Connection error",
				"Unable to contact the server. Check the host/port and try again.",
				QMessageBox::Ok, mw);
			mb.exec();
		}
	}
}

void buildTree(Ui::MainWindow &ui, VersionControl::Root_ptr root) {
	QTreeWidget *tw = ui.treeWidget;
	tw->setColumnCount(1);
	QList<QTreeWidgetItem *> items;
	VersionControl::ModelSeq *models = root->getModels();
	for (unsigned int i = 0; i < models->length(); i++) {
		VersionControl::Model_var model = (*models)[i];
		QString name = QString::fromUtf8(model->getName());
		items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(name)));
	}
	tw->insertTopLevelItems(0, items);
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
	mw->show();
	VersionControl::Root_ptr root = showConnectDialog(mw, orb);
	VersionControl::User_ptr user = root->getMyUser();
	ui.statusbar->addWidget(new QLabel(user->getName()));
	ui.menuAdministration->menuAction()->setVisible(user->getAdmin());
	buildTree(ui, root);
	return app.exec();
}
