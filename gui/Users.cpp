#include "Users.h"

void Users::buildList() {
	QListWidget *lw = m_ui->listWidget;
	lw->clear();
	m_hash.clear();
	VersionControl::UserAdminSeq *users = m_admin->getUsers();
	for (unsigned int i = 0; i < users->length(); i++) {
		VersionControl::UserAdmin_var user = (*users)[i];
		QString name = QString::fromUtf8(user->getName());
		QListWidgetItem *lwi = new QListWidgetItem(name);
		lwi->setCheckState(user->getAdmin() ? Qt::Checked : Qt::Unchecked);
		lw->addItem(lwi);
		m_hash[lwi] = user;
	}
}

void Users::itemChanged(QListWidgetItem *item) {
	if (item == NULL) return;
	const QString name = item->text();
	try {
		m_hash[item]->setAdmin(item->checkState() == Qt::Checked);
		CATCH_DBERROR
		CATCH_ACCESSDENIED("Only administrators can manage users.")
	}
	buildList();
}

void Users::clicked(QAbstractButton *button) {
	if (button == m_add) {
		try {
			QString name = QInputDialog::getText(m_d, "New user", "User name:");
			if (name.isEmpty()) return; // Cancel or empty name
			m_admin->addUser(name.toUtf8().constData());
			buildList();
			CATCH_DBERROR
			CATCH_ACCESSDENIED("Only administrators can add users.")
			CATCH_ALREADYEXISTS("user")
		}
	} else if (button == m_remove) {
		QListWidgetItem *lwi = m_ui->listWidget->currentItem();
		if (lwi == NULL) return;
		QMessageBox mb(QMessageBox::Question, "User removal",
			"Do you want to remove the selected user with its every revision? There is no undo.",
			QMessageBox::Yes | QMessageBox::No, m_d);
		if (mb.exec() != QMessageBox::Yes) return;
		try {
			m_admin->removeUser(m_hash[lwi]);
			buildList();
			CATCH_DBERROR
			CATCH_ACCESSDENIED("Only administrators can remove users.")
		}
	} else if (button == m_pw) {
		QListWidgetItem *lwi = m_ui->listWidget->currentItem();
		if (lwi == NULL) return;
		const QString pw = QInputDialog::getText(m_d, "Password",
			"New password:", QLineEdit::Password);
		const QString pw2 = QInputDialog::getText(m_d, "Password",
			"New password (again):", QLineEdit::Password);
		if (pw.isEmpty() || pw2.isEmpty()) return; // Cancel or empty password
		if (pw != pw2) {
			QMessageBox::critical(m_d, "Password mismatch", "The passwords entered doesn't match!");
			return;
		}
		try {
			m_hash[lwi]->setPassword(pw.toUtf8().constData());
			CATCH_DBERROR
		}
	}
}

void Users::addButtons() {
	QDialogButtonBox *bb = m_ui->buttonBox;
	m_add = (QAbstractButton*)bb->addButton("Add", QDialogButtonBox::ActionRole);
	m_remove = (QAbstractButton*)bb->addButton("Remove", QDialogButtonBox::ActionRole);
	m_pw = (QAbstractButton*)bb->addButton("Set password", QDialogButtonBox::ActionRole);
}

Users::Users(QDialog *d, Ui::UsersDialog *ui, VersionControl::Admin_var admin) {
	m_d = d;
	m_ui = ui;
	m_admin = admin;
	addButtons();
	buildList();
}
