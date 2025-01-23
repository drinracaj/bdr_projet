//
// Created by drin on 12/22/24.
//

#ifndef UTILS_H
#define UTILS_H

#include "column.h"

#include <QWidget>

void alert_err(const QString& msg, QWidget* parent = nullptr);
void alert_inf(const QString& msg, QWidget* parent = nullptr);

template <typename T>
QString qtable_get_helper(const Column<T>& col, T& data) {
  switch (col.type()) {
    case ColumnType::DATE:
    case ColumnType::VARCHAR:
      if (col.nullable()) {
        auto opt = std::get<StrViewOpt>(col.getter(data));
        return opt.has_value() ? QString::fromStdString(opt.value().data())
                               : QString("NULL");
      }
      return QString::fromStdString(
          std::get<std::string_view>(col.getter(data)).data());
    case ColumnType::INTEGER:
      if (col.nullable()) {
        auto opt = std::get<IntOpt>(col.getter(data));
        return opt.has_value() ? QString::number(opt.value()) : QString("NULL");
      }
      return QString::number(std::get<int>(col.getter(data)));
    case ColumnType::REAL:
      if (col.nullable()) {
        auto opt = std::get<DoubleOpt>(col.getter(data));
        return opt.has_value() ? QString::number(opt.value(), 'f', 2)
                               : QString("NULL");
      }
      return QString::number(std::get<double>(col.getter(data)), 'f', 2);
    case ColumnType::FK:
      return QString::fromStdString(
          std::get<ForeignKeyRef>(col.getter(data)).get().second);
    default:
      return "";
  }
}
#endif  //UTILS_H
