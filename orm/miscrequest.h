//
// Created by drin on 22/01/2025.
//

#ifndef RJMMO_MISCREQUEST_H
#define RJMMO_MISCREQUEST_H

#include "db.h"

#include <optional>
#include <vector>

struct Fees {
  int occ_id;
  double volume;
  double heat_coefficient;
  int nb_month;
  double heat_fee;
  double water_fee;
  double misc_fee;
};

struct Payments {
  int occ_id;
  std::string start_date;
  double rent;
  double fees;
  int ddm;
  std::string date;
  double amount;
  std::string payment_type;
};

class MiscRequest {
 public:
  MiscRequest() = delete;
  ~MiscRequest() = delete;

  static std::optional<std::vector<std::unique_ptr<Fees>>> get_all_fees(
      int building_id, int year) {
    std::string req = "WITH RECURSIVE month_sequence AS (\n"
        "    SELECT \n"
        "        o.id as idOcc,\n"
        "        EXTRACT(MONTH FROM GREATEST(o.startDate, DATE '" + std::to_string(year) + "-07-01'))::int as currMonth,\n"
        "        EXTRACT(YEAR FROM GREATEST(o.startDate, DATE '" + std::to_string(year) + "-07-01'))::int as currYear,\n"
        "        o.endDate,\n"
        "        (\n"
        "            SELECT SUM(rtf.volume)\n"
        "            FROM RoomType_Flat rtf\n"
        "            WHERE rtf.idFlat = o.idFlat\n"
        "        ) AS flatTotVolume\n"
        "    FROM Occupancy o\n"
        "    JOIN Flat f ON o.idFlat = f.id\n"
        "    WHERE f.idBuilding = " + std::to_string(building_id) + "\n"
        "    AND o.startDate < '" + std::to_string(year + 1) + "-07-01'\n"
        "    AND (o.endDate IS NULL OR o.endDate > '" + std::to_string(year) + "-07-01')\n"
        "\n"
        "    UNION ALL\n"
        "\n"
        "    SELECT\n"
        "        idOcc,\n"
        "        CASE WHEN currMonth = 12 THEN 1 ELSE currMonth + 1 END,\n"
        "        CASE WHEN currMonth = 12 THEN currYear + 1 ELSE currYear END,\n"
        "        endDate,\n"
        "        flatTotVolume\n"
        "    FROM month_sequence\n"
        "    WHERE (currYear = " + std::to_string(year) + " OR (currYear = " + std::to_string(year + 1) + " AND currMonth < 6))\n"
        "    AND (endDate IS NULL OR DATE(endDate) > make_date(CASE WHEN currMonth = 12 THEN currYear + 1 ELSE currYear END,\n"
        "        CASE WHEN currMonth = 12 THEN 1 ELSE currMonth + 1 END, 1))\n"
        "),\n"
        "total_coefficients AS (\n"
        "    SELECT \n"
        "        idOcc,\n"
        "        flatTotVolume,\n"
        "        (SUM(h.coefficient) * flatTotVolume / 100) as total_coefficient,\n"
        "        COUNT(*) as nb_months\n"
        "    FROM month_sequence ms\n"
        "    JOIN heatmonthlycoefficient h ON h.month = ms.currMonth\n"
        "    GROUP BY idOcc, flatTotVolume\n"
        "),\n"
        "building_totals AS (\n"
        "    SELECT \n"
        "        ft.id as idFeeType,\n"
        "        ft.type as feeTypeName,\n"
        "        COALESCE((\n"
        "            SELECT SUM(amount)\n"
        "            FROM Invoice i\n"
        "            WHERE i.idFeeType = ft.id\n"
        "            AND i.idBuilding = " + std::to_string(building_id) + "\n"
        "            AND i.date BETWEEN '" + std::to_string(year) + "-07-01' AND '" + std::to_string(year + 1) + "-06-30'\n"
        "        ), 0) as total_invoice,\n"
        "        (\n"
        "            SELECT SUM(RoomType_Flat.volume)\n"
        "            FROM RoomType_Flat\n"
        "            INNER JOIN Flat ON RoomType_Flat.idFlat = Flat.id\n"
        "            WHERE Flat.idBuilding = "+ std::to_string(building_id) +"\n"
        "        ) as buildingTotalVolume\n"
        "    FROM feetype ft\n"
        "),\n"
        "fees_calculated AS (\n"
        "    SELECT \n"
        "        tc.idOcc,\n"
        "        bt.idFeeType,\n"
        "        CASE \n"
        "            WHEN bt.idFeeType = 1 THEN \n"
        "                ROUND((tc.total_coefficient * (bt.total_invoice / bt.buildingTotalVolume))::numeric, 2)\n"
        "            ELSE \n"
        "                ROUND(((tc.nb_months * tc.flatTotVolume / 12.0) * (bt.total_invoice / bt.buildingTotalVolume))::numeric, 2)\n"
        "        END as fee_amount\n"
        "    FROM total_coefficients tc\n"
        "    CROSS JOIN building_totals bt\n"
        ")\n"
        "SELECT \n"
        "    tc.idOcc,\n"
        "    tc.flatTotVolume,\n"
        "    tc.total_coefficient,\n"
        "    tc.nb_months,\n"
        "    MAX(CASE WHEN fc.idFeeType = 1 THEN fc.fee_amount END) as heat_fee,\n"
        "    MAX(CASE WHEN fc.idFeeType = 2 THEN fc.fee_amount END) as water_fee,\n"
        "    MAX(CASE WHEN fc.idFeeType = 3 THEN fc.fee_amount END) as misc_fee\n"
        "FROM total_coefficients tc\n"
        "LEFT JOIN fees_calculated fc ON tc.idOcc = fc.idOcc\n"
        "GROUP BY tc.idOcc, tc.flatTotVolume, tc.total_coefficient, tc.nb_months;";

    try {
      std::vector<std::unique_ptr<Fees>> res;
      for (const soci::rowset rs = db::session.prepare << req;
           const soci::row& row : rs) {
        Fees fees{};
        fees.occ_id = row.get<int>("idocc");
        fees.volume = row.get<double>("flattotvolume");
        fees.heat_coefficient = row.get<double>("total_coefficient");
        fees.nb_month = row.get<int>("nb_months");
        fees.heat_fee = row.get<double>("heat_fee");
        fees.water_fee = row.get<double>("water_fee");
        fees.misc_fee = row.get<double>("misc_fee");

        res.push_back(std::make_unique<Fees>(fees));
      }
      return std::make_optional(std::move(res));
    } catch (const std::exception& e) {
      db::log(e.what(), true);
      return std::nullopt;
    }
  }

  static std::optional<std::vector<std::unique_ptr<Payments>>> get_all_pay(
      int occ_id) {
    std::string req =
        "SELECT Payment.*, Occupancy.*, PaymentType.type "
        "FROM Payment "
        "INNER JOIN Occupancy ON Payment.idOccupancy = Occupancy.id "
        "INNER JOIN PaymentType ON Payment.idPaymentType = PaymentType.id "
        "WHERE Payment.idOccupancy = " +
        std::to_string(occ_id) + ";";

    try {
      std::vector<std::unique_ptr<Payments>> res;
      for (const soci::rowset rs = db::session.prepare << req;
           const soci::row& row : rs) {
        Payments pay{};
        pay.occ_id = row.get<int>("idoccupancy");
        std::ostringstream oss;
        std::tm date = row.get<std::tm>("startdate");
        oss << std::put_time(&date, "%Y-%m-%d");
        pay.start_date = oss.str();
        oss.flush();
        oss.str("");
        oss.clear();
        pay.rent = row.get<double>("rent");
        pay.fees = row.get<double>("fees");
        pay.ddm = row.get<int>("ddm");
        date = row.get<std::tm>("date");
        oss << std::put_time(&date, "%Y-%m-%d");
        pay.date = oss.str();
        pay.amount = row.get<double>("amount");
        pay.payment_type = row.get<std::string>("type");
        res.push_back(std::make_unique<Payments>(pay));
      }
      return std::make_optional(std::move(res));
    } catch (const std::exception& e) {
      db::log(e.what(), true);
      return std::nullopt;
    }
  }

  static std::optional<std::vector<std::pair<int, std::string>>> get_all_occ() {
    static const std::string req =
        "SELECT Occupancy.id, CONCAT(Building.street, ' ', Building.no, ' | ', "
        "Flat.no, ' - ', Occupancy.id) AS occ "
        "FROM Occupancy "
        "INNER JOIN Flat ON Occupancy.idFlat = Flat.id "
        "INNER JOIN Building ON Flat.idBuilding = Building.id";

    try {
      std::vector<std::pair<int, std::string>> res;
      for (const soci::rowset rs = db::session.prepare << req;
           const soci::row& row : rs) {
        res.emplace_back(row.get<int>("id"), row.get<std::string>("occ"));
      }
      return std::make_optional(res);
    } catch (const std::exception& e) {
      db::log(e.what(), true);
      return std::nullopt;
    }
  }
};

#endif  //RJMMO_MISCREQUEST_H
