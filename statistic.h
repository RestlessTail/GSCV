#ifndef STATISTIC_H
#define STATISTIC_H

#include <QWidget>

class opengl_view_widget;

namespace Ui {
class statistic;
}

class statistic : public QWidget
{
    Q_OBJECT

public:
    explicit statistic(QWidget *parent = nullptr);
    ~statistic();
    void set_view_widget(opengl_view_widget* widget);
    int update_statistic();

private:
    template<typename T1, typename T2> void add_property(T1 key, T2 value);
    template<typename T1> void add_property(T1 key, size_t value);
    opengl_view_widget* view_widget;
    Ui::statistic *ui;
};

#endif // STATISTIC_H
