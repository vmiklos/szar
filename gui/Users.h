#include <VersionControl.hh>
#include <QInputDialog>
#include <QObject>
#include <QMessageBox>
#include "ui_users.h"
#include "Exceptions.h"

class Users : public QObject {
	Q_OBJECT

protected:
	VersionControl::Admin_var m_admin;
	Ui::UsersDialog *m_ui;
	QDialog *m_d;
	QAbstractButton *m_add, *m_remove, *m_pw;
	QHash<QListWidgetItem*, VersionControl::UserAdmin_var> m_hash;
	void addButtons();
public:
	Users(QDialog *d, Ui::UsersDialog *ui, VersionControl::Admin_var admin);
	void buildList();
public slots:
	void clicked(QAbstractButton *button);
	void itemChanged(QListWidgetItem *item);
};
