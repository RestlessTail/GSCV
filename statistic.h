#ifndef STATISTIC_H
#define STATISTIC_H

#include "right_panel_manager.h"
#include "ui_statistic.h"

class opengl_view_widget;

namespace Ui {
class statistic;
}

class statistic : public right_panel_base
{
    Q_OBJECT
public:
    explicit statistic(QWidget *parent = nullptr);
    ~statistic();
    int update_statistic();

private:
    template<typename T1, typename T2> void add_property(T1 key, T2 value);
    template<typename T1> void add_property(T1 key, size_t value);
    Ui::statistic *ui;
};

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

#endif // STATISTIC_H