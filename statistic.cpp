#include "statistic.h"
#include "ui_statistic.h"
#include "opengl_view_widget.h"

statistic::statistic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::statistic)
{
    view_widget = nullptr;
    ui->setupUi(this);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->setHorizontalHeaderLabels({tr("Property"), tr("Value")});
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

statistic::~statistic()
{
    delete ui;
}

int statistic::update_statistic()
{
    if(view_widget == nullptr){
        return -1;
    }
    else{
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        add_property(tr("Name"), QString::fromStdString(view_widget->get_archive_name()));
        add_property(tr("Cells"), view_widget->get_n_cell());
        add_property(tr("Genes"), view_widget->get_n_gene());
        ui->tableWidget->setHorizontalHeaderLabels({QString(tr("Property")), QString(tr("Value"))});
    }
    return 0;
}

template<typename T1, typename T2>
void statistic::add_property(T1 key, T2 value)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(key));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(value));
}

template<typename T1>
void statistic::add_property(T1 key, size_t value)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(key));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(value)));
}

void statistic::set_view_widget(opengl_view_widget *widget){
    view_widget = widget;
}