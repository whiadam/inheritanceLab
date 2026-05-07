#pragma once
#include "person.hpp"
#include "csv.hpp"
#include <vector>

class Instructor : public Person {
  std::string office_;
  std::vector<std::string> teaches_;
public:
  // Strong exception safety: office validated inline in the initializer list
  // via an immediately-invoked lambda. teaches_ is only moved after office_
  // is confirmed valid. If the lambda throws, the object is never partially
  // constructed and no resources are leaked.
  Instructor(int id, std::string name, std::string email,
             std::string office, std::vector<std::string> teaches = {})
    : Person(id, std::move(name), std::move(email)),
      office_([&]{
        if (office.empty()) throw ValidationError("office is required");
        return std::move(office);
      }()),
      teaches_(std::move(teaches)) {}

  std::string role() const override { return "Instructor"; }

  nlohmann::json to_json() const override {
    return {
      {"role", role()},
      {"id", id_},
      {"name", name_},
      {"email", email_},
      {"office", office_},
      {"teaches", teaches_}
    };
  }

  YAML::Node to_yaml() const override {
    YAML::Node n;
    n["role"] = role();
    n["id"] = id_;
    n["name"] = name_;
    n["email"] = email_;
    n["office"] = office_;
    for (const auto& c : teaches_) n["teaches"].push_back(c);
    return n;
  }

  std::string csv_header() const override {
    return "role,id,name,email,office,teaches";
  }

  std::string csv_row() const override {
    std::string joined;
    for (size_t i = 0; i < teaches_.size(); ++i) {
      if (i) joined += ';';
      joined += teaches_[i];
    }
    return csv_escape(role()) + "," + std::to_string(id_) + "," +
           csv_escape(name_) + "," + csv_escape(email_) + "," +
           csv_escape(office_) + "," + csv_escape(joined);
  }
};
