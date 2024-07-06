#include <iostream>
#include <string>
#include <variant>
#include <stdexcept>
#include <vector>
#include <iomanip>
#include <sstream>

enum class PY_OJ_Type { INT, FLOAT, CHAR, STRING, LIST };

struct PY_OJ {
    union {
        int i;
        float f;
        char c;
        std::string* s;
        std::vector<PY_OJ>* l;
    };
    PY_OJ_Type active_type;

    PY_OJ() : i(0), active_type(PY_OJ_Type::INT) {}
    PY_OJ(int val) : i(val), active_type(PY_OJ_Type::INT) {}
    PY_OJ(float val) : f(val), active_type(PY_OJ_Type::FLOAT) {}
    PY_OJ(char val) : c(val), active_type(PY_OJ_Type::CHAR) {}
    PY_OJ(const std::string& val) : s(new std::string(val)), active_type(PY_OJ_Type::STRING) {}
    PY_OJ(const std::vector<PY_OJ>& val) : l(new std::vector<PY_OJ>(val)), active_type(PY_OJ_Type::LIST) {}

    ~PY_OJ() {
        if (active_type == PY_OJ_Type::STRING) {
            delete s;
        } else if (active_type == PY_OJ_Type::LIST) {
            delete l;
        }
    }

    PY_OJ(const PY_OJ& other) : active_type(other.active_type) {
        switch (active_type) {
            case PY_OJ_Type::INT: i = other.i; break;
            case PY_OJ_Type::FLOAT: f = other.f; break;
            case PY_OJ_Type::CHAR: c = other.c; break;
            case PY_OJ_Type::STRING: s = new std::string(*other.s); break;
            case PY_OJ_Type::LIST: l = new std::vector<PY_OJ>(*other.l); break;
        }
    }

    // Assignment operator
    PY_OJ& operator=(const PY_OJ& other) {
        if (this != &other) {
            this->~PY_OJ(); // Call destructor to free any allocated memory
            active_type = other.active_type;
            switch (active_type) {
                case PY_OJ_Type::INT: i = other.i; break;
                case PY_OJ_Type::FLOAT: f = other.f; break;
                case PY_OJ_Type::CHAR: c = other.c; break;
                case PY_OJ_Type::STRING: s = new std::string(*other.s); break;
                case PY_OJ_Type::LIST: l = new std::vector<PY_OJ>(*other.l); break;
            }
        }
        return *this;
    }
};

std::variant<int, float, char, std::string, std::vector<PY_OJ>> type_inference(const PY_OJ& obj) {
    switch(obj.active_type) {
        case PY_OJ_Type::INT: return obj.i;
        case PY_OJ_Type::FLOAT: return obj.f;
        case PY_OJ_Type::CHAR: return obj.c;
        case PY_OJ_Type::STRING: return *obj.s;
        case PY_OJ_Type::LIST: return *obj.l;
        default: throw std::runtime_error("Unknown type");
    }
}

std::string to_string(const std::variant<int, float, char, std::string, std::vector<PY_OJ>>& var) {
    return std::visit([](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            return std::to_string(arg);
        } else if constexpr (std::is_same_v<T, float>) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(6) << arg;
            std::string str = oss.str();
            str.erase(str.find_last_not_of('0') + 1, std::string::npos);
            if (str.back() == '.') str.pop_back();
            return str;
        } else if constexpr (std::is_same_v<T, char>) {
            return std::string(1, arg);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return "\"" + arg + "\"";
        } else if constexpr (std::is_same_v<T, std::vector<PY_OJ>>) {
            std::string result = "[";
            for (size_t i = 0; i < arg.size(); ++i) {
                result += to_string(type_inference(arg[i]));
                if (i < arg.size() - 1) result += ", ";
            }
            result += "]";
            return result;
        } else {
            throw std::runtime_error("Unsupported type in to_string");
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

PY_OJ PY_LIST_APPEND(PY_OJ& list, const PY_OJ& item) {
    if (list.active_type != PY_OJ_Type::LIST) {
        throw std::runtime_error("Append can only be used on lists");
    }
    // Create a copy of the item to be appended
    PY_OJ item_copy;
    switch (item.active_type) {
        case PY_OJ_Type::INT:
            item_copy = PY_OJ(item.i);
            break;
        case PY_OJ_Type::FLOAT:
            item_copy = PY_OJ(item.f);
            break;
        case PY_OJ_Type::CHAR:
            item_copy = PY_OJ(item.c);
            break;
        case PY_OJ_Type::STRING:
            item_copy = PY_OJ(*item.s);
            break;
        case PY_OJ_Type::LIST:
            item_copy = PY_OJ(*item.l);
            break;
    }
    list.l->push_back(item_copy);
    return PY_OJ(); // Return None
}

// Add this function to PY2.cpp
PY_OJ PY_LIST_DELETE(PY_OJ& list, const PY_OJ& index) {
    if (list.active_type != PY_OJ_Type::LIST) {
        throw std::runtime_error("Delete can only be used on lists");
    }
    auto index_value = type_inference(index);
    if (!std::holds_alternative<int>(index_value)) {
        throw std::runtime_error("List index must be an integer");
    }
    int idx = std::get<int>(index_value);
    if (idx < 0 || idx >= static_cast<int>(list.l->size())) {
        throw std::runtime_error("List index out of range");
    }
    PY_OJ removed_item = (*list.l)[idx];
    list.l->erase(list.l->begin() + idx);
    return removed_item; // Return the removed item, similar to Python's pop()
}

PY_OJ PY_LIST_GET(const PY_OJ& list, const PY_OJ& index) {
    if (list.active_type != PY_OJ_Type::LIST) {
        throw std::runtime_error("Subscript can only be used on lists");
    }
    auto index_value = type_inference(index);
    if (!std::holds_alternative<int>(index_value)) {
        throw std::runtime_error("List index must be an integer");
    }
    int idx = std::get<int>(index_value);
    if (idx < 0 || idx >= static_cast<int>(list.l->size())) {
        throw std::runtime_error("List index out of range");
    }
    return (*list.l)[idx];
}

template<typename Op>
bool PY_COMPARE(const PY_OJ& a, Op op, const PY_OJ& b) {
    auto type_a = type_inference(a);
    auto type_b = type_inference(b);

    return std::visit([&op](const auto& a, const auto& b) -> bool {
        using T1 = std::decay_t<decltype(a)>;
        using T2 = std::decay_t<decltype(b)>;

        if constexpr (std::is_same_v<T1, T2>) {
            return op(a, b);
        } else if constexpr (std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>) {
            return op(static_cast<double>(a), static_cast<double>(b));
        } else {
            throw std::runtime_error("Incompatible types for comparison");
        }
    }, type_a, type_b);
}

bool operator<(const PY_OJ& a, const PY_OJ& b) {
    return PY_COMPARE(a, std::less<>(), b);
}

bool operator<=(const PY_OJ& a, const PY_OJ& b) {
    return PY_COMPARE(a, std::less_equal<>(), b);
}

bool operator>(const PY_OJ& a, const PY_OJ& b) {
    return PY_COMPARE(a, std::greater<>(), b);
}

bool operator>=(const PY_OJ& a, const PY_OJ& b) {
    return PY_COMPARE(a, std::greater_equal<>(), b);
}

bool operator==(const PY_OJ& a, const PY_OJ& b) {
    return PY_COMPARE(a, std::equal_to<>(), b);
}

bool operator!=(const PY_OJ& a, const PY_OJ& b) {
    return PY_COMPARE(a, std::not_equal_to<>(), b);
}

void PY_PRINT() {
    std::cout << std::endl;
}

template<typename... Args>
void PY_PRINT(const PY_OJ& first, const Args&... args) {
    auto value = type_inference(first);
    std::visit([](const auto& v) {
        if constexpr (std::is_same_v<std::decay_t<decltype(v)>, std::vector<PY_OJ>>) {
            std::cout << "[";
            for (size_t i = 0; i < v.size(); ++i) {
                print_py_oj(v[i]);
                if (i < v.size() - 1) std::cout << ", ";
            }
            std::cout << "]";
        } else {
            std::cout << v;
        }
        std::cout << " ";
    }, value);
    PY_PRINT(args...);
}

// Helper function to print PY_OJ values
void print_py_oj(const PY_OJ& obj) {
    auto value = type_inference(obj);
    std::visit([](const auto& v) {
        if constexpr (std::is_same_v<std::decay_t<decltype(v)>, std::vector<PY_OJ>>) {
            std::cout << "[";
            for (size_t i = 0; i < v.size(); ++i) {
                print_py_oj(v[i]);
                if (i < v.size() - 1) std::cout << ", ";
            }
            std::cout << "]";
        } else {
            std::cout << v;
        }
    }, value);
}