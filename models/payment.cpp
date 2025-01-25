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
       Column<Payment>([](const Payment& p) -> double { return p.amount(); },
                       [](Payment& p, FieldSetter val) -> void {
                         p.amount(std::get<double>(val));
                       },
                       "amount", "Amount", ColumnType::REAL, .0,
                       1'000'000'000.0, 2),
       Column<Payment>(
           [](const Payment& p) -> ForeignKeyRef {
             return std::cref(p.payment_type());
           },
           [](Payment& p, FieldSetter val) -> void {
             p.payment_type(std::move(std::get<ForeignKey>(val)));
           },
           "idpaymenttype", "Payment Type", ColumnType::FK, OnDelete::CASCADE,
           typeid(PaymentType)),
       Column<Payment>(
           [](const Payment& p) -> ForeignKeyRef {
             return std::cref(p.occupancy());
           },
           [](Payment& p, FieldSetter val) -> void {
             p.occupancy(std::move(std::get<ForeignKey>(val)));
           },
           "idoccupancy", "Occupancy", ColumnType::FK, OnDelete::CASCADE,
           typeid(Occupancy))}};
  return table;
}

const std::string Payment::sql_req_sel_all =
    "SELECT Payment.*, PaymentType.type AS idPaymentType" +
    FK_SUFFIX +
    ", CONCAT(Building.street, ' ', Building.no, ' - ', Flat.no, ' | ', "
    "Occupancy.id) AS idOccupancy" +
    FK_SUFFIX +
    " FROM Payment "
    "INNER JOIN PaymentType ON Payment.idPaymentType = PaymentType.id "
    "INNER JOIN Occupancy ON Payment.idOccupancy = Occupancy.id "
    "INNER JOIN Flat ON Occupancy.idFlat = Flat.id "
    "INNER JOIN Building ON Flat.idBuilding = Building.id";

const std::string Payment::sql_req_sel_all_as_str;
