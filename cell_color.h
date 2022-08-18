#ifndef CELL_COLOR_H
#define CELL_COLOR_H

#include <QWidget>

class QTabWidget;
class opengl_view_widget;

namespace Ui {
class cell_color;
}

class cell_color : public QWidget
{
    Q_OBJECT

public:
    explicit cell_color(QWidget *parent = nullptr);
    ~cell_color();
    void set_view_widget(opengl_view_widget* widget){
        view_widget = widget;
        update_data();
    }
    void update_data();
    void set_tab_widget_master(QTabWidget *newTab_widget_master);
    void set_to_this(cell_color **newTo_this);

private slots:
    void on_search_gene_go_clicked();
    void on_meta_go_clicked();
    void on_clear_color_clicked();

    void on_meta_combobox_currentTextChanged(const QString &arg1);
    void on_close_panel_clicked();

private:
    //sc_data* data;
    opengl_view_widget* view_widget;
    Ui::cell_color *ui;
    QTabWidget* tab_widget_master;
    cell_color** to_this;
};

#endif // CELL_COLOR_H