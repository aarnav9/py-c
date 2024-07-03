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
        std::string* s;
    };
    PY_OJ_Type active_type;

    PY_OJ() : i(0), active_type(PY_OJ_Type::INT) {}
    PY_OJ(int val) : i(val), active_type(PY_OJ_Type::INT) {}
    PY_OJ(float val) : f(val), active_type(PY_OJ_Type::FLOAT) {}
    PY_OJ(char val) : c(val), active_type(PY_OJ_Type::CHAR) {}
    PY_OJ(const std::string& val) : s(new std::string(val)), active_type(PY_OJ_Type::STRING) {}

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

std::string to_string(const std::variant<int, float, char, std::string>& var) {
    return std::visit([](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            return std::to_string(arg);
        } else if constexpr (std::is_same_v<T, float>) {
            return std::to_string(arg);
        } else if constexpr (std::is_same_v<T, char>) {
            return std::string(1, arg);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return arg;
        }
    }, var);
}

PY_OJ PY_ADD(const PY_OJ& a, const PY_OJ& b) {
    auto type_a = type_inference(a);
    auto type_b = type_inference(b);

    if (std::holds_alternative<std::string>(type_a) || std::holds_alternative<std::string>(type_b) ||
        std::holds_alternative<char>(type_a) || std::holds_alternative<char>(type_b)) {
        return PY_OJ(to_string(type_a) + to_string(type_b));
    } else if (std::holds_alternative<float>(type_a) || std::holds_alternative<float>(type_b)) {
        float a_val = std::holds_alternative<float>(type_a) ? std::get<float>(type_a) : std::get<int>(type_a);
        float b_val = std::holds_alternative<float>(type_b) ? std::get<float>(type_b) : std::get<int>(type_b);
        return PY_OJ(a_val + b_val);
    } else {
        return PY_OJ(std::get<int>(type_a) + std::get<int>(type_b));
    }
}

PY_OJ PY_SUB(const PY_OJ& a, const PY_OJ& b) {
    auto type_a = type_inference(a);
    auto type_b = type_inference(b);

    if (std::holds_alternative<float>(type_a) || std::holds_alternative<float>(type_b)) {
        float a_val = std::holds_alternative<float>(type_a) ? std::get<float>(type_a) : 
                      (std::holds_alternative<int>(type_a) ? std::get<int>(type_a) : 
                      static_cast<float>(std::get<char>(type_a)));
        float b_val = std::holds_alternative<float>(type_b) ? std::get<float>(type_b) : 
                      (std::holds_alternative<int>(type_b) ? std::get<int>(type_b) : 
                      static_cast<float>(std::get<char>(type_b)));
        return PY_OJ(a_val - b_val);
    } else if (std::holds_alternative<int>(type_a) && std::holds_alternative<int>(type_b)) {
        return PY_OJ(std::get<int>(type_a) - std::get<int>(type_b));
    } else if (std::holds_alternative<char>(type_a) && std::holds_alternative<char>(type_b)) {
        return PY_OJ(static_cast<int>(std::get<char>(type_a)) - static_cast<int>(std::get<char>(type_b)));
    } else {
        throw std::runtime_error("Unsupported types for subtraction");
    }
}

PY_OJ PY_MULT(const PY_OJ& a, const PY_OJ& b) {
    auto type_a = type_inference(a);
    auto type_b = type_inference(b);
    
    if (std::holds_alternative<std::string>(type_a) && std::holds_alternative<int>(type_b)) {
        std::string str = std::get<std::string>(type_a);
        int repeats = std::get<int>(type_b);
        std::string result;
        for (int i = 0; i < repeats; ++i) {
            result += str;
        }
        return PY_OJ(result);
    } else if (std::holds_alternative<int>(type_a) && std::holds_alternative<std::string>(type_b)) {
        std::string str = std::get<std::string>(type_b);
        int repeats = std::get<int>(type_a);
        std::string result;
        for (int i = 0; i < repeats; ++i) {
            result += str;
        }
        return PY_OJ(result);
    } else if (std::holds_alternative<float>(type_a) || std::holds_alternative<float>(type_b)) {
        float a_val = std::holds_alternative<float>(type_a) ? std::get<float>(type_a) : 
                      (std::holds_alternative<int>(type_a) ? std::get<int>(type_a) : 
                      static_cast<float>(std::get<char>(type_a)));
        float b_val = std::holds_alternative<float>(type_b) ? std::get<float>(type_b) : 
                      (std::holds_alternative<int>(type_b) ? std::get<int>(type_b) : 
                      static_cast<float>(std::get<char>(type_b)));
        return PY_OJ(a_val * b_val);
    } else if (std::holds_alternative<int>(type_a) && std::holds_alternative<int>(type_b)) {
        return PY_OJ(std::get<int>(type_a) * std::get<int>(type_b));
    } else if (std::holds_alternative<char>(type_a) && std::holds_alternative<char>(type_b)) {
        return PY_OJ(static_cast<int>(std::get<char>(type_a)) * static_cast<int>(std::get<char>(type_b)));
    } else {
        throw std::runtime_error("Unsupported types for multiplication");
    }
}

PY_OJ PY_DIV(const PY_OJ& a, const PY_OJ& b) {
    auto type_a = type_inference(a);
    auto type_b = type_inference(b);

    if (std::holds_alternative<float>(type_a) || std::holds_alternative<float>(type_b)) {
        float a_val = std::holds_alternative<float>(type_a) ? std::get<float>(type_a) : 
                      (std::holds_alternative<int>(type_a) ? std::get<int>(type_a) : 
                      static_cast<float>(std::get<char>(type_a)));
        float b_val = std::holds_alternative<float>(type_b) ? std::get<float>(type_b) : 
                      (std::holds_alternative<int>(type_b) ? std::get<int>(type_b) : 
                      static_cast<float>(std::get<char>(type_b)));
        if (b_val == 0) {
            throw std::runtime_error("Division by zero");
        }
        return PY_OJ(a_val / b_val);
    } else if (std::holds_alternative<int>(type_a) && std::holds_alternative<int>(type_b)) {
        int b_val = std::get<int>(type_b);
        if (b_val == 0) {
            throw std::runtime_error("Division by zero");
        }
        return PY_OJ(static_cast<float>(std::get<int>(type_a)) / static_cast<float>(b_val));
    } else {
        throw std::runtime_error("Unsupported types for division");
    }
}

template<typename Op>
bool PY_COMPARE(const PY_OJ& a, Op op, const PY_OJ& b) {
    auto type_a = type_inference(a);
    auto type_b = type_inference(b);

    if (std::holds_alternative<float>(type_a) || std::holds_alternative<float>(type_b)) {
        float a_val = std::holds_alternative<float>(type_a) ? std::get<float>(type_a) : 
                      (std::holds_alternative<int>(type_a) ? std::get<int>(type_a) : 
                      static_cast<float>(std::get<char>(type_a)));
        float b_val = std::holds_alternative<float>(type_b) ? std::get<float>(type_b) : 
                      (std::holds_alternative<int>(type_b) ? std::get<int>(type_b) : 
                      static_cast<float>(std::get<char>(type_b)));
        return op(a_val, b_val);
    } else if (std::holds_alternative<int>(type_a) && std::holds_alternative<int>(type_b)) {
        return op(std::get<int>(type_a), std::get<int>(type_b));
    } else if (std::holds_alternative<char>(type_a) && std::holds_alternative<char>(type_b)) {
        return op(std::get<char>(type_a), std::get<char>(type_b));
    } else if (std::holds_alternative<std::string>(type_a) && std::holds_alternative<std::string>(type_b)) {
        return op(std::get<std::string>(type_a), std::get<std::string>(type_b));
    } else {
        throw std::runtime_error("Unsupported types for comparison");
    }
}

void PY_PRINT() {
    std::cout << std::endl;
}

template<typename... Args>
void PY_PRINT(const PY_OJ& first, const Args&... args) {
    auto value = type_inference(first);
    std::visit([](const auto& v) { std::cout << v << " "; }, value);
    PY_PRINT(args...);
}

// Helper function to print PY_OJ values
void print_py_oj(const PY_OJ& obj) {
    auto value = type_inference(obj);
    std::visit([](const auto& v) { std::cout << v; }, value);
}