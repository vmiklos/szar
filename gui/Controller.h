#include <VersionControl.hh>
#include <QInputDialog>
#include <QObject>
#include <QMessageBox>

class Controller : public QObject {
	Q_OBJECT

protected:
	VersionControl::Root_var m_root;
	QWidget *m_mw;
public:
	Controller(QWidget *mw, VersionControl::Root_var root);
public slots:
	void addModel();
};
