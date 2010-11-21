#include "Controller.h"

void Controller::buildTree() {
	QTreeWidget *tw = m_ui->treeWidget;
	QList<QTreeWidgetItem *> items;
	VersionControl::ModelSeq *models = m_root->getModels();
	for (unsigned int i = 0; i < models->length(); i++) {
		VersionControl::Model_var model = (*models)[i];
		QString name = QString::fromUtf8(model->getName());
		QString numrevs;
		numrevs.setNum(model->getRevisions()->length());
		QStringList columns;
		columns << name << numrevs + " revision(s)";
		items.append(new QTreeWidgetItem((QTreeWidget*)0, columns));
	}
	tw->clear();
	tw->insertTopLevelItems(0, items);
}

void Controller::addModel() {
	try {
		VersionControl::Admin_var admin = m_root->getAdmin();
		QString name = QInputDialog::getText(m_mw, "New model", "Model name:");
		if (name.isEmpty()) return; // Cancel or empty name
		admin->addModel(name.toUtf8().constData());
		buildTree();
		CATCH_DBERROR
		CATCH_ACCESSDENIED("Only administrators can add models.")
		CATCH_ALREADYEXISTS("model")
	}
}

void Controller::renameModel() {
	QTreeWidgetItem *twi = m_ui->treeWidget->currentItem();
	if (twi == NULL) return;
	try {
		const QString oldName = twi->text(0);
		VersionControl::ModelAdmin_var admin =
			m_root->getAdmin()->getModelAdmin(m_root->getModel(oldName.toUtf8().constData()));
		const QString newName = QInputDialog::getText(
			m_mw, "New model", "Model name:", QLineEdit::Normal, oldName);
		if (newName.isEmpty() || newName == oldName) return; // Cancel or empty or old name
		admin->setName(newName.toUtf8().constData());
		buildTree();
		CATCH_DBERROR
		CATCH_INVALIDMODEL
		CATCH_ACCESSDENIED("Only administrators can rename models.")
		CATCH_ALREADYEXISTS("model")
	}
}

void Controller::removeModel() {
	QTreeWidgetItem *twi = m_ui->treeWidget->currentItem();
	if (twi == NULL) return;
	try {
		const QString oldName = twi->text(0);
		VersionControl::ModelAdmin_var admin =
			m_root->getAdmin()->getModelAdmin(m_root->getModel(oldName.toUtf8().constData()));
		QMessageBox mb(QMessageBox::Question, "Model removal",
			"Do you want to remove the selected model with its every revision? There is no undo.",
			QMessageBox::Yes | QMessageBox::No, m_mw);
		if (mb.exec() != QMessageBox::Yes) return;
		admin->removeModel();
		buildTree();
		CATCH_DBERROR
		CATCH_INVALIDMODEL
		CATCH_ACCESSDENIED("Only administrators can remove models.")
	}
}

void Controller::manageUsers() {
	try {
		VersionControl::Admin_var admin = m_root->getAdmin();
		QDialog *ud = new QDialog(m_mw, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
		Ui::UsersDialog ui;
		ui.setupUi(ud);
		Users ctrl(ud, &ui, admin);
		QObject::connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)),
			&ctrl, SLOT(clicked(QAbstractButton*)));
		QObject::connect(ui.listWidget, SIGNAL(itemChanged(QListWidgetItem*)),
			&ctrl, SLOT(itemChanged(QListWidgetItem*)));
		ud->exec();
		CATCH_DBERROR
		CATCH_ACCESSDENIED("Only administrators can manage users.")
	}
}

void Controller::editACL() {
	QTreeWidgetItem *twi = m_ui->treeWidget->currentItem();
	if (twi == NULL) return;
	try {
		const QString name = twi->text(0);
		VersionControl::Admin_var admin = m_root->getAdmin();
		VersionControl::ModelAdmin_var model =
			admin->getModelAdmin(m_root->getModel(name.toUtf8().constData()));
		QDialog *ud = new QDialog(m_mw, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
		Ui::UsersDialog ui;
		ui.setupUi(ud);
		ACL ctrl(ud, &ui, model, admin);
		QObject::connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)),
			&ctrl, SLOT(clicked(QAbstractButton*)));
		QObject::connect(ui.listWidget, SIGNAL(itemChanged(QListWidgetItem*)),
			&ctrl, SLOT(itemChanged(QListWidgetItem*)));
		ud->exec();
		CATCH_DBERROR
		CATCH_INVALIDMODEL
		CATCH_ACCESSDENIED("Only administrators can edit ACL.")
	}
}

Controller::Controller(QWidget *mw, Ui::MainWindow *ui, VersionControl::Root_var root) {
	m_mw = mw;
	m_ui = ui;
	m_root = root;
}
