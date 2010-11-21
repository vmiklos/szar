#include <VersionControl.hh>
#include <QInputDialog>
#include <QObject>
#include <QMessageBox>
#include <QFileDialog>
#include "ui_main.h"
#include "ui_users.h"
#include "Exceptions.h"
#include "Users.h"
#include "ACL.h"

class Controller : public QObject {
	Q_OBJECT

protected:
	VersionControl::Root_var m_root;
	Ui::MainWindow *m_ui;
	QWidget *m_mw;
public:
	Controller(QWidget *mw, Ui::MainWindow *ui, VersionControl::Root_var root);
	void buildTree();
public slots:
	void addModel();
	void renameModel();
	void removeModel();
	void manageUsers();
	void editACL();
	void commit();
};
