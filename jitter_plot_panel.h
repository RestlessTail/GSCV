#ifndef JITTER_PLOT_PANEL_H
#define JITTER_PLOT_PANEL_H

#include "right_panel_manager.h"

namespace Ui {
class jitter_plot_panel;
}

class jitter_plot_panel : public right_panel_base
{
    Q_OBJECT
public:
    explicit jitter_plot_panel(QWidget *parent = nullptr);
    ~jitter_plot_panel();

private slots:
    void on_x_set_order_clicked();
    void on_y_set_order_clicked();
    void on_update_clicked();
    void on_close_clicked();

private:
    Ui::jitter_plot_panel *ui;
};

#endif // JITTER_PLOT_PANEL_H
