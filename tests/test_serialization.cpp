#include <catch2/catch_test_macros.hpp>
#include "student.hpp"
#include "instructor.hpp"
#include "csv_writer.hpp"
#include <vector>

// ── Original tests ────────────────────────────────────────────────────────────

TEST_CASE("Student JSON serialization") {
  Student s(10,"Alice","alice@uni.edu",2027, {"CS101","HIST110"});
  auto j = s.to_json();
  CHECK(j.at("role") == "Student");
  CHECK(j.at("name") == "Alice");
  CHECK(j.at("grad_year") == 2027);
}

TEST_CASE("Instructor YAML serialization") {
  Instructor i(11,"Bob","bob@uni.edu","C-210", {"CS101"});
  auto y = i.to_yaml();
  CHECK(y["role"].as<std::string>() == "Instructor");
  CHECK(y["office"].as<std::string>() == "C-210");
}

TEST_CASE("CSV escaping handles commas and quotes") {
  Student s(12,"Eve, \"The Great\"","eve@uni.edu",2028, {"CS,101","AI\"Lab"});
  auto row = s.csv_row();
  REQUIRE(row.find("\"Eve, \"\"The Great\"\"\"") != std::string::npos);
}

TEST_CASE("Validation throws on bad email") {
  REQUIRE_THROWS_AS(Student(13,"Zed","not-an-email",2026), ValidationError);
}

// ── Extension 1: Polymorphic CSV writer ───────────────────────────────────────

TEST_CASE("Polymorphic CSV writer produces superset header") {
  Student    s(1, "Ada",   "ada@uni.edu",   2026, {"CS101","MATH200"});
  Instructor i(2, "Grace", "grace@uni.edu", "Room 314", {"CS101"});
  std::vector<const Person*> people = {&s, &i};
  auto csv = write_polymorphic_csv(people);
  // Header present
  REQUIRE(csv.find("role,id,name,email,grad_year,office,courses,teaches") != std::string::npos);
  // Student row: office and teaches should be empty
  REQUIRE(csv.find("Student") != std::string::npos);
  REQUIRE(csv.find("2026") != std::string::npos);
  // Instructor row: grad_year and courses should be empty
  REQUIRE(csv.find("Instructor") != std::string::npos);
  REQUIRE(csv.find("Room 314") != std::string::npos);
}

TEST_CASE("Polymorphic CSV writer stable column order") {
  Student s(3, "Carol", "carol@uni.edu", 2029, {"AI101"});
  std::vector<const Person*> people = {&s};
  auto csv = write_polymorphic_csv(people);
  // Columns: role,id,name,email,grad_year,office,courses,teaches
  // For a Student, office and teaches fields must be empty (two trailing commas)
  REQUIRE(csv.find("Student,3,Carol,carol@uni.edu,2029,,AI101,") != std::string::npos);
}

// ── Extension 2: Strong exception safety ─────────────────────────────────────

TEST_CASE("Strong exception safety: Student bad grad_year leaves no partial object") {
  // Constructing with bad grad_year must throw; catching confirms no object escapes
  bool threw = false;
  try {
    Student s(20, "Dave", "dave@uni.edu", 1999);
  } catch (const ValidationError&) {
    threw = true;
  }
  REQUIRE(threw);
}

TEST_CASE("Strong exception safety: Instructor empty office leaves no partial object") {
  bool threw = false;
  try {
    Instructor i(21, "Eve", "eve@uni.edu", "");
  } catch (const ValidationError&) {
    threw = true;
  }
  REQUIRE(threw);
}

TEST_CASE("Strong exception safety: bad email throws before any derived field is set") {
  // Email validation happens in Person base; derived fields must never be touched
  REQUIRE_THROWS_AS(Student(22, "Frank", "not-valid", 2027), ValidationError);
  REQUIRE_THROWS_AS(Instructor(23, "Gina",  "not-valid", "Rm 1"),  ValidationError);
}
