#include "Controller.h"

void Controller::buildTree() {
	QTreeWidget *tw = m_ui->treeWidget;
	QList<QTreeWidgetItem *> items;
	VersionControl::ModelSeq *models = m_root->getModels();
	for (unsigned int i = 0; i < models->length(); i++) {
		VersionControl::Model_var model = (*models)[i];
		QString name = QString::fromUtf8(model->getName());
		QString numrevs;
		VersionControl::RevisionSeq *revs = model->getRevisions();
		unsigned int revnum = revs->length();
		numrevs.setNum(revnum);
		QStringList columns;
		columns << name << numrevs + " revision" + (numrevs == "1" ? "" : "s");
		QTreeWidgetItem *item = new QTreeWidgetItem(columns);
		for (unsigned int j = 0; j < revnum; j++) {
			VersionControl::Revision_var rev = (*revs)[j];
			QStringList revCols;
			QString rn;
			rn.setNum(rev->getNumber());
			revCols << QString("r") + rn <<
				QString::fromUtf8(rev->getTimestamp()).replace('T', " ") +
				" by " + QString::fromUtf8(rev->getAuthor()->getName());
			QTreeWidgetItem *revItem = new QTreeWidgetItem(revCols);
			item->insertChild(0, revItem);
		}
		items.append(item);
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
	while (twi->parent() != NULL) twi = twi->parent(); // get model
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
	while (twi->parent() != NULL) twi = twi->parent(); // get model
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
	while (twi->parent() != NULL) twi = twi->parent(); // get model
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

void Controller::commit() {
	QTreeWidgetItem *twi = m_ui->treeWidget->currentItem();
	if (twi == NULL) return;
	while (twi->parent() != NULL) twi = twi->parent(); // get model
	QString fn = QFileDialog::getOpenFileName(m_mw, "Select file to commit",
		"", "SQL scripts (*.sql);;All files (*.*)");
	if (fn == NULL) return; // Cancel
	QFile file(fn);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::critical(m_mw, "Cannot open file",
			"The selected file cannot be opened. Check permissions.");
		return;
	}
	QByteArray ba = file.readAll();
	if (ba.size() != file.size()) {
		QMessageBox::critical(m_mw, "Cannot read file",
			"The contents of the selected file cannot be read.");
		return;
	}
	try {
		const QString name = twi->text(0);
		m_root->getModel(name.toUtf8().constData())->commit(
			ba.constData(), twi->childCount());
		buildTree();
		QMessageBox::information(m_mw, "Successful commit",
			"The selected file has been committed to the repository.");
		CATCH_DBERROR
		CATCH_INVALIDMODEL
		CATCH_ACCESSDENIED("You don't have write permissions on the selected model.")
	}
}

void Controller::checkout() {
	QTreeWidgetItem *twi = m_ui->treeWidget->currentItem();
	if (twi == NULL) return;
	QTreeWidgetItem *parent = twi->parent();
	if (parent == NULL) {
		QMessageBox::critical(m_mw, "Invalid selection", "Please select a revision to check out.");
		return;
	}
	QString fn = QFileDialog::getSaveFileName(m_mw, "Select file name to check out to",
		parent->text(0) + "-" + twi->text(0) + ".sql", "SQL scripts (*.sql);;All files (*.*)");
	if (fn == NULL) return; // Cancel
	QFile file(fn);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
		QMessageBox::critical(m_mw, "Cannot open file",
			"The selected file cannot be opened. Check permissions.");
		return;
	}
	unsigned int revnum = atoi(twi->text(0).toUtf8().constData() + 1); // skip 'r'
	const QString name = parent->text(0);
	try {
		file.write(m_root->getModel(name.toUtf8().constData())->getRevision(revnum)->getData());
		CATCH_INVALIDMODEL
		CATCH_DBERROR
	}
}

void Controller::changePassword() {
	const QString pw = QInputDialog::getText(m_mw, "Password",
		"New password:", QLineEdit::Password);
	const QString pw2 = QInputDialog::getText(m_mw, "Password",
		"New password (again):", QLineEdit::Password);
	if (pw.isEmpty() || pw2.isEmpty()) return; // Cancel or empty password
	if (pw != pw2) {
		QMessageBox::critical(m_mw, "Password mismatch", "The passwords entered doesn't match!");
		return;
	}
	try {
		m_root->getMyUser()->setPassword(pw.toUtf8().constData());
		CATCH_DBERROR
	}
}

Controller::Controller(QWidget *mw, Ui::MainWindow *ui, VersionControl::Root_var root) {
	m_mw = mw;
	m_ui = ui;
	m_root = root;
}
