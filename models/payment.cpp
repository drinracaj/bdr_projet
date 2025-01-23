//
// Created by drin on 18/01/2025.
//

#include "payment.h"

#include "occupancy.h"
#include "paymenttype.h"

const Table<Payment>& Payment::table() {
  static const Table<Payment> table = {
      typeid(Payment),
      {Column<Payment>(
           [](const Payment& p) -> std::string_view { return p.date(); },
           [](Payment& p, FieldSetter val) -> void {
             p.date(std::move(std::get<std::string>(val)));
           },
           "date", "Date", ColumnType::DATE),
       Column<Payment>([](const Payment& p) -> int { return p.amount(); },
                       [](Payment& p, FieldSetter val) -> void {
                         p.amount(std::get<int>(val));
                       },
                       "amount", "Amount", ColumnType::INTEGER, 0,
                       std::numeric_limits<int>::max()),
       Column<Payment>(
           [](const Payment& p) -> ForeignKeyRef {
             return std::cref(p.payment_type());
           },
           [](Payment& p, FieldSetter val) -> void {
             p.payment_type(std::move(std::get<ForeignKey>(val)));
           },
           "paymenttype_id", "Payment Type", ColumnType::FK, OnDelete::CASCADE,
           typeid(PaymentType)),
       Column<Payment>(
           [](const Payment& p) -> ForeignKeyRef {
             return std::cref(p.occupancy());
           },
           [](Payment& p, FieldSetter val) -> void {
             p.occupancy(std::move(std::get<ForeignKey>(val)));
           },
           "occupancy_id", "Occupancy", ColumnType::FK, OnDelete::CASCADE,
           typeid(Occupancy))}};
  return table;
}

const std::string Payment::sql_req_sel_all =
    "SELECT Payment.*, PaymentType.type AS "
    "paymenttype_id" +
    FK_SUFFIX +
    ", CONCAT(Building.street, ' ', Building.no, ' - ', Flat.no, ' - ', "
    "Occupancy.id) AS occupancy_id" +
    FK_SUFFIX +
    " FROM Payment INNER JOIN PaymentType ON Payment.paymenttype_id = "
    "PaymentType.id "
    "INNER JOIN Occupancy ON Occupancy.id = Payment.occupancy_id "
    "INNER JOIN Flat ON Occupancy.flat_id = Flat.id "
    "INNER JOIN Building ON Flat.building_id = Building.id";

const std::string Payment::sql_req_sel_all_as_str;
