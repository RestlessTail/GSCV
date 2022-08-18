#include <QColorDialog>
#include <QtWidgets/QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include "color_scheme.h"
#include "ui_color_scheme.h"

color_scheme::color_scheme(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::color_scheme)
{
    ui->setupUi(this);
    ui->gradient_bicolored->setChecked(true);
    ui->gradient_tricolored->setChecked(false);
    setAttribute(Qt::WA_DeleteOnClose, true);
}
color_scheme::~color_scheme()
{
    delete ui;
}
void color_scheme::set_palette(palette_t *p)
{
    palette = p;
    if(p->exp_color_type == color_type::gradient_2){
        ui->gradient_bicolored->setChecked(true);
        ui->gradient_tricolored->setChecked(false);
        on_gradient_bicolored_clicked();
    }
    else if(p->exp_color_type == color_type::gradient_3){
        ui->gradient_bicolored->setChecked(false);
        ui->gradient_tricolored->setChecked(true);
        on_gradient_tricolored_clicked();
    }
    set_gradient_high(p->high);
    set_gradient_mid(p->mid);
    set_gradient_low(p->low);
    set_discrete(p->meta_colors);
    set_background(p->background);
    set_inactive(p->inactive);
}
void color_scheme::set_gradient_high(rgb_color& color)
{
	QString hex = color.get_hex_color();
    ui->gradient_high_hex->setText(color.get_hex_color());
	ui->gradient_high_select->setStyleSheet("background: " + hex + ";");
}
void color_scheme::set_gradient_mid(rgb_color &color)
{
	QString hex = color.get_hex_color();
    ui->gradient_mid_hex->setText(color.get_hex_color());
	ui->gradient_mid_select->setStyleSheet("background: " + hex + ";");
}
void color_scheme::set_gradient_low(rgb_color &color)
{
	QString hex = color.get_hex_color();
    ui->gradient_low_hex->setText(color.get_hex_color());
	ui->gradient_low_select->setStyleSheet("background: " + hex + ";");
}
void color_scheme::set_discrete(color_set &color)
{
    ui->discrete_preview->clear();
	int size = color.size();
	for(int i = 0; i < size; ++i){
		add_discrete_color(color[i], i);
    }
}
void color_scheme::set_inactive(rgb_color &color)
{
	QString hex = color.get_hex_color();
    ui->inactive_hex->setText(color.get_hex_color());
	ui->inactive_select->setStyleSheet("background: " + hex + ";");
}
void color_scheme::set_background(rgb_color &color)
{
	QString hex = color.get_hex_color();
    ui->background_hex->setText(color.get_hex_color());
	ui->background_select->setStyleSheet("background: " + hex + ";");
}
void color_scheme::on_gradient_bicolored_clicked()
{
    palette->exp_color_type = color_type::gradient_2;
    ui->gradient_mid_hex->setEnabled(false);
    ui->gradient_mid_select->setEnabled(false);
}
void color_scheme::on_gradient_tricolored_clicked()
{
    palette->exp_color_type = color_type::gradient_3;
    ui->gradient_mid_hex->setEnabled(true);
    ui->gradient_mid_select->setEnabled(true);
}
void color_scheme::on_gradient_high_select_clicked()
{
    QColor color_old = QColor(palette->high.r_raw(), palette->high.g_raw(), palette->high.b_raw());
    QColor color = QColorDialog::getColor(color_old, this, "Select color");
    if (color.isValid()) {
		rgb_color color_new(color.red(), color.green(), color.blue());
		set_gradient_high(color_new);
		palette->high = color_new;
    }
}
void color_scheme::on_gradient_mid_select_clicked()
{
    QColor color_old = QColor(palette->mid.r_raw(), palette->mid.g_raw(), palette->mid.b_raw());
    QColor color = QColorDialog::getColor(color_old, this, "Select color");
    if (color.isValid()) {
		rgb_color color_new(color.red(), color.green(), color.blue());
		set_gradient_mid(color_new);
		palette->mid = color_new;
    }
}
void color_scheme::on_gradient_low_select_clicked()
{
    QColor color_old = QColor(palette->low.r_raw(), palette->low.g_raw(), palette->low.b_raw());
    QColor color = QColorDialog::getColor(color_old, this, "Select color");
    if (color.isValid()) {
		rgb_color color_new(color.red(), color.green(), color.blue());
		set_gradient_low(color_new);
		palette->low = color_new;
    }
}
void color_scheme::on_inactive_select_clicked()
{
    QColor color_old = QColor(palette->inactive.r_raw(), palette->inactive.g_raw(), palette->inactive.b_raw());
    QColor color = QColorDialog::getColor(color_old, this, "Select color");
    if (color.isValid()) {
		rgb_color color_new(color.red(), color.green(), color.blue());
		set_inactive(color_new);
		palette->inactive = color_new;
    }
}
void color_scheme::on_background_select_clicked()
{
    QColor color_old = QColor(palette->background.r_raw(), palette->background.g_raw(), palette->background.b_raw());
    QColor color = QColorDialog::getColor(color_old, this, "Select color");
    if (color.isValid()) {
		rgb_color color_new(color.red(), color.green(), color.blue());
		set_background(color_new);
		palette->background = color_new;
    }
}
void color_scheme::on_discrete_add_clicked()
{
	palette->meta_colors.add();
	int index = palette->meta_colors.size() - 1;
	add_discrete_color(palette->meta_colors[index], index);
}
void color_scheme::on_discrete_remove_clicked()
{
	int index = ui->discrete_preview->currentRow();
	if(index == -1){ return; }
	palette->meta_colors.remove(index);
	QListWidgetItem* item = ui->discrete_preview->currentItem();
	ui->discrete_preview->removeItemWidget(item);
	delete item;
	int size = palette->meta_colors.size();
	for(int i = 0; i < size; ++i){
		QListWidgetItem* item = ui->discrete_preview->item(i);
		discrete_preview_item* widget = (discrete_preview_item*)(ui->discrete_preview->itemWidget(item));
		widget->color_index = i;
		widget->set_color(palette->meta_colors[i]);
		widget->update_color();
	}
}
void color_scheme::on_discrete_up_clicked()
{
	int index = ui->discrete_preview->currentRow();
	if(index <= 0){ return; }
	palette->meta_colors.move_up(index);
	ui->discrete_preview->setCurrentRow(index - 1);
	int size = palette->meta_colors.size();
	for(int i = 0; i < size; ++i){
		QListWidgetItem* item = ui->discrete_preview->item(i);
		discrete_preview_item* widget = (discrete_preview_item*)(ui->discrete_preview->itemWidget(item));
		widget->color_index = i;
		widget->set_color(palette->meta_colors[i]);
		widget->update_color();
	}
}
void color_scheme::on_discrete_down_clicked()
{
	int index = ui->discrete_preview->currentRow();
	int size = palette->meta_colors.size();
	if(index == -1 || index >= size - 1){ return; }
	palette->meta_colors.move_down(index);
	ui->discrete_preview->setCurrentRow(index + 1);
	for(int i = 0; i < size; ++i){
		QListWidgetItem* item = ui->discrete_preview->item(i);
		discrete_preview_item* widget = (discrete_preview_item*)(ui->discrete_preview->itemWidget(item));
		widget->color_index = i;
		widget->set_color(palette->meta_colors[i]);
		widget->update_color();
	}
}
void color_scheme::on_close_clicked()
{
    tab_widget_master->removeTab(tab_widget_master->indexOf(this));
    *to_this = nullptr;
    deleteLater();
}
void color_scheme::set_to_this(color_scheme **newTo_this)
{
    to_this = newTo_this;
}
void color_scheme::set_tab_widget_master(QTabWidget *newTab_widget_master)
{
    tab_widget_master = newTab_widget_master;
}
void color_scheme::add_discrete_color(rgb_color& color, int index){
	discrete_preview_item* widget = new discrete_preview_item;
	widget->set_color(color);
	widget->color_index = index;
	widget->palette = palette;
	QListWidgetItem* item = new QListWidgetItem;
	item->setSizeHint(widget->sizeHint());
	ui->discrete_preview->addItem(item);
	ui->discrete_preview->setItemWidget(item, widget);
}
discrete_preview_item::discrete_preview_item()
{
	layout = new QHBoxLayout(this);
	hex_label = new QLabel(this);
	hex_label->setObjectName(QString::fromUtf8("hex_label"));
	color_select = new QToolButton(this);
	color_select->setObjectName(QString::fromUtf8("color_select"));
	layout->addWidget(hex_label);
	layout->addWidget(color_select);
	setLayout(layout);
	QMetaObject::connectSlotsByName(this);
	connect(color_select, &QToolButton::clicked, this, [&](){
		QColor color_old = QColor(color_value.r_raw(), color_value.g_raw(), color_value.b_raw());
		QColor color = QColorDialog::getColor(color_old, this, "Select color");
		if (color.isValid()) {
			rgb_color color_new(color.red(), color.green(), color.blue());
			color_value = color_new;
			palette->meta_colors[color_index] = color_new;
			update_color();
		}
	});
}
void discrete_preview_item::set_color(rgb_color& color){
	color_value = color;
	update_color();
}
void discrete_preview_item::update_color(){
	QString hex = color_value.get_hex_color();
	hex_label->setText(hex);
	color_select->setStyleSheet("background: " + hex + ";");
}
rgb_color discrete_preview_item::get_color(){
	return color_value;
}
