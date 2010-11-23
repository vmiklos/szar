#include <VersionControl.hh>
#include <QInputDialog>
#include <QObject>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include "ui_main.h"
#include "ui_users.h"
#include "ui_commit.h"
#include "ui_browser.h"
#include "Exceptions.h"
#include "Browser.h"
#include "Users.h"
#include "ACL.h"

class CommitCancelled {};

class Controller : public QObject {
	Q_OBJECT

protected:
	VersionControl::Root_var m_root;
	Ui::MainWindow *m_ui;
	QWidget *m_mw;
	bool m_autoAccept;
	void commitContent(const char *content, unsigned int base, VersionControl::Model_var model);
	unsigned int getBase(VersionControl::Model_var model, const QString fileName);
	void showBrowser(const char *content, bool ok, const QString fileName);
public:
	Controller(QWidget *mw, Ui::MainWindow *ui, VersionControl::Root_var root);
public slots:
	void addModel();
	void renameModel();
	void removeModel();
	void manageUsers();
	void editACL();
	void commit();
	void checkout();
	void changePassword();
	void lock();
	void unlock();
	void buildTree();
};
