//
// Created by drin on 22/01/2025.
//

#include "mainwindow.h"

#include "datamanager.h"

#include "building.h"
#include "company.h"
#include "feetype.h"
#include "flat.h"
#include "heatmonthlycoef.h"
#include "invoice.h"
#include "miscrequest.h"
#include "occtenant.h"
#include "occupancy.h"
#include "payment.h"
#include "paymenttype.h"
#include "roomtype.h"
#include "roomtypeflat.h"
#include "tenant.h"
#include "watermeasurement.h"
#include "watermeter.h"
#include "watermeterflat.h"

#include <QComboBox>
#include <QLabel>
#include <QMenuBar>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  if (!db::init() || !db::create_sql_table<Building>() ||
      !db::create_sql_table<Flat>() || !db::create_sql_table<Occupancy>() ||
      !db::create_sql_table<Tenant>() || !db::create_sql_table<OccTenant>() ||
      !db::create_sql_table<PaymentType>() ||
      !db::create_sql_table<Payment>() || !db::create_sql_table<WaterMeter>() ||
      !db::create_sql_table<WaterMeterFlat>() ||
      !db::create_sql_table<WaterMeasurement>() ||
      !db::create_sql_table<Company>() || !db::create_sql_table<RoomType>() ||
      !db::create_sql_table<RoomTypeFlat>() ||
      !db::create_sql_table<FeeType>() || !db::create_sql_table<Invoice>() ||
      !db::create_sql_table<HeatMonthlyCoefficient>()) {
    alert_err("Failed to initialize the program.");
    return;
  }
  setup();
}

MainWindow::~MainWindow() {}

void MainWindow::setup() {
  // Set window title and size
  setWindowTitle("TabWidget Example");
  resize(800, 600);

  menu_ = menuBar()->addAction("DataManager");
  connect(menu_, &QAction::triggered, this, &MainWindow::on_menu_clicked);

  // Create central widget and layout
  QWidget* central_widget = new QWidget(this);
  QVBoxLayout* layout = new QVBoxLayout(central_widget);

  // Create and setup TabWidget
  tab_widget_ = new QTabWidget(this);
  layout->addWidget(tab_widget_);

  // Create the tabs
  create_tabs();

  // Set central widget
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
  building_cb_ = new QComboBox(tab);
  for (const auto& b : b_list.value()) {
    building_cb_->addItem(b.second.data(), b.first);
  }
  tab_layout->addWidget(building_label);
  tab_layout->addWidget(building_cb_);

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
  int building_id = building_cb_->currentData().toInt();
  auto fees = MiscRequest::get_all_fees(building_id, year);
  if (!fees.has_value()) {
    alert_err("An error occured while fetching the fees.");
  }
  table_widget_->setRowCount(0);
  for (const auto& fee : fees.value()) {
    table_widget_->insertRow(table_widget_->rowCount());
    table_widget_->setItem(table_widget_->rowCount(), 0,
                           new QTableWidgetItem(QString::number(fee->occ_id)));
    table_widget_->setItem(
        table_widget_->rowCount(), 1,
        new QTableWidgetItem(QString::number(fee->volume, 'f', 2)));
    table_widget_->setItem(
        table_widget_->rowCount(), 2,
        new QTableWidgetItem(QString::number(fee->heat_coefficient, 'f', 2)));
    table_widget_->setItem(
        table_widget_->rowCount(), 3,
        new QTableWidgetItem(QString::number(fee->nb_month)));
    table_widget_->setItem(
        table_widget_->rowCount(), 4,
        new QTableWidgetItem(QString::number(fee->heat_fee, 'f', 2)));
    table_widget_->setItem(
        table_widget_->rowCount(), 5,
        new QTableWidgetItem(QString::number(fee->water_fee, 'f', 2)));
    table_widget_->setItem(
        table_widget_->rowCount(), 6,
        new QTableWidgetItem(QString::number(fee->misc_fee, 'f', 2)));
  }
}