#ifndef GSCV_IMPORT_WIZARD_H
#define GSCV_IMPORT_WIZARD_H

#include <QWizard>
#include <QWizardPage>

class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QTextEdit;
class QFrame;
class QPlainTextEdit;
class QLineEdit;
class QProgressBar;
class sc_packer;

class GSCV_import_wizard_page_1 : public QWizardPage {
    Q_OBJECT
public:
    GSCV_import_wizard_page_1();
private:
    QVBoxLayout* primary;
    QLabel* intro;
};

class GSCV_import_wizard_page_2 : public QWizardPage {
    Q_OBJECT
public:
    GSCV_import_wizard_page_2();
private:
    QVBoxLayout* primary;
    QLabel* intro;
    QTextEdit* script;
};

class GSCV_import_wizard_page_3 : public QWizardPage {
    Q_OBJECT
public:
    GSCV_import_wizard_page_3();
private:
    QVBoxLayout* primary;
    QLabel* example;
    QLineEdit* example_script;
    QLabel* example2;
};

class GSCV_import_wizard_page_4 : public QWizardPage {
    Q_OBJECT
public:
    GSCV_import_wizard_page_4();
    void initializePage() override;
private:
    QString directory_val;
private:
    void select_directory();
    QVBoxLayout* primary;
    QLabel* intro;
    QHBoxLayout* directory_box;
    QLabel* field_name;
    QLineEdit* directory;
    QPushButton* select;
};

class GSCV_import_wizard_page_5 : public QWizardPage {
    Q_OBJECT
public:
    GSCV_import_wizard_page_5();
    void initializePage() override;
private:
    QString filename_val;
private:
    void set_out_filename();
    QVBoxLayout* primary;
    QLabel* intro;
    QHBoxLayout* directory_box;
    QLabel* field_name;
    QLineEdit* out_filename;
    QPushButton* select;
};

class GSCV_import_wizard_page_6 : public QWizardPage {
    Q_OBJECT
public:
    GSCV_import_wizard_page_6();
    void initializePage() override;
	bool isComplete() const override;
    bool running;
	bool complete;
private:
    void select_directory();
    QVBoxLayout* primary;
    QLabel* label;
    QProgressBar* progress_bar;
    QPlainTextEdit* detail;
    sc_packer* packer;
public slots:
    void terminate_thread();
private slots:
    void add_log(QString msg);
    void set_progress_bar_max(int val);
    void set_progress_bar_val(int val);
	void set_completed();
};

class GSCV_import_wizard : public QWizard
{
    Q_OBJECT
public:
    explicit GSCV_import_wizard(QWidget* parent = nullptr);
    void closeEvent(QCloseEvent* event) override;
    void reject() override;
private:
    GSCV_import_wizard_page_1 page_1;
    GSCV_import_wizard_page_2 page_2;
    GSCV_import_wizard_page_3 page_3;
    GSCV_import_wizard_page_4 page_4;
    GSCV_import_wizard_page_5 page_5;
    GSCV_import_wizard_page_6 page_6;
};

#endif // GSCV_IMPORT_WIZARD_H