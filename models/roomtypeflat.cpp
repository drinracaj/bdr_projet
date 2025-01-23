//
// Created by drin on 22/01/2025.
//

#include "roomtypeflat.h"
const Table<RoomTypeFlat>& RoomTypeFlat::table() {
  static const Table<RoomTypeFlat> table = {
      typeid(RoomTypeFlat),
      {Column<RoomTypeFlat>(
           [](const RoomTypeFlat& rtf) -> double { return rtf.volume(); },
           [](RoomTypeFlat& rtf, FieldSetter val) -> void {
             rtf.volume(std::get<double>(val));
           },
           "volume", "Volume", ColumnType::REAL, .0, 9999.99, 2),
       Column<RoomTypeFlat>(
           [](const RoomTypeFlat& rtf) -> FieldGetter {
             return std::cref(rtf.room_type());
           },
           [](RoomTypeFlat& rtf, FieldSetter val) -> void {
             rtf.room_type(std::move(std::get<ForeignKey>(val)));
           },
           "room_type_id", "Room Type", ColumnType::FK, OnDelete::CASCADE,
           typeid(RoomType)),
       Column<RoomTypeFlat>(
           [](const RoomTypeFlat& rtf) -> FieldGetter {
             return std::cref(rtf.flat());
           },
           [](RoomTypeFlat& rtf, FieldSetter val) -> void {
             rtf.flat(std::move(std::get<ForeignKey>(val)));
           },
           "flat_id", "Room Type", ColumnType::FK, OnDelete::CASCADE,
           typeid(Flat))}};
  return table;
}

const std::string RoomTypeFlat::sql_req_sel_all =
    "SELECT RoomTypeFlat.*, CONCAT(Building.street, ' ', Building.no, ' - ', "
    "Flat.no) AS flat_id" +
    FK_SUFFIX + ", RoomType.type AS room_type_id" + FK_SUFFIX +
    " FROM RoomTypeFlat "
    "INNER JOIN Flat ON Flat.id = RoomTypeFlat.flat_id "
    "INNER JOIN Building ON Flat.building_id = Building.id "
    "INNER JOIN RoomType ON RoomType.id = RoomTypeFlat.room_type_id";

const std::string RoomTypeFlat::sql_req_sel_all_as_str;