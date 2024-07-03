#include <iostream>
#include <string>
#include <variant>
#include <stdexcept>

enum class PY_OJ_Type { INT, FLOAT, CHAR, STRING };

struct PY_OJ {
    union {
        int i;
        float f;
        char c;
        std::string* s;  // Use a pointer for string
    };
    PY_OJ_Type active_type;

    PY_OJ() : i(0), active_type(PY_OJ_Type::INT) {}  // Default constructor
    
    // Constructor for string
    PY_OJ(const std::string& str) : s(new std::string(str)), active_type(PY_OJ_Type::STRING) {}
    
    // Destructor to handle string deletion
    ~PY_OJ() {
        if (active_type == PY_OJ_Type::STRING) {
            delete s;
        }
    }
};

std::variant<int, float, char, std::string> type_inference(const PY_OJ& obj) {
    switch(obj.active_type) {
        case PY_OJ_Type::INT: return obj.i;
        case PY_OJ_Type::FLOAT: return obj.f;
        case PY_OJ_Type::CHAR: return obj.c;
        case PY_OJ_Type::STRING: return *obj.s;
        default: throw std::runtime_error("Unknown type");
    }
}

std::variant<int, float, std::string> PY_ADD(const PY_OJ& a, const PY_OJ& b) {
    auto type_a = type_inference(a);
    auto type_b = type_inference(b);

    auto to_string = [](const auto& var) -> std::string {
        if (std::holds_alternative<std::string>(var)) return std::get<std::string>(var);
        if (std::holds_alternative<char>(var)) return std::string(1, std::get<char>(var));
        if (std::holds_alternative<int>(var)) return std::to_string(std::get<int>(var));
        if (std::holds_alternative<float>(var)) return std::to_string(std::get<float>(var));
        throw std::runtime_error("Unsupported type");
    };

    if (std::holds_alternative<std::string>(type_a) || std::holds_alternative<std::string>(type_b) ||
        std::holds_alternative<char>(type_a) || std::holds_alternative<char>(type_b)) {
        return to_string(type_a) + to_string(type_b);
    } else if (std::holds_alternative<float>(type_a) || std::holds_alternative<float>(type_b)) {
        float a_val = std::holds_alternative<float>(type_a) ? std::get<float>(type_a) : std::get<int>(type_a);
        float b_val = std::holds_alternative<float>(type_b) ? std::get<float>(type_b) : std::get<int>(type_b);
        return a_val + b_val;
    } else {
        return std::get<int>(type_a) + std::get<int>(type_b);
    }
}

// Overloaded PY_ADD functions for basic types
std::variant<int, float, std::string> PY_ADD(int a, int b) {
    return a + b;
}

std::variant<int, float, std::string> PY_ADD(float a, float b) {
    return a + b;
}

std::variant<int, float, std::string> PY_ADD(const std::string& a, const std::string& b) {
    return a + b;
}

std::variant<int, float, std::string> PY_ADD(int a, float b) {
    return static_cast<float>(a) + b;
}

std::variant<int, float, std::string> PY_ADD(float a, int b) {
    return a + static_cast<float>(b);
}

std::variant<int, float, std::string> PY_ADD(const std::string& a, int b) {
    return a + std::to_string(b);
}

std::variant<int, float, std::string> PY_ADD(int a, const std::string& b) {
    return std::to_string(a) + b;
}

std::variant<int, float, std::string> PY_ADD(const std::string& a, float b) {
    return a + std::to_string(b);
}

std::variant<int, float, std::string> PY_ADD(float a, const std::string& b) {
    return std::to_string(a) + b;
}

std::variant<int, float, std::string> PY_MULT(const PY_OJ& a, const PY_OJ& b) {
    auto type_a = type_inference(a);
    auto type_b = type_inference(b);
    
    if (std::holds_alternative<std::string>(type_a) && std::holds_alternative<int>(type_b)) {
        std::string str = std::get<std::string>(type_a);
        int repeats = std::get<int>(type_b);
        std::string result;
        for (int i = 0; i < repeats; ++i) {
            result += str;
        }
        return result;
    } else if (std::holds_alternative<int>(type_a) && std::holds_alternative<std::string>(type_b)) {
        std::string str = std::get<std::string>(type_b);
        int repeats = std::get<int>(type_a);
        std::string result;
        for (int i = 0; i < repeats; ++i) {
            result += str;
        }
        return result;
    } else if (std::holds_alternative<float>(type_a) || std::holds_alternative<float>(type_b)) {
        float a_val = std::holds_alternative<float>(type_a) ? std::get<float>(type_a) : 
                      (std::holds_alternative<int>(type_a) ? std::get<int>(type_a) : 
                      static_cast<float>(std::get<char>(type_a)));
        float b_val = std::holds_alternative<float>(type_b) ? std::get<float>(type_b) : 
                      (std::holds_alternative<int>(type_b) ? std::get<int>(type_b) : 
                      static_cast<float>(std::get<char>(type_b)));
        return a_val * b_val;
    } else if (std::holds_alternative<int>(type_a) && std::holds_alternative<int>(type_b)) {
        return std::get<int>(type_a) * std::get<int>(type_b);
    } else if (std::holds_alternative<char>(type_a) && std::holds_alternative<char>(type_b)) {
        return static_cast<int>(std::get<char>(type_a)) * static_cast<int>(std::get<char>(type_b));
    } else {
        throw std::runtime_error("Unsupported types for multiplication");
    }
}

// Overloaded PY_MULT functions for basic types
std::variant<int, float, std::string> PY_MULT(int a, int b) {
    return a * b;
}

std::variant<int, float, std::string> PY_MULT(float a, float b) {
    return a * b;
}

std::variant<int, float, std::string> PY_MULT(const std::string& a, int b) {
    std::string result;
    for (int i = 0; i < b; ++i) {
        result += a;
    }
    return result;
}

std::variant<int, float, std::string> PY_MULT(int a, const std::string& b) {
    return PY_MULT(b, a);  // Reuse the string * int implementation
}

std::variant<int, float, std::string> PY_MULT(int a, float b) {
    return static_cast<float>(a) * b;
}

std::variant<int, float, std::string> PY_MULT(float a, int b) {
    return a * static_cast<float>(b);
}

void print_result(const std::variant<int, float, std::string>& result) {
    if (std::holds_alternative<int>(result)) {
        std::cout << "Int result: " << std::get<int>(result) << std::endl;
    } else if (std::holds_alternative<float>(result)) {
        std::cout << "Float result: " << std::get<float>(result) << std::endl;
    } else {
        std::cout << "String result: " << std::get<std::string>(result) << std::endl;
    }
}

int main() {
    PY_OJ obj1, obj2;

    // Example with integers
    obj1.i = 5;
    obj1.active_type = PY_OJ_Type::INT;
    obj2.i = 10;
    obj2.active_type = PY_OJ_Type::INT;
    print_result(PY_ADD(obj1, obj2));

    // Example with mixed types (int and float)
    obj1.i = 5;
    obj1.active_type = PY_OJ_Type::INT;
    obj2.f = 2.86f;
    obj2.active_type = PY_OJ_Type::FLOAT;
    print_result(PY_ADD(obj1, obj2));

    // Example with floats
    obj1.f = 3.14f;
    obj1.active_type = PY_OJ_Type::FLOAT;
    obj2.f = 2.86f;
    obj2.active_type = PY_OJ_Type::FLOAT;
    print_result(PY_ADD(obj1, obj2));

    // Example with chars
    obj1.c = 'A';
    obj1.active_type = PY_OJ_Type::CHAR;
    obj2.c = 'B';
    obj2.active_type = PY_OJ_Type::CHAR;
    print_result(PY_ADD(obj1, obj2));

    // Example with strings
    PY_OJ str1("Hello");
    PY_OJ str2("World");
    print_result(PY_ADD(str1, str2));

        // Example with string and int (Python-like string multiplication)
    obj2.i = 3;
    obj2.active_type = PY_OJ_Type::INT;
    print_result(PY_MULT(str1, obj2));

    // Example with int and string (reversed order)
    obj1.i = 2;
    obj1.active_type = PY_OJ_Type::INT;
    print_result(PY_MULT(obj1, str2));

    // New examples with basic types
    print_result(PY_ADD(5, 10));
    print_result(PY_ADD(3.14f, 2.86f));
    print_result(PY_ADD(std::string("Hello "), std::string("World")));
    print_result(PY_ADD(5, 2.5f));
    print_result(PY_ADD(std::string("Number "), 42));

    print_result(PY_MULT(5, 10));
    print_result(PY_MULT(3.14f, 2.0f));
    print_result(PY_MULT(std::string("Hello "), 3));
    print_result(PY_MULT(2, std::string("World ")));
    print_result(PY_MULT(5, 2.5f));

    return 0;
}