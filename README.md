# C++ Inheritance & Exceptions Lab

A small class hierarchy with basic → advanced inheritance, robust validation via exceptions, and three serializers (JSON, CSV, YAML). Unit tests are written with Catch2 and built/tested with CMake.

## Project Layout

```
.
├─ CMakeLists.txt
├─ src/
│  ├─ exceptions.hpp
│  ├─ serializable.hpp
│  ├─ csv.hpp
│  ├─ person.hpp        person.cpp
│  ├─ student.hpp       student.cpp
│  ├─ instructor.hpp    instructor.cpp
│  └─ main.cpp
└─ tests/
   └─ test_serialization.cpp
```

## Design Choices

- **`Person`** is an abstract base class implementing `Serializable`. It owns `id`, `name`, and `email` with validation enforced in the constructor.
- **`Student`** and **`Instructor`** are concrete derived classes that each override all four serialization methods (`to_json`, `to_yaml`, `csv_header`, `csv_row`).
- **Exceptions are thrown** in constructors for invalid `id` (≤ 0), empty `name`, malformed `email` (regex check), `grad_year < 2000`, and empty `office`.
- **CSV escaping** (`csv_escape`) wraps fields containing commas, double-quotes, or newlines in double-quotes and doubles any internal double-quote characters per RFC-4180.

## Build & Test

```bash
mkdir -p build && cd build
cmake -S .. -B . -DCMAKE_BUILD_TYPE=Debug
cmake --build .
ctest --output-on-failure
./app
```

## Dependencies (auto-fetched via CMake FetchContent)

- [nlohmann/json](https://github.com/nlohmann/json) v3.11.3
- [yaml-cpp](https://github.com/jbeder/yaml-cpp) 0.8.0
- [Catch2](https://github.com/catchorg/Catch2) v3.6.0
