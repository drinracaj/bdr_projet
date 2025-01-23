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

class MiscRequest {
 public:
  MiscRequest() = delete;
  ~MiscRequest() = delete;

  static std::optional<std::vector<std::unique_ptr<Fees>>> get_all_fees(
      int building_id, int year) {
    std::string req = "WITH RECURSIVE month_sequence AS (\n"
    "    SELECT \n"
     "        o.id as occupancy_id,\n"
     "        EXTRACT(MONTH FROM GREATEST(o.start, DATE '" + std::to_string(year) + "-07-01'))::int as current_month,\n"
     "        EXTRACT(YEAR FROM GREATEST(o.start, DATE '" + std::to_string(year) + "-07-01'))::int as current_year,\n"
     "        o.end_date,\n"
     "        (\n"
     "            SELECT SUM(rtf.volume)\n"
     "            FROM RoomTypeFlat rtf\n"
     "            WHERE rtf.flat_id = o.flat_id\n"
     "        ) AS totVolume\n"
     "    FROM Occupancy o\n"
     "    JOIN Flat f ON o.flat_id = f.id\n"
     "    WHERE f.building_id = " + std::to_string(building_id) + "\n"
     "    AND o.start < '" + std::to_string(year + 1) + "-07-01' \n"
     "    AND (o.end_date IS NULL OR o.end_date > '" + std::to_string(year) + "-07-01')\n"
     "\n"
     "    UNION ALL\n"
     "\n"
     "    SELECT \n"
     "        occupancy_id,\n"
     "        CASE WHEN current_month = 12 THEN 1 ELSE current_month + 1 END,\n"
     "        CASE WHEN current_month = 12 THEN current_year + 1 ELSE current_year END,\n"
     "        end_date,\n"
     "        totVolume\n"
     "    FROM month_sequence\n"
     "    WHERE (current_year = " + std::to_string(year) + " OR (current_year = " + std::to_string(year + 1) + " AND current_month < 6))\n"
     "    AND (end_date IS NULL OR DATE(end_date) > make_date(CASE WHEN current_month = 12 THEN current_year + 1 ELSE current_year END, \n"
     "        CASE WHEN current_month = 12 THEN 1 ELSE current_month + 1 END, 1))\n"
     "),\n"
   "total_coefficients AS (\n"
   "    SELECT \n"
   "        occupancy_id,\n"
   "        totVolume,\n"
   "        (SUM(h.coefficient) * totVolume / 100) as total_coefficient,\n"
   "        COUNT(*) as nb_months\n"
   "    FROM month_sequence ms\n"
   "    JOIN heatmonthlycoefficient h ON h.month = ms.current_month\n"
   "    GROUP BY occupancy_id, totVolume\n"
   "),\n"
   "building_totals AS (\n"
   "    SELECT \n"
   "        ft.id as fee_type_id,\n"
   "        ft.type as fee_type_name,\n"
   "        COALESCE((\n"
   "            SELECT SUM(amount)\n"
   "            FROM Invoice i\n"
   "            WHERE i.fee_type_id = ft.id\n"
   "            AND i.building_id = " + std::to_string(building_id) + "\n"
   "            AND i.date BETWEEN '" + std::to_string(year) + "-07-01' AND '" + std::to_string(year + 1) + "-06-30'\n"
   "        ), 0) as total_invoice,\n"
   "        (\n"
   "            SELECT SUM(RoomTypeFlat.volume)\n"
   "            FROM RoomTypeFlat\n"
   "            INNER JOIN Flat ON RoomTypeFlat.flat_id = Flat.id\n"
   "            WHERE Flat.building_id = " + std::to_string(building_id) + "\n"
   "        ) as totalVolume\n"
   "    FROM feetype ft\n"
   "),\n"
   "fees_calculated AS (\n"
   "    SELECT \n"
   "        tc.occupancy_id,\n"
   "        bt.fee_type_id,\n"
   "        CASE \n"
   "            WHEN bt.fee_type_id = 1 THEN \n"
   "                ROUND((tc.total_coefficient * (bt.total_invoice / bt.totalVolume))::numeric, 2)\n"
   "            ELSE \n"
   "                ROUND(((tc.nb_months * tc.totVolume / 12.0) * (bt.total_invoice / bt.totalVolume))::numeric, 2)\n"
   "        END as fee_amount\n"
   "    FROM total_coefficients tc\n"
   "    CROSS JOIN building_totals bt\n"
   ")\n"
   "SELECT \n"
   "    tc.occupancy_id,\n"
   "    tc.totVolume,\n"
   "    tc.total_coefficient,\n"
   "    tc.nb_months,\n"
   "    MAX(CASE WHEN fc.fee_type_id = 1 THEN fc.fee_amount END) as heat_fee,\n"
   "    MAX(CASE WHEN fc.fee_type_id = 2 THEN fc.fee_amount END) as water_fee,\n"
   "    MAX(CASE WHEN fc.fee_type_id = 3 THEN fc.fee_amount END) as misc_fee\n"
   "FROM total_coefficients tc\n"
   "LEFT JOIN fees_calculated fc ON tc.occupancy_id = fc.occupancy_id\n"
   "GROUP BY tc.occupancy_id, tc.totVolume, tc.total_coefficient, tc.nb_months;";

    try {
      std::vector<std::unique_ptr<Fees>> res;
      for (const soci::rowset rs = db::session.prepare << req;
           const soci::row& row : rs) {
        Fees fees{};
        fees.occ_id = row.get<int>("occupancy_id");
        fees.volume = row.get<double>("totvolume");
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
};

#endif  //RJMMO_MISCREQUEST_H
