//
// Created by drin on 12/22/24.
//

#include "utils.h"

#include <QMessageBox>

void alert_err(const QString& msg, QWidget* parent) {
  QMessageBox::critical(parent, "Error", msg);
}

void alert_inf(const QString& msg, QWidget* parent) {
  QMessageBox::information(parent, "Info", msg);
}