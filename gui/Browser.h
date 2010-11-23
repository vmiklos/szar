#include <QFileDialog>
#include <QMessageBox>
#include "ui_browser.h"

class Browser : public QObject {
	Q_OBJECT

protected:
	Ui::BrowserDialog *m_ui;
	QDialog *m_d;
	QAbstractButton *m_save;
	QString m_fileName;
	const char *m_content;
	void addButtons();
public:
	Browser(QDialog *d, Ui::BrowserDialog *ui, const char *content, const QString fileName);
	static void saveSQL(QWidget *parent, const QString title,
	    const QString filename, const char *contents);
public slots:
	void clicked(QAbstractButton *button);
};
