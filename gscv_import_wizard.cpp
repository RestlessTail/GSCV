#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QMessageBox>
#include "gscv_import_wizard.h"
#include "sc_packer.h"

GSCV_import_wizard::GSCV_import_wizard(QWidget* parent) : QWizard(parent) {
    setWizardStyle(QWizard::ModernStyle);
    setPage(1, &page_1);
    setPage(2, &page_2);
    setPage(3, &page_3);
    setPage(4, &page_4);
    setPage(5, &page_5);
    setPage(6, &page_6);
    button(QWizard::NextButton)->show();
    button(QWizard::BackButton)->show();
    button(QWizard::CancelButton)->show();
    setOption(QWizard::NoBackButtonOnLastPage);
}
void GSCV_import_wizard::reject(){
    if(page_6.running && (!page_6.complete)){
        auto ret = QMessageBox::question(this, tr("Warning"), tr("Packing thread is running. Aborting packing thread can be dangerous. Do you want to continue?"));
        if(ret == QMessageBox::Yes){
            page_6.terminate_thread();
            done(QDialog::Rejected);
        }
    }
    else{
        done(QDialog::Rejected);
    }
}
void GSCV_import_wizard::closeEvent(QCloseEvent* event){
    if(page_6.running && (!page_6.complete)){
        auto ret = QMessageBox::question(this, tr("Warning"), tr("Packing thread is running. Aborting packing thread can be dangerous. Do you want to continue?"));
        if(ret == QMessageBox::Yes){
            page_6.terminate_thread();
            event->accept();
        }
        else{
            event->ignore();
        }
    }
    else{
        event->accept();
    }
}
GSCV_import_wizard_page_1::GSCV_import_wizard_page_1() {
    setTitle(tr("GSCV import wizard"));
    primary = new QVBoxLayout;
    intro = new QLabel;
    intro->setText(tr("Welcome to GSCV import wizard. This wizard will help you convert a Seurat object (of R language) to a gscv archive."));
    intro->setWordWrap(true);
    primary->addWidget(intro);
    setLayout(primary);
}
GSCV_import_wizard_page_2::GSCV_import_wizard_page_2() {
    setTitle(tr("Export seurat object"));
    setSubTitle(tr("Please copy the scripts and paste them in your R console, which will generate intermediate files automatically."));
    primary = new QVBoxLayout;
    intro = new QLabel;
    intro->setText(tr("This function converts a seurat object to text files. The first argument is a seurat object, and the second argument is the filename of output."));
    intro->setWordWrap(true);

    script = new QTextEdit;
    script->setReadOnly(true);
    script->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    script->setText("export_gscv <- function(obj, pro){\n"
        "    if(dir.exists(pro) == F){\n"
        "        dir.create(pro)\n"
        "    }\n"
        "    write.table(obj@meta.data, file = paste0(pro, \"/meta.tsv\"), quote = F, sep = \"\\t\", row.names = T, col.names = T)\n"
        "    data = t(FetchData(obj, vars = obj@assays$RNA@data@Dimnames[[1]]))\n"
        "    write.table(colnames(data), paste0(pro, \"/index.tsv\"), quote=F, sep=\"\\t\", col.names=F, row.names=F)\n"
        "    write.table(data, paste0(pro, \"/expr.tsv\"), quote=F, sep=\"\\t\", col.names=F)\n"
        "    write.table(Embeddings(obj,\"umap\"), file = paste0(pro, \"/position.tsv\"), quote = F, sep = \"\\t\", row.names = T, col.names = F)\n"
        "}"
    );
    QFontMetrics metrix(script->font());
    script->setMinimumHeight(metrix.height() * 8);
    script->setMaximumHeight(metrix.height() * 8);

    primary->addWidget(intro);
    primary->addWidget(script);
    setLayout(primary);
}
GSCV_import_wizard_page_3::GSCV_import_wizard_page_3() {
    setTitle(tr("Export seurat object"));
    setSubTitle(tr("Please copy the scripts and paste them in your R console, which will generate intermediate files automatically."));
    primary = new QVBoxLayout;

    example = new QLabel;
    example->setText(tr("For example, to process the seurat object \"hsa\", use the following script:"));
    example->setWordWrap(true);

    example_script = new QLineEdit;
    example_script->setReadOnly(true);
    example_script->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    example_script->setText("export_gscv(hsa, \"hsa_2022_7_1\")");

    example2 = new QLabel;
    example2->setText(tr("This script exports the object \"hsa\", and create a directory called \"hsa_2022_7_1\", where the exported text files are saved."));
    example2->setWordWrap(true);

    primary->addWidget(example);
    primary->addWidget(example_script);
    primary->addWidget(example2);

    setLayout(primary);
}
GSCV_import_wizard_page_4::GSCV_import_wizard_page_4() {
    setTitle(tr("Select the directory just created"));
    setSubTitle(tr("Please select the directory just created by the R script."));
    primary = new QVBoxLayout;
    intro = new QLabel;
    intro->setWordWrap(true);
    intro->setText(tr("The R script creates a directory, whose name is specified by the second argument of export_gscv(). The directory contains four tab-separated values files (*.tsv). Please select this directory by clicking the \"select\" button."));

    directory_box = new QHBoxLayout;
    field_name = new QLabel;
    field_name->setText(tr("Directory:"));
    directory = new QLineEdit;
    directory->setText("");
    registerField("directory*", directory);
    select = new QPushButton;
    select->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    select->setText(tr("select"));
    directory_box->addWidget(field_name);
    directory_box->addWidget(directory);
    directory_box->addWidget(select);
    connect(select, &QPushButton::clicked, this, &GSCV_import_wizard_page_4::select_directory);

    primary->addWidget(intro);
    primary->addLayout(directory_box);
    setLayout(primary);
}
void GSCV_import_wizard_page_4::initializePage(){
    directory->setText(directory_val);
    setField("directory", directory_val);
}
void GSCV_import_wizard_page_4::select_directory() {
    directory_val = QFileDialog::getExistingDirectory(this, tr("Choose a directory"));
    directory->setText(directory_val);
    setField("directory", directory_val);
}
GSCV_import_wizard_page_5::GSCV_import_wizard_page_5() {
    setTitle(tr("Set filename"));
    setSubTitle(tr("Please set the filename of the output."));
    primary = new QVBoxLayout;
    intro = new QLabel;
    intro->setWordWrap(true);
    intro->setText(tr("The output will be written to a specified file. Please set filename by clicking the \"select\" button."));

    directory_box = new QHBoxLayout;
    field_name = new QLabel;
    field_name->setText(tr("Filename:"));
    out_filename = new QLineEdit;
    out_filename->setText("");
    registerField("out_filename*", out_filename);
    select = new QPushButton;
    select->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    select->setText(tr("select"));
    directory_box->addWidget(field_name);
    directory_box->addWidget(out_filename);
    directory_box->addWidget(select);
    connect(select, &QPushButton::clicked, this, &GSCV_import_wizard_page_5::set_out_filename);

    primary->addWidget(intro);
    primary->addLayout(directory_box);
    setLayout(primary);
}
void GSCV_import_wizard_page_5::initializePage(){
    out_filename->setText(filename_val);
    setField("out_filename", filename_val);
}
void GSCV_import_wizard_page_5::set_out_filename() {
    QString caption = tr("Set filename");
    QString filter = tr("GSCV Archive (*.gscvArchive)");
    QString filename;
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
    filename = QFileDialog::getSaveFileName(this, caption, QString(), filter);
    if(filename.isEmpty()){
        return;
    }
#else
    //QFileDialog::getSaveFileName doesn't add extension to a filename without extension on Linux
    //It is required to check extension manually
    QFileDialog dlg(this, caption);
    dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setNameFilter(filter);
    if(dlg.exec()){
        QStringList files = dlg.selectedFiles();
        if(files.size() < 1){ return; }
        filename = files[0];
    }
    //check extension
    if(filename.isEmpty()){ return; }
    QString selected_filter = dlg.selectedNameFilter();
    int extension_begin = selected_filter.indexOf('*') + 1;
    int extension_size = selected_filter.size() - extension_begin - 1;
    selected_filter = selected_filter.mid(extension_begin, extension_size);
    if(!filename.endsWith(selected_filter)){
        filename = filename + selected_filter;
    }
#endif
	filename_val = filename;
    out_filename->setText(filename);
    setField("out_filename", filename);
}

GSCV_import_wizard_page_6::GSCV_import_wizard_page_6() {
	complete = false;
    running = false;
    packer = nullptr;
    primary = new QVBoxLayout;

    label = new QLabel;
    label->setText(tr("Progress"));
    progress_bar = new QProgressBar;
	progress_bar->setMaximum(4);
	progress_bar->setMinimum(0);
	progress_bar->setValue(0);
    detail = new QPlainTextEdit;

    primary->addWidget(label);
    primary->addWidget(progress_bar);
    primary->addWidget(detail);

    setLayout(primary);
}

void GSCV_import_wizard_page_6::initializePage(){
    running = true;
	emit completeChanged();
    QString src = field("directory").toString();
    QString dest = field("out_filename").toString();
    packer = new sc_packer;
    connect(packer, &sc_packer::finished, packer, &QObject::deleteLater);
	connect(packer, &sc_packer::console_log, this, &GSCV_import_wizard_page_6::add_log);
	connect(packer, &sc_packer::progress_bar_max, this, &GSCV_import_wizard_page_6::set_progress_bar_max);
	connect(packer, &sc_packer::progress_bar_val, this, &GSCV_import_wizard_page_6::set_progress_bar_val);
    connect(packer, &sc_packer::finished, this, &GSCV_import_wizard_page_6::set_completed);
	packer->set_src_dir(src.toStdString());
	packer->set_dest_dir(dest.toStdString());
	packer->start();
}

void GSCV_import_wizard_page_6::terminate_thread(){
    if(running && packer){
        packer->terminate();
    }
}

bool GSCV_import_wizard_page_6::isComplete() const{
	return complete;
}

void GSCV_import_wizard_page_6::set_completed(){
	complete = true;
	emit completeChanged();
}

void GSCV_import_wizard_page_6::add_log(QString msg){
	detail->appendPlainText(msg);
}
void GSCV_import_wizard_page_6::set_progress_bar_max(int val){
	progress_bar->setMaximum(val);
}
void GSCV_import_wizard_page_6::set_progress_bar_val(int val){
	progress_bar->setValue(val);
}
