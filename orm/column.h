//
// Created by drin on 16/12/2024.
//

#ifndef RJMMO_COLUMN_H
#define RJMMO_COLUMN_H

#include "columntype.h"

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <variant>
#include <vector>

const std::string FK_SUFFIX = "_as_str";

template <typename T>
class Column {
 public:
  std::function<FieldGetter(const T&)> getter;
  std::function<void(T&, FieldSetter)> setter;

  //CONSTRUCTORS
  //COMPLETE
  Column(
      std::function<FieldGetter(const T&)> f_getter,
      //TODO this is not ideal, since the get is done in the lamda in the derived model class
      //and it'll throw an error if the value passed is not of the right type, must think of a better solution
      std::function<void(T&, FieldSetter)> f_setter, std::string name,
      std::string label, ColumnType type, bool is_null = false, int imin = 0,
      int imax = 0, double dmin = .0, double dmax = .0, int decimal = 0,
      int vlen = 0, OnDelete od = OnDelete::NA,
      std::type_index ref = typeid(void))
      : getter(std::move(f_getter)),
        setter(std::move(f_setter)),
        name_(std::move(name)),
        label_(std::move(label)),
        type_(type),
        nullable_(is_null),
        imin_(imin),
        imax_(imax),
        dmin_(dmin),
        dmax_(dmax),
        decimal_(decimal),
        vlen_(vlen),
        od_(od),
        fk_ref_(ref) {}

  //INTEGER
  Column(std::function<FieldGetter(const T&)> f_getter,
         std::function<void(T&, FieldSetter)> f_setter, std::string name,
         std::string label, const ColumnType type, int min, int max,
         bool is_null = false)
      : Column(std::move(f_getter), std::move(f_setter), std::move(name),
               std::move(label), type, is_null, min, max) {}

  //REAL
  Column(std::function<FieldGetter(const T&)> f_getter,
         std::function<void(T&, FieldSetter)> f_setter, std::string name,
         std::string label, const ColumnType type, double min, double max,
         int decimal, bool is_null = false)
      : Column(std::move(f_getter), std::move(f_setter), std::move(name),
               std::move(label), type, is_null, 0, 0, min, max, decimal) {}

  //VARCHAR
  Column(std::function<FieldGetter(const T&)> f_getter,
         std::function<void(T&, FieldSetter)> f_setter, std::string name,
         std::string label, const ColumnType type, int vlen,
         bool is_null = false)
      : Column(std::move(f_getter), std::move(f_setter), std::move(name),
               std::move(label), type, is_null, 0, 0, .0, .0, 0, vlen) {}

  //FK
  Column(std::function<FieldGetter(const T&)> f_getter,
         std::function<void(T&, FieldSetter)> f_setter, std::string name,
         std::string label, const ColumnType type, OnDelete od,
         std::type_index ref, bool is_null = false)
      : Column(std::move(f_getter), std::move(f_setter), std::move(name),
               std::move(label), type, is_null, 0, 0, .0, .0, 0, 0, od, ref) {}

  [[nodiscard]] const std::string& name() const { return name_; }
  [[nodiscard]] const std::string& label() const { return label_; }
  [[nodiscard]] ColumnType type() const { return type_; }
  [[nodiscard]] bool nullable() const { return nullable_; }
  [[nodiscard]] int imin() const { return imin_; }
  [[nodiscard]] int imax() const { return imax_; }
  [[nodiscard]] int dmin() const { return dmin_; }
  [[nodiscard]] int dmax() const { return dmax_; }
  [[nodiscard]] int decimal() const { return decimal_; }
  [[nodiscard]] int vlen() const { return vlen_; }
  [[nodiscard]] OnDelete od() const { return od_; }
  [[nodiscard]] std::type_index fk_ref() const { return fk_ref_; }

 private:
  //COMMON
  const std::string name_;
  const std::string label_;
  const ColumnType type_;
  const bool nullable_;
  //INTEGER
  const int imin_;
  const int imax_;
  //REAL
  const double dmin_;
  const double dmax_;
  const int decimal_;
  //VARCHAR
  const int vlen_;
  //FK
  const OnDelete od_;
  const std::type_index fk_ref_;
};

template <typename T>
using Columns = std::vector<Column<T>>;

template <typename T>
struct Table {
  const std::type_index&
      table_index;  //TODO maybe useless to store it as a const &
  const Columns<T>& columns;
};

#endif  //RJMMO_COLUMN_H
