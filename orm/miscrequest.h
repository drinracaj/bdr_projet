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
    std::string req =
        "WITH RECURSIVE month_sequence AS (\n"
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
        "            WHERE Flat.idBuilding = " + std::to_string(building_id) + "\n"
        "        ) as buildingTotalVolume\n"
        "    FROM feetype ft\n"
        "),\n"
        "watermeter_status AS (\n"
        "   SELECT CASE\n"
        "       WHEN EXISTS (\n"
        "           SELECT 1 FROM watermeter w\n"
        "           JOIN flat_watermeter fw ON fw.idwatermeter = w.id \n"
        "           JOIN flat f ON f.id = fw.idflat\n"
        "           WHERE f.idbuilding = " + std::to_string(building_id) + "\n"
        "           GROUP BY w.id\n"
        "           HAVING COUNT(DISTINCT f.id) = (SELECT COUNT(*) FROM flat WHERE idbuilding = " + std::to_string(building_id) + ")\n"
        "       ) THEN true\n"
        "       ELSE false\n"
        "   END as has_shared_watermeter\n"
        "),\n"
        "water_calculations AS (\n"
        "   SELECT\n"
        "       ws.has_shared_watermeter,\n"
        "       (SELECT price FROM waterprice WHERE year = " + std::to_string(year) + ") as water_price,\n"
        "       COALESCE((\n"
        "           SELECT SUM(amount) \n"
        "           FROM Invoice i\n"
        "           WHERE i.idFeeType = 2 AND i.idBuilding = " + std::to_string(building_id) + " \n"
        "           AND i.date BETWEEN '" + std::to_string(year) + "-07-01' AND '" + std::to_string(year + 1) + "-06-30'\n"
        "       ), 0) as total_water_invoice,\n"
        "       COALESCE((\n"
        "           SELECT SUM(wm.measure)\n"
        "           FROM watermeasurement wm\n"
        "           JOIN watermeter w ON wm.idwatermeter = w.id\n"
        "           JOIN flat_watermeter fw ON w.id = fw.idwatermeter\n"
        "           WHERE wm.year = " + std::to_string(year) + "\n"
        "           AND w.id NOT IN (\n"
        "               SELECT w2.id \n"
        "               FROM watermeter w2\n"
        "               JOIN flat_watermeter fw2 ON fw2.idwatermeter = w2.id\n"
        "               JOIN flat f2 ON f2.id = fw2.idflat\n"
        "               WHERE f2.idbuilding = " + std::to_string(building_id) + "\n"
        "               GROUP BY w2.id\n"
        "               HAVING COUNT(DISTINCT f2.id) = (SELECT COUNT(*) FROM flat WHERE idbuilding = " + std::to_string(building_id) + ")\n"
        "           )\n"
        "       ), 0) as non_splitted_cubic,\n"
        "       (\n"
        "           SELECT SUM(rtf.volume)\n"
        "           FROM RoomType_Flat rtf\n"
        "           JOIN Flat f ON rtf.idFlat = f.id\n"
        "           WHERE f.idBuilding = " + std::to_string(building_id) + "\n"
        "       ) as building_volume\n"
        "   FROM watermeter_status ws\n"
        "),\n"
        "individual_measurements AS (\n"
        "   SELECT \n"
        "       o.id as idOcc,\n"
        "       COALESCE(SUM(wm.measure), 0) as water_measure\n"
        "   FROM Occupancy o\n"
        "   JOIN Flat f ON o.idFlat = f.id\n"
        "   JOIN flat_watermeter fw ON f.id = fw.idflat\n"
        "   JOIN watermeter w ON fw.idwatermeter = w.id\n"
        "   LEFT JOIN watermeasurement wm ON w.id = wm.idwatermeter AND wm.year = " + std::to_string(year) + "\n"
        "   WHERE w.id NOT IN (\n"
        "       SELECT w2.id \n"
        "       FROM watermeter w2\n"
        "       JOIN flat_watermeter fw2 ON fw2.idwatermeter = w2.id\n"
        "       JOIN flat f2 ON f2.id = fw2.idflat\n"
        "       WHERE f2.idbuilding = " + std::to_string(building_id) + "\n"
        "       GROUP BY w2.id\n"
        "       HAVING COUNT(DISTINCT f2.id) = (SELECT COUNT(*) FROM flat WHERE idbuilding = " + std::to_string(building_id) + ")\n"
        "   )\n"
        "   GROUP BY o.id\n"
        "),\n"
        "fees_calculated AS (\n"
        "    SELECT \n"
        "        tc.idOcc,\n"
        "        bt.idFeeType,\n"
        "        CASE \n"
        "            WHEN bt.idFeeType = 1 THEN \n"
        "                ROUND((tc.total_coefficient * (bt.total_invoice / bt.buildingTotalVolume))::numeric, 2)\n"
        "            WHEN bt.idFeeType = 2 AND wc.has_shared_watermeter THEN\n"
        "                ROUND((\n"
        "                    ((wc.total_water_invoice - (wc.water_price * wc.non_splitted_cubic)) * tc.flatTotVolume * tc.nb_months) / \n"
        "                    (wc.building_volume * 12) +\n"
        "                    (wc.water_price * COALESCE(im.water_measure, 0) * tc.nb_months) / 12\n"
        "                )::numeric, 2)\n"
        "            WHEN bt.idFeeType = 2 AND NOT wc.has_shared_watermeter THEN\n"
        "                ROUND((\n"
        "                    SELECT COALESCE(SUM((wm.measure * wc.water_price * tc.nb_months) / 12), 0)\n"
        "                    FROM Occupancy o\n"
        "                    JOIN Flat f ON o.idFlat = f.id\n"
        "                    JOIN flat_watermeter fw ON f.id = fw.idflat\n"
        "                    JOIN watermeter w ON fw.idwatermeter = w.id\n"
        "                    JOIN watermeasurement wm ON w.id = wm.idwatermeter \n"
        "                    WHERE o.id = tc.idOcc\n"
        "                    AND wm.year = " + std::to_string(year) + "\n"
        "                )::numeric, 2)\n"
        "            ELSE \n"
        "                ROUND(((tc.nb_months * tc.flatTotVolume / 12.0) * (bt.total_invoice / bt.buildingTotalVolume))::numeric, 2)\n"
        "        END as fee_amount\n"
        "    FROM total_coefficients tc\n"
        "    CROSS JOIN building_totals bt\n"
        "    CROSS JOIN water_calculations wc\n"
        "    LEFT JOIN individual_measurements im ON tc.idOcc = im.idOcc\n"
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
