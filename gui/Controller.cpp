#include "Controller.h"

void Controller::addModel() {
	try {
		VersionControl::Admin_var admin = m_root->getAdmin();
		QString name = QInputDialog::getText(m_mw, "New model", "Model name:");
		if (name.isEmpty()) return; // Cancel or empty name
		admin->addModel(name.toUtf8().constData());
	}
	catch (VersionControl::AccessDenied& e) {
		QMessageBox mb(QMessageBox::Critical, "Access Denied",
			"Only administrators can add models.",
			QMessageBox::Ok, m_mw);
		mb.exec();
	}
	catch (VersionControl::AlreadyExistsException& e) {
		QMessageBox mb(QMessageBox::Critical, "Duplicate model",
			"A model with the same name already exists.",
			QMessageBox::Ok, m_mw);
		mb.exec();
	}
	catch (VersionControl::DbError& e) {
		QMessageBox mb(QMessageBox::Critical, "Database error",
			"A database error occured on the server, please try again.",
			QMessageBox::Ok, m_mw);
		mb.exec();
	}
}

Controller::Controller(QWidget *mw, VersionControl::Root_var root) {
	m_mw = mw;
	m_root = root;
}
