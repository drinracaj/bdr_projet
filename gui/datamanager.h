//
// Created by drin on 21/12/2024.
//

#ifndef RJMMO_DATAMANAGER_H
#define RJMMO_DATAMANAGER_H

#include "form.h"
#include "model.h"
#include "utils.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>

#include <iostream>

const std::string NULL_VAL = "NULL";

class Datamanager : public QMainWindow {
 public:
  explicit Datamanager(QWidget* parent = nullptr);
  ~Datamanager() override;

 private slots:
  void prep_qtable(const QString& model_str);

 private:
  //CONST window settings
  //TODO maybe create a .conf file to save these but flemme
  const int WIN_WIDTH_ = 800;
  const int WIN_HEIGHT_ = 600;
  const QString WIN_TITLE_ = "rjmmo";

  QDockWidget* nav_dock_;
  QTableWidget* table_ = nullptr;
  QAction* add_menu_;

  template <typename T>
  void connect_action_btn(QPushButton* edit, QPushButton* remove,
                          std::unique_ptr<T> data) {
    int id = data->id();
    connect(edit, &QPushButton::clicked, this,
            [this, ptr = std::move(data)]() mutable {
              auto form = std::make_unique<Form<T>>(table_, ptr.get());
              if (form->exec() == QDialog::Accepted) {
                load_qtable<T>();
              }
            });
    connect(remove, &QPushButton::clicked, this, [this, id] {
      if (QMessageBox::StandardButton confirmation = QMessageBox::critical(
              table_, "Confirm deletion",
              "Are you sure you want to delete this item ?",
              QMessageBox::Yes | QMessageBox::No);
          confirmation == QMessageBox::Yes) {
        if (T::remove(id)) {
          load_qtable<T>();
        } else {
          alert_err("Something went wrong while trying to delete the item.");
        }
      }
    });
  }

  template <typename T>
  void setup_add_menu() {
    if (!add_menu_->isEnabled()) {
      add_menu_->setEnabled(true);
    }
    add_menu_->disconnect();
    connect(add_menu_, &QAction::triggered, this, [this]() {
      T data;
      auto form = std::make_unique<Form<T>>(this, &data, true);
      if (form->exec() == QDialog::Accepted) {
        load_qtable<T>();
      }
    });
  }

  template <typename T>
  QWidget* create_action_btn(std::unique_ptr<T> data) {
    QWidget* btn_widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(btn_widget);
    layout->setContentsMargins(4, 4, 4, 4);

    QPushButton* edit_btn = new QPushButton("Edit");
    QPushButton* delete_btn = new QPushButton("Delete");

    layout->addWidget(edit_btn);
    layout->addWidget(delete_btn);
    layout->addStretch();

    connect_action_btn<T>(edit_btn, delete_btn, std::move(data));

    return btn_widget;
  }

  template <typename T>
  void load_qtable() {
    auto opt_data = T::get_all();
    if (!opt_data.has_value()) {
      alert_err(
          "Could not load the data. Look at the logs for more information.");
      return;
    }

    setup_add_menu<T>();

    table_->clear();
    table_->setColumnCount(T::table().columns.size() + 1);
    table_->setRowCount(0);

    QStringList headers;
    for (const auto& col : T::table().columns) {
      headers.push_back(col.label().data());
    }
    headers.push_back("Actions");
    table_->setHorizontalHeaderLabels(headers);

    for (auto& elem : opt_data.value()) {
      int col_cnt = 0;
      table_->insertRow(table_->rowCount());
      for (const Column<T>& col : T::table().columns) {
        QTableWidgetItem* item = nullptr;
        item = new QTableWidgetItem(qtable_get_helper<T>(col, *elem.second));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        table_->setItem(table_->rowCount() - 1, col_cnt++, item);
      }
      QWidget* btn_widget = create_action_btn<T>(std::move(elem.second));
      table_->setCellWidget(table_->rowCount() - 1, col_cnt, btn_widget);
    }
    table_->resizeColumnsToContents();
    table_->verticalHeader()->setVisible(false);
  }
};

#endif  //RJMMO_DATAMANAGER_H
