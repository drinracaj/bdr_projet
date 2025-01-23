//
// Created by drin on 22/01/2025.
//

#ifndef RJMMO_ROOMTYPEFLAT_H
#define RJMMO_ROOMTYPEFLAT_H

#include "model.h"

#include "flat.h"
#include "roomtype.h"

class RoomTypeFlat : public Model<RoomTypeFlat> {
 public:
  using Model::Model;

  static const Table<RoomTypeFlat>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  //getters
  [[nodiscard]] double volume() const { return volume_; };
  [[nodiscard]] const ForeignKey& room_type() const { return room_type_; };
  [[nodiscard]] const ForeignKey& flat() const { return flat_; };

  //setters
  void volume(double val) { volume_ = val; }
  void room_type(ForeignKey val) { room_type_ = std::move(val); }
  void flat(ForeignKey val) { flat_ = std::move(val); }

 private:
  double volume_;
  ForeignKey room_type_;
  ForeignKey flat_;
};

REGISTER_MODEL(RoomTypeFlat);

#endif  //RJMMO_ROOMTYPEFLAT_H
