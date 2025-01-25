//
// Created by drin on 22/01/2025.
//

#include "mainwindow.h"

#include "datamanager.h"

#include "building.h"
#include "miscrequest.h"

#include <QComboBox>
#include <QLabel>
#include <QMenuBar>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  if (!db::init()) {
    alert_err("Failed to initialize the program.");
    return;
  }
  setup();
}

MainWindow::~MainWindow() {
  db::close();
}

void MainWindow::setup() {
  setWindowTitle("Proj BDR");
  resize(800, 600);

  menu_ = menuBar()->addAction("DataManager");
  connect(menu_, &QAction::triggered, this, &MainWindow::on_menu_clicked);

  QWidget* central_widget = new QWidget(this);
  QVBoxLayout* layout = new QVBoxLayout(central_widget);

  tab_widget_ = new QTabWidget(this);
  layout->addWidget(tab_widget_);

  create_tabs();
  create_payment_tab();

  setCentralWidget(central_widget);
}

void MainWindow::create_tabs() {
  QWidget* tab = new QWidget();
  QVBoxLayout* tab_layout = new QVBoxLayout(tab);

  auto b_list = Model<Building>::get_all_as_str();

  if (!b_list.has_value()) {
    alert_err("An error occured while fetching the buildings.");
  }

  QLabel* building_label = new QLabel("Building", tab);
  building_cb_fee_ = new QComboBox(tab);
  for (const auto& b : b_list.value()) {
    building_cb_fee_->addItem(b.second.data(), b.first);
  }
  tab_layout->addWidget(building_label);
  tab_layout->addWidget(building_cb_fee_);

  QLabel* year_label = new QLabel("Year:", tab);
  year_picker_ = new QSpinBox(tab);
  year_picker_->setRange(1900, 9999);
  year_picker_->setValue(QDate::currentDate().year());
  tab_layout->addWidget(year_label);
  tab_layout->addWidget(year_picker_);

  QPushButton* apply_button = new QPushButton("Apply", tab);
  connect(apply_button, &QPushButton::clicked, this,
          &MainWindow::on_apply_btn_clicked);
  tab_layout->addWidget(apply_button);

  table_widget_ = new QTableWidget(tab);
  table_widget_->setColumnCount(7);
  table_widget_->setRowCount(0);
  QStringList headers = {"Occupancy", "Volume",    "Heat Coef", "Month",
                         "Heat Fee",  "Water Fee", "Misc Fee"};
  table_widget_->setHorizontalHeaderLabels(headers);
  tab_layout->addWidget(table_widget_);

  tab_widget_->addTab(tab, "Fees");
}

void MainWindow::on_menu_clicked() {
  QMainWindow* dm = new Datamanager(this);
  dm->setAttribute(Qt::WA_DeleteOnClose);
  dm->setWindowFlags(dm->windowFlags() | Qt::Dialog);
  dm->setWindowModality(Qt::ApplicationModal);
  dm->show();
}

void MainWindow::on_apply_btn_clicked() {
  int year = year_picker_->value();
  int building_id = building_cb_fee_->currentData().toInt();
  auto fees = MiscRequest::get_all_fees(building_id, year);
  if (!fees.has_value()) {
    alert_err("An error occured while fetching the fees.");
  }
  table_widget_->setRowCount(0);
  for (const auto& fee : fees.value()) {
    table_widget_->insertRow(table_widget_->rowCount());
    table_widget_->setItem(table_widget_->rowCount() - 1, 0,
                           new QTableWidgetItem(QString::number(fee->occ_id)));
    table_widget_->setItem(
        table_widget_->rowCount() - 1, 1,
        new QTableWidgetItem(QString::number(fee->volume, 'f', 2)));
    table_widget_->setItem(
        table_widget_->rowCount() - 1, 2,
        new QTableWidgetItem(QString::number(fee->heat_coefficient, 'f', 2)));
    table_widget_->setItem(
        table_widget_->rowCount() - 1, 3,
        new QTableWidgetItem(QString::number(fee->nb_month)));
    table_widget_->setItem(
        table_widget_->rowCount() - 1, 4,
        new QTableWidgetItem(QString::number(fee->heat_fee, 'f', 2)));
    table_widget_->setItem(
        table_widget_->rowCount() - 1, 5,
        new QTableWidgetItem(QString::number(fee->water_fee, 'f', 2)));
    table_widget_->setItem(
        table_widget_->rowCount() - 1, 6,
        new QTableWidgetItem(QString::number(fee->misc_fee, 'f', 2)));
  }
}

void MainWindow::create_payment_tab() {
  QWidget* tab = new QWidget();
  QVBoxLayout* tab_layout = new QVBoxLayout(tab);

  auto o_list = MiscRequest::get_all_occ();

  QLabel* occ_label = new QLabel("Occupancy", tab);
  occ_cb_pay_ = new QComboBox(tab);
  for (const auto& o : o_list.value()) {
    occ_cb_pay_->addItem(o.second.data(), o.first);
  }
  tab_layout->addWidget(occ_label);
  tab_layout->addWidget(occ_cb_pay_);

  QPushButton* apply_btn_pay = new QPushButton("Apply", tab);
  connect(apply_btn_pay, &QPushButton::clicked, this,
          &MainWindow::on_payment_apply_clicked);
  tab_layout->addWidget(apply_btn_pay);

  payment_table_ = new QTableWidget(tab);
  payment_table_->setColumnCount(8);
  payment_table_->setRowCount(0);
  QStringList headers = {"Occupancy", "Start date", "Rent",   "Fees",
                         "Ddm",       "Pay. date",  "Amount", "Type"};
  payment_table_->setHorizontalHeaderLabels(headers);
  tab_layout->addWidget(payment_table_);

  tab_widget_->addTab(tab, "Payments");
}

void MainWindow::on_payment_apply_clicked() {
  int occ_id = occ_cb_pay_->currentData().toInt();

  auto payments = MiscRequest::get_all_pay(occ_id);
  if (!payments.has_value()) {
    alert_err("An error occurred while fetching the payments.");
    return;
  }

  payment_table_->setRowCount(0);
  for (const auto& payment : payments.value()) {
    payment_table_->insertRow(payment_table_->rowCount());
    payment_table_->setItem(
        payment_table_->rowCount() - 1, 0,
        new QTableWidgetItem(QString::number(payment->occ_id)));
    payment_table_->setItem(
        payment_table_->rowCount() - 1, 1,
        new QTableWidgetItem(QString::fromStdString(payment->start_date)));
    payment_table_->setItem(
        payment_table_->rowCount() - 1, 2,
        new QTableWidgetItem(QString::number(payment->rent, 'f', 2)));
    payment_table_->setItem(
        payment_table_->rowCount() - 1, 3,
        new QTableWidgetItem(QString::number(payment->fees, 'f', 2)));
    payment_table_->setItem(
        payment_table_->rowCount() - 1, 4,
        new QTableWidgetItem(QString::number(payment->ddm)));
    payment_table_->setItem(
        payment_table_->rowCount() - 1, 5,
        new QTableWidgetItem(QString::fromStdString(payment->date)));
    payment_table_->setItem(
        payment_table_->rowCount() - 1, 6,
        new QTableWidgetItem(QString::number(payment->amount, 'f', 2)));
    payment_table_->setItem(
        payment_table_->rowCount() - 1, 7,
        new QTableWidgetItem(QString::fromStdString(payment->payment_type)));
  }
}