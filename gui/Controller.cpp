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

Controller::Controller(QWidget *mw, Ui::MainWindow *ui, VersionControl::Root_var root) {
	m_mw = mw;
	m_ui = ui;
	m_root = root;
}
