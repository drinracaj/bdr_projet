//
// Created by drin on 16/12/2024.
//

#include "db.h"

#include <soci/postgresql/soci-postgresql.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <fstream>

const std::string db::APP_NAME = "rjmmo";
const std::string db::DB_NAME = APP_NAME + ".db";
const std::string db::LOG_FILENAME = APP_NAME + ".log";

#if defined(_WIN32)
const std::filesystem::path db::OS_SPECIFIC_LOCAL_APP_DIR =
    getenv("LOCALAPPDATA") ? getenv("LOCALAPPDATA") : "";
#elif defined(__linux__)
const std::filesystem::path db::OS_SPECIFIC_LOCAL_APP_DIR =
    getenv("XDG_DATA_HOME") ? getenv("XDG_CONFIG_HOME") : getenv("HOME");
#elif defined(__APPLE__) && defined(__MACH__)
const std::filesystem::path db::OS_SPECIFIC_LOCAL_APP_DIR =
    getenv("HOME") ? getenv("HOME") : "";
#endif
const std::filesystem::path db::APP_DIR = OS_SPECIFIC_LOCAL_APP_DIR / APP_NAME;

bool db::init() {
  if (OS_SPECIFIC_LOCAL_APP_DIR.empty()) {
    return init_success;
  }

  if (!exists(APP_DIR)) {
    create_directory(APP_DIR);
  }

  if (const std::ofstream log_file(APP_DIR / LOG_FILENAME, std::ios_base::app);
      !log_file) {
    return init_success;
  }

  try {
    std::string co =
        "dbname=rjmmo "
        "user=rjmmo_user "
        "password=root "
        "host=localhost "
        "port=5432";
    //if the .db file does not exist sqlite3 will create it
    session.open(soci::postgresql, co);
    //    session << "PRAGMA foreign_keys = ON;";
  } catch (const soci::soci_error& e) {
    log(e.what(), true);
    return init_success;
  }

  return init_success = true;
}

void db::close() {
  if (session.is_connected()) {
    session.close();
  }
}
void db::log(std::string_view msg, const bool err) {
  if (std::ofstream log_file(APP_DIR / LOG_FILENAME, std::ios_base::app);
      log_file) {
    time_t now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    log_file << std::put_time(std::localtime(&now), "[%d-%m-%Y %H:%M:%S] ")
             << (err ? "ERROR " : "INFO ") << msg << "\n";
  }
}
