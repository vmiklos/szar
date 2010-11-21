#include "ACL.h"

void ACL::buildList() {
	QListWidget *lw = m_ui->listWidget;
	lw->clear();
	m_hash.clear();
	VersionControl::UserAccessSeq *users = m_model->getUsers();
	for (unsigned int i = 0; i < users->length(); i++) {
		VersionControl::UserAccess user = (*users)[i];
		QString name = QString::fromUtf8(user.grantee->getName());
		QListWidgetItem *lwi = new QListWidgetItem(name);
		lwi->setCheckState(user.level == VersionControl::ReadWrite ? Qt::Checked : Qt::Unchecked);
		lw->addItem(lwi);
		m_hash[lwi] = user;
	}
}

void ACL::itemChanged(QListWidgetItem *item) {
	if (item == NULL) return;
	const QString name = item->text();
	try {
		m_model->changeUserLevel(m_hash[item].grantee, item->checkState() ==
			Qt::Checked ? VersionControl::ReadWrite : VersionControl::Read);
		CATCH_DBERROR
		CATCH_INVALIDUSER
		CATCH_INVALIDMODEL
	}
	buildList();
}

void ACL::clicked(QAbstractButton *button) {
	if (button == m_add) {
		try {
			QHash<QString, VersionControl::User_var> hash;
			QStringList userList;
			VersionControl::UserAdminSeq *users = m_admin->getUsers();
			for (unsigned int i = 0; i < users->length(); i++) {
				VersionControl::User_var user = (VersionControl::User_var)(*users)[i];
				QString name = QString::fromUtf8(user->getName());
				hash[name] = user;
				userList << name;
			}
			QString name = QInputDialog::getItem(m_d, "Assign user", "Select user:",
				userList, 0, false);
			if (name.isEmpty()) return; // Cancel
			VersionControl::UserAccess ua;
			ua.grantee = hash[name];
			ua.level = VersionControl::Read;
			m_model->addUser(ua);
			buildList();
			CATCH_DBERROR
			CATCH_INVALIDUSER
			CATCH_INVALIDMODEL
			CATCH_ALREADYASSIGNED
			CATCH_ACCESSDENIED("Only administrators can assign users to a model.")
		}
	} else if (button == m_remove) {
		QListWidgetItem *lwi = m_ui->listWidget->currentItem();
		if (lwi == NULL) return;
		try {
			m_model->removeUser(m_hash[lwi].grantee);
			buildList();
			CATCH_DBERROR
			CATCH_INVALIDUSER
			CATCH_INVALIDMODEL
		}
	}
}

void ACL::addButtons() {
	QDialogButtonBox *bb = m_ui->buttonBox;
	m_add = (QAbstractButton*)bb->addButton("Add", QDialogButtonBox::ActionRole);
	m_remove = (QAbstractButton*)bb->addButton("Remove", QDialogButtonBox::ActionRole);
}

ACL::ACL(QDialog *d, Ui::UsersDialog *ui, VersionControl::ModelAdmin_var model, VersionControl::Admin_var admin) {
	m_d = d;
	m_ui = ui;
	m_admin = admin;
	m_model = model;
	m_ui->label->setText("Users with write permission have the box in front of their names checked.");
	addButtons();
	buildList();
}
