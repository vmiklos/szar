#include <VersionControl.hh>
#include <QInputDialog>
#include <QObject>
#include <QMessageBox>
#include "ui_users.h"
#include "Exceptions.h"

class ACLController : public QObject {
	Q_OBJECT

protected:
	VersionControl::ModelAdmin_var m_model;
	VersionControl::Admin_var m_admin;
	Ui::UsersDialog *m_ui;
	QDialog *m_d;
	QAbstractButton *m_add, *m_remove;
	QHash<QListWidgetItem*, VersionControl::UserAccess> m_hash;
	void addButtons();
public:
	ACLController(QDialog *d, Ui::UsersDialog *ui, VersionControl::ModelAdmin_var model, VersionControl::Admin_var admin);
	void buildList();
public slots:
	void clicked(QAbstractButton *button);
	void itemChanged(QListWidgetItem *item);
};
