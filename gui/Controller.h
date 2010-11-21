#include <VersionControl.hh>
#include <QInputDialog>
#include <QObject>
#include <QMessageBox>
#include "ui_main.h"

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
};
