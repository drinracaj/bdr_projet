//
// Created by drin on 22/01/2025.
//

#include "roomtype.h"

const Table<RoomType>& RoomType::table() {
  static const Table<RoomType> table = {
      typeid(RoomType),
      {Column<RoomType>(
           [](const RoomType& rt) -> double { return rt.coefficient(); },
           [](RoomType& rt, FieldSetter val) -> void {
             rt.coefficient(std::get<double>(val));
           },
           "coefficient", "Coefficient", ColumnType::REAL, .0, 1.0, 1),
       Column<RoomType>(
           [](const RoomType& rt) -> std::string_view { return rt.type(); },
           [](RoomType& rt, FieldSetter val) -> void {
             rt.type(std::move(std::get<std::string>(val)));
           },
           "type", "Type", ColumnType::VARCHAR, 20)}};
  return table;
}

const std::string RoomType::sql_req_sel_all =
    "SELECT * FROM " + ModelRegistry::model(typeid(RoomType));

const std::string RoomType::sql_req_sel_all_as_str =
    "SELECT id, type AS as_str FROM " + ModelRegistry::model(typeid(RoomType));