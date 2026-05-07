Inheritance and Serialization Lab

For this lab, I made Person the main base class. It is abstract and also uses the Serializable interface. Then Student and Instructor both inherit from Person. I did it this way so different people can be stored in the same vector<unique_ptr<Person>>, and the program will still call the correct serialize function for each one.

I put the validation inside the constructors. This means an object should not be created if the data is bad. Person checks for a valid id, name, and email. Student checks that the graduation year is not too old. Instructor checks that the office is not empty. I also made a SerializationError, but it is mostly there in case something goes wrong later with saving or loading data.

For CSV escaping, the csv_escape() function checks if the text has a comma, quote, or new line. If it does, the text gets put inside quotes. If there is already a quote inside the text, it gets doubled so the CSV file can still read it correctly. For example, Eve, The Great turns into Eve, The Great.

To build and run it, I make the build folder, run CMake, build the project, run the tests, and then run the app.


```bash
mkdir -p build && cd build
cmake -S .. -B . -DCMAKE_BUILD_TYPE=Debug
cmake --build .
ctest --output-on-failure
./app
```
