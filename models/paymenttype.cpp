//
// Created by drin on 18/01/2025.
//

#include "paymenttype.h"

const Table<PaymentType>& PaymentType::table() {
  static const Table<PaymentType> table = {
      typeid(PaymentType),
      {Column<PaymentType>(
          [](const PaymentType& pt) -> std::string_view { return pt.type(); },
          [](PaymentType& pt, FieldSetter val) -> void {
            pt.type(std::move(std::get<std::string>(val)));
          },
          "type", "Type", ColumnType::VARCHAR, 15)}};
  return table;
}

const std::string PaymentType::sql_req_sel_all =
    "SELECT * FROM " + ModelRegistry::model(typeid(PaymentType));

const std::string PaymentType::sql_req_sel_all_as_str =
    "SELECT id, type AS as_str FROM " +
    ModelRegistry::model(typeid(PaymentType));