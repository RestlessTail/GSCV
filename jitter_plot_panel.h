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
    void on_x_customize_clicked();
    void on_y_customize_clicked();
    void on_update_clicked();
    void on_close_clicked();
private:
    void update_preview_x();
    void update_preview_y();
private:
    Ui::jitter_plot_panel *ui;
    bool x_is_continuous;
    bool y_is_continuous;
    int x_step;
    int y_step;
    std::vector<std::string> x_lim;
    std::vector<std::string> y_lim;
};

#endif // JITTER_PLOT_PANEL_H
