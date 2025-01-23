//
// Created by drin on 16/12/2024.
//

#ifndef RJMMO_DB_H
#define RJMMO_DB_H

#include "column.h"
#include "modelregistry.h"

#include <soci/soci.h>
#include <filesystem>
#include <string_view>

class db {
 public:
  static inline bool init_success = false;
  static inline soci::session session;
  db() = delete;
  ~db() = delete;
  static bool init();
  static void close();
  static void log(std::string_view msg, bool err = false);
  template <typename T>
  static bool create_sql_table() {
    std::ostringstream os;
    std::ostringstream os_fk;
    os << "CREATE TABLE IF NOT EXISTS "
       << ModelRegistry::model(T::table().table_index) << "("
       << "id serial PRIMARY KEY,";
    for (const auto& col : T::table().columns) {
      if (col.type() == ColumnType::FK) {
        os << col.name() << " INTEGER NOT NULL,";
        os_fk << column_type_str(col.type()) << "(" << col.name()
              << ") REFERENCES " << ModelRegistry::model(col.fk_ref())
              << "(id) ON DELETE " << od_str(col.od()) << ",";
      } else {
        os << col.name() << " " << column_type_str(col.type());
        if (col.vlen() != 0) {
          os << "(" << col.vlen() << ")";
        }
        os << " " << (!col.nullable() ? "NOT " : "") << "NULL"
           << ",";
      }
    }

    if (!os_fk.str().empty()) {
      os << os_fk.str();
    }

    if (std::string tmp = os.str(); tmp.back() == ',') {
      tmp.pop_back();
      os.str("");
      os.clear();
      os << tmp << ");";
    }

    try {
      session << os.str();
    } catch (const soci::soci_error& e) {
      log(e.what(), true);
      return false;
    }
    return true;
  }

 private:
  static const std::string APP_NAME;
  static const std::string DB_NAME;
  static const std::string LOG_FILENAME;
  static const std::filesystem::path OS_SPECIFIC_LOCAL_APP_DIR;
  static const std::filesystem::path APP_DIR;
};

#endif  //RJMMO_DB_H
