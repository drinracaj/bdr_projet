//
// Created by drin on 22/01/2025.
//

#include "roomtypeflat.h"
const Table<RoomType_Flat>& RoomType_Flat::table() {
  static const Table<RoomType_Flat> table = {
      typeid(RoomType_Flat),
      {Column<RoomType_Flat>(
           [](const RoomType_Flat& rtf) -> double { return rtf.volume(); },
           [](RoomType_Flat& rtf, FieldSetter val) -> void {
             rtf.volume(std::get<double>(val));
           },
           "volume", "Volume", ColumnType::REAL, .0, 9999.99, 2),
       Column<RoomType_Flat>(
           [](const RoomType_Flat& rtf) -> FieldGetter {
             return std::cref(rtf.room_type());
           },
           [](RoomType_Flat& rtf, FieldSetter val) -> void {
             rtf.room_type(std::move(std::get<ForeignKey>(val)));
           },
           "idroomtype", "Room Type", ColumnType::FK, OnDelete::CASCADE,
           typeid(RoomType)),
       Column<RoomType_Flat>(
           [](const RoomType_Flat& rtf) -> FieldGetter {
             return std::cref(rtf.flat());
           },
           [](RoomType_Flat& rtf, FieldSetter val) -> void {
             rtf.flat(std::move(std::get<ForeignKey>(val)));
           },
           "idflat", "Flat", ColumnType::FK, OnDelete::CASCADE,
           typeid(Flat))}};
  return table;
}

const std::string RoomType_Flat::sql_req_sel_all =
    "SELECT RoomType_Flat.*, CONCAT(Building.street, ' ', Building.no, ' - ', "
    "Flat.no) AS idFlat" +
    FK_SUFFIX + ", RoomType.type AS idRoomType" + FK_SUFFIX +
    " FROM RoomType_Flat "
    "INNER JOIN Flat ON RoomType_Flat.idFlat = Flat.id "
    "INNER JOIN RoomType ON RoomType_Flat.idRoomType = RoomType.id "
    "INNER JOIN Building ON Flat.idBuilding = Building.id";

const std::string RoomType_Flat::sql_req_sel_all_as_str;