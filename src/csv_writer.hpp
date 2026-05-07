#pragma once
#include "person.hpp"
#include "csv.hpp"
#include <span>
#include <string>
#include <sstream>

// Extension 1: Polymorphic CSV writer
// Emits a single CSV with superset header regardless of concrete Person type.
// Missing fields are filled with empty strings. Column order is stable.
//
// Superset header: role,id,name,email,grad_year,office,courses,teaches

inline std::string write_polymorphic_csv(std::span<const Person* const> people) {
    std::ostringstream out;
    out << "role,id,name,email,grad_year,office,courses,teaches\n";
    for (const Person* p : people) {
        if (!p) continue;
        auto j = p->to_json();
        auto get = [&](const std::string& key) -> std::string {
            if (!j.contains(key)) return "";
            const auto& v = j.at(key);
            if (v.is_string()) return v.get<std::string>();
            if (v.is_number()) return std::to_string(v.get<int>());
            if (v.is_array()) {
                std::string joined;
                for (size_t i = 0; i < v.size(); ++i) {
                    if (i) joined += ';';
                    joined += v[i].get<std::string>();
                }
                return joined;
            }
            return "";
        };
        out << csv_escape(get("role"))     << ","
            << csv_escape(get("id"))       << ","
            << csv_escape(get("name"))     << ","
            << csv_escape(get("email"))    << ","
            << csv_escape(get("grad_year")) << ","
            << csv_escape(get("office"))   << ","
            << csv_escape(get("courses"))  << ","
            << csv_escape(get("teaches"))  << "\n";
    }
    return out.str();
}
