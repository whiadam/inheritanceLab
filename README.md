# Inheritance & Serialization Lab

## Design Choices

I made `Person` an abstract base class that inherits from a `Serializable` interface. `Student` and `Instructor` both extend `Person` and override the serialization methods. I did it this way so I could store them together in a `vector<unique_ptr<Person>>` and call the right serialize method automatically (polymorphism).

## Where Exceptions Are Thrown

All validation happens in the constructors so you can never create an object in a bad state.

- `Person`: throws `ValidationError` if `id <= 0`, name is empty, or email doesn't match the regex
- `Student`: throws `ValidationError` if `grad_year < 2000`
- `Instructor`: throws `ValidationError` if `office` is empty

There's also a `SerializationError` defined in case serialization ever fails.

## How CSV Escaping Works

The `csv_escape()` function in `csv.hpp` checks if a field contains a comma, double-quote, or newline. If it does, the whole field gets wrapped in double-quotes. Any double-quote character inside the field gets doubled up (`"` becomes `""`). This follows the basic RFC-4180 CSV rules.

Example: `Eve, "The Great"` becomes `"Eve, ""The Great""`

## Build & Run

```bash
mkdir -p build && cd build
cmake -S .. -B . -DCMAKE_BUILD_TYPE=Debug
cmake --build .
ctest --output-on-failure
./app
```
