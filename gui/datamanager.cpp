//
// Created by drin on 21/12/2024.
//

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

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QListWidget>
#include <QMainWindow>
#include <QMenuBar>

Datamanager::Datamanager(QWidget* parent) : QMainWindow(parent) {

  nav_dock_ = new QDockWidget("Navigation", this);
  nav_dock_->setFeatures(QDockWidget::DockWidgetClosable);
  nav_dock_->setAllowedAreas(Qt::LeftDockWidgetArea);

  QListWidget* nav_list = new QListWidget(nav_dock_);
  nav_list->addItem("Building");
  nav_list->addItem("Flat");
  nav_list->addItem("Occupancy");
  nav_list->addItem("Tenant");
  nav_list->addItem("OccTenant");
  nav_list->addItem("PaymentType");
  nav_list->addItem("Payment");
  nav_list->addItem("WaterMeter");
  nav_list->addItem("WaterMeterFlat");
  nav_list->addItem("WaterMeasurement");
  nav_list->addItem("Company");
  nav_list->addItem("RoomType");
  nav_list->addItem("RoomTypeFlat");
  nav_list->addItem("FeeType");
  nav_list->addItem("Invoice");
  nav_list->addItem("HeatMonthlyCoef");

  connect(nav_list, &QListWidget::itemClicked, this,
          [this](QListWidgetItem* item) { prep_qtable(item->text()); });

  nav_dock_->setWidget(nav_list);

  addDockWidget(Qt::LeftDockWidgetArea, nav_dock_);

  add_menu_ = menuBar()->addAction("Add");
  add_menu_->setDisabled(true);

  QMenu* view_menu = menuBar()->addMenu("&View");
  QAction* toggle_nav_action = nav_dock_->toggleViewAction();
  toggle_nav_action->setShortcut(QKeySequence("Ctrl+N"));
  toggle_nav_action->setText("Show Navigation");
  view_menu->addAction(toggle_nav_action);

  table_ = new QTableWidget(this);
  setCentralWidget(table_);

  setWindowTitle(WIN_TITLE_);
  resize(WIN_WIDTH_, WIN_HEIGHT_);
}

Datamanager::~Datamanager() {}

void Datamanager::prep_qtable(const QString& model_str) {
  add_menu_->setEnabled(true);
  add_menu_->disconnect();
  if (model_str == "Building") {
    return load_qtable<Building>();
  }
  if (model_str == "Flat") {
    return load_qtable<Flat>();
  }
  if (model_str == "Occupancy") {
    return load_qtable<Occupancy>();
  }
  if (model_str == "Tenant") {
    return load_qtable<Tenant>();
  }
  if (model_str == "OccTenant") {
    return load_qtable<Occupancy_Tenant>();
  }
  if (model_str == "PaymentType") {
    return load_qtable<PaymentType>();
  }
  if (model_str == "Payment") {
    return load_qtable<Payment>();
  }
  if (model_str == "WaterMeter") {
    return load_qtable<WaterMeter>();
  }
  if (model_str == "WaterMeterFlat") {
    return load_qtable<Flat_WaterMeter>();
  }
  if (model_str == "WaterMeasurement") {
    return load_qtable<WaterMeasurement>();
  }
  if (model_str == "Company") {
    return load_qtable<Company>();
  }
  if (model_str == "RoomType") {
    return load_qtable<RoomType>();
  }
  if (model_str == "RoomTypeFlat") {
    return load_qtable<RoomType_Flat>();
  }
  if (model_str == "FeeType") {
    return load_qtable<FeeType>();
  }
  if (model_str == "Invoice") {
    return load_qtable<Invoice>();
  }
  if (model_str == "HeatMonthlyCoef") {
    return load_qtable<HeatMonthlyCoefficient>();
  }
}
