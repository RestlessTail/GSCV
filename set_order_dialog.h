#ifndef SET_ORDER_DIALOG_H
#define SET_ORDER_DIALOG_H

#include <QDialog>
#include <vector>
#include <string>

namespace Ui {
class set_order_dialog;
}

class set_order_dialog : public QDialog
{
    Q_OBJECT
public:
    explicit set_order_dialog(QWidget *parent = nullptr);
    ~set_order_dialog();
    void attach_data(std::vector<std::string>* new_data);
    void accept_data();
private slots:
    void on_up_clicked();
    void on_down_clicked();
private:
    Ui::set_order_dialog *ui;
    std::vector<std::string>* raw_data;
    std::vector<std::string> data;
};

#endif // SET_ORDER_DIALOG_H
