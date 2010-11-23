#include "Controller.h"

void Controller::buildTree() {
	QTreeWidget *tw = m_ui->treeWidget;
	QList<QTreeWidgetItem *> items;
	VersionControl::ModelSeq *models = m_root->getModels();
	for (unsigned int i = 0; i < models->length(); i++) {
		VersionControl::Model_var model = (*models)[i];
		QString name = QString::fromUtf8(model->getName());
		VersionControl::RevisionSeq *revs = model->getRevisions();
		unsigned int revnum = revs->length();
		QStringList columns;
		columns << name << QString::number(revnum) + " revision" + (revnum == 1 ? "" : "s");
		QTreeWidgetItem *item = new QTreeWidgetItem(columns);
		for (unsigned int j = 0; j < revnum; j++) {
			VersionControl::Revision_var rev = (*revs)[j];
			QStringList revCols;
			revCols << QString("r") + QString::number(rev->getNumber()) <<
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
	const QString fn = QFileDialog::getOpenFileName(m_mw, "Select file to commit",
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
	file.close();
	const QString name = twi->text(0);
	try {
		VersionControl::Model_var model = m_root->getModel(name.toUtf8().constData());
		commitContent(ba.constData(), getBase(model, fn), model);
		buildTree();
		QMessageBox::information(m_mw, "Successful commit",
			"The selected file has been committed to the repository.");
		CATCH_LOCKED
		CATCH_DBERROR
		CATCH_INVALIDMODEL
		CATCH_ACCESSDENIED("You don't have write permissions on the selected model.")
	} catch (CommitCancelled&) {}
}

unsigned int Controller::getBase(VersionControl::Model_var model, const QString fileName) {
	QDialog *cd = new QDialog(m_mw, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
	Ui::CommitDialog ui;
	ui.setupUi(cd);
	QString name = QString::fromUtf8(model->getName());
	ui.lbModel->setText(name);
	ui.lbFile->setText(fileName);
	ui.lbRemote->setText(QString("r") +
		QString::number(model->getCurrentRevision()->getNumber()));
	VersionControl::RevisionSeq *revs = model->getRevisions();
	unsigned int revnum = revs->length();
	for (unsigned int j = 0; j < revnum; j++) {
		VersionControl::Revision_var rev = (*revs)[j];
		QString rn;
		unsigned int cur = rev->getNumber();
		ui.cbBase->addItem(QString("r") + QString::number(cur), cur);
	}
	QSettings settings;
	QVariant v = settings.value(QString("checkouts/") + name.toUtf8().toHex(), "");
	if (v == "") {
		ui.lbLocal->setText("N/A");
	} else {
		QString r = QString("r") + v.toString();
		ui.lbLocal->setText(r);
		int i = ui.cbBase->findText(r);
		if (i != -1) ui.cbBase->setCurrentIndex(i);
	}
	if (cd->exec() != QDialog::Accepted) throw CommitCancelled();
	m_autoAccept = ui.cbAutoAccept->isChecked();
	return ui.cbBase->itemData(ui.cbBase->currentIndex()).toUInt();
}

void Controller::commitContent(const char *content, unsigned int base, VersionControl::Model_var model) {
	try {
		model->commit(content, base);
	} catch (VersionControl::NotUptodateException &e) {
		VersionControl::Resolver_var res = m_root->getResolver();
		VersionControl::Revision_var rev = model->getCurrentRevision();
		VersionControl::NamedModel remote, local;
		remote.name = "remote";
		remote.data = rev->getData();
		local.name = "local";
		local.data = content;
		const QString fileName = QString::fromUtf8(model->getName()) + "-merge-base" +
			QString::number(base) + "-remote" + QString::number(rev->getNumber()) + "-local.sql";
		try {
			char *merged = res->merge(model->getRevision(base)->getData(), remote, local);
			if (!m_autoAccept) showBrowser(merged, true, fileName);
			commitContent(merged, rev->getNumber(), model);
			delete merged;
		} catch (VersionControl::ConflictException &e) {
			showBrowser(e.toResolve, false, fileName);
			throw CommitCancelled();
		}
	}
}

void Controller::showBrowser(const char *content, bool ok, const QString fileName) {
	QDialog *bd = new QDialog(m_mw, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
	Ui::BrowserDialog ui;
	ui.setupUi(bd);
	Browser ctrl(bd, &ui, content, fileName);
	QObject::connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)),
		&ctrl, SLOT(clicked(QAbstractButton*)));
	if (!ok) {
		ui.buttonBox->setStandardButtons(QDialogButtonBox::Close);
		ui.label->setText(
			"The commit resulted in conflicts that cannot be resolved automatically. "
			"You can find the source of the model below, the parts where you and someone "
			"else modified at since the base are marked with <b>remote</b> and <b>local</b>. "
			"Click Save to save the version seen below to a file."
		);
	}
	if (bd->exec() != QDialog::Accepted) throw CommitCancelled();
}

void Controller::checkout() {
	QTreeWidgetItem *twi = m_ui->treeWidget->currentItem();
	if (twi == NULL) return;
	QTreeWidgetItem *parent = twi->parent();
	if (parent == NULL) {
		QMessageBox::critical(m_mw, "Invalid selection", "Please select a revision to check out.");
		return;
	}
	unsigned int revnum = atoi(twi->text(0).toUtf8().constData() + 1); // skip 'r'
	const QByteArray ba = parent->text(0).toUtf8();
	try {
		Browser::saveSQL(m_mw, "Select file name to check out to",
			parent->text(0) + "-" + twi->text(0) + ".sql",
			m_root->getModel(ba.constData())->getRevision(revnum)->getData());
		// Successful file write, save revision
		QSettings settings;
		settings.setValue(QString("checkouts/") + ba.toHex(), revnum);
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

void Controller::lock() {
	QTreeWidgetItem *twi = m_ui->treeWidget->currentItem();
	if (twi == NULL) return;
	while (twi->parent() != NULL) twi = twi->parent(); // get model
	try {
		m_root->getModel(twi->text(0).toUtf8().constData())->lock();
		QMessageBox::information(m_mw, "Successful lock",
			"Model is now locked, only you (and administrators) can unlock it.");
		CATCH_LOCKED
		CATCH_DBERROR
		CATCH_INVALIDMODEL
		CATCH_ACCESSDENIED("You don't have write permissions on the selected model.")
	}
}

void Controller::unlock() {
	QTreeWidgetItem *twi = m_ui->treeWidget->currentItem();
	if (twi == NULL) return;
	while (twi->parent() != NULL) twi = twi->parent(); // get model
	try {
		m_root->getModel(twi->text(0).toUtf8().constData())->unlock();
		QMessageBox::information(m_mw, "Successful lock",
			"Model is now unlocked, everyone with write privileges can commit to it.");
		CATCH_LOCKED
		CATCH_DBERROR
		CATCH_INVALIDMODEL
		CATCH_ACCESSDENIED("You don't have write permissions on the selected model.")
	}
}

void Controller::about() {
	QMessageBox::about(m_mw, "About SQL Version Control", QString::fromUtf8(
		"<b>SQL Version Control</b><br />"
		"<a href='http://www.aut.bme.hu/portal/szoftverarchitekturak'>"
		"http://www.aut.bme.hu/portal/szoftverarchitekturak</a><br /><br />"
		"Vajna Miklós<br />Veres-Szentkirályi András<br /><br />2010"
	));
}

Controller::Controller(QWidget *mw, Ui::MainWindow *ui, VersionControl::Root_var root) {
	m_mw = mw;
	m_ui = ui;
	m_root = root;
}
