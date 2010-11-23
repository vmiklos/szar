#include "Browser.h"

void Browser::clicked(QAbstractButton *button) {
	if (button == m_save)
		saveSQL(m_d, "Select file to export merge result to", m_fileName, m_content);
}

void Browser::addButtons() {
	QDialogButtonBox *bb = m_ui->buttonBox;
	m_save = (QAbstractButton*)bb->addButton("Save as...", QDialogButtonBox::ActionRole);
}

void Browser::saveSQL(QWidget *parent, const QString title,
	const QString filename, const char *contents) {
	QString fn = QFileDialog::getSaveFileName(parent, title, filename,
		"SQL scripts (*.sql);;All files (*.*)");
	if (fn == NULL) return; // Cancel
	QFile file(fn);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
		QMessageBox::critical(0, "Cannot open file",
			"The selected file cannot be opened. Check permissions.");
		return;
	}
	file.write(contents);
}

Browser::Browser(QDialog *d, Ui::BrowserDialog *ui, const char *content, const QString fileName) {
	m_d = d;
	m_ui = ui;
	m_content = content;
	m_fileName = fileName;
	ui->code->document()->setPlainText(QString::fromUtf8(content));
	addButtons();
}
