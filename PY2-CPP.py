import ast

class PythonToCConverter(ast.NodeVisitor):
    def __init__(self):
        self.c_code = []
        self.indent_level = 0

    def indent(self):
        return "  " * self.indent_level

    def has_return(self, node):
        for stmt in ast.walk(node):
            if isinstance(stmt, ast.Return):
                return True
        return False

    def visit_FunctionDef(self, node):
        if node.name == 'main':
            self.has_main = True
            self.c_code.append(f"{self.indent()}void py_main() {{")
        else:
            return_type = "PY_OJ" if self.has_return(node) else "void"
            self.c_code.append(f"{self.indent()}{return_type} {node.name}({', '.join(f'PY_OJ {arg.arg}' for arg in node.args.args)}) {{")
        
        self.indent_level += 1
        for stmt in node.body:
            self.visit(stmt)
        self.indent_level -= 1
        self.c_code.append(f"{self.indent()}}}")

    def visit_Assign(self, node):
        target = self.visit(node.targets[0])
        value = self.visit(node.value)
        self.c_code.append(f"{self.indent()}auto {target} = {value};")

    def visit_BinOp(self, node):
        left = self.visit(node.left)
        right = self.visit(node.right)
        op = {
            ast.Add: 'PY_ADD',
            ast.Sub: 'PY_SUB',
            ast.Mult: 'PY_MULT',
            ast.Div: 'PY_DIV'
        }.get(type(node.op), '?')
        return f"{op}({left}, {right})"

    def visit_Return(self, node):
        if node.value is None:
            self.c_code.append(f"{self.indent()}return;")
        else:
            self.c_code.append(f"{self.indent()}return {self.visit(node.value)};")

    def visit_If(self, node):
        condition = self.visit(node.test)
        self.c_code.append(f"{self.indent()}if ({condition}) {{")
        self.indent_level += 1
        for stmt in node.body:
            self.visit(stmt)
        self.indent_level -= 1
        self.c_code.append(f"{self.indent()}}}")
        if node.orelse:
            self.c_code.append(f"{self.indent()}else {{")
            self.indent_level += 1
            for stmt in node.orelse:
                self.visit(stmt)
            self.indent_level -= 1
            self.c_code.append(f"{self.indent()}}}")

    def visit_Compare(self, node):
        left = self.visit(node.left)
        op = {
            ast.Eq: 'std::equal_to<>()', 
            ast.NotEq: 'std::not_equal_to<>()',
            ast.Lt: 'std::less<>()', 
            ast.LtE: 'std::less_equal<>()', 
            ast.Gt: 'std::greater<>()', 
            ast.GtE: 'std::greater_equal<>()'
        }.get(type(node.ops[0]), '?')
        right = self.visit(node.comparators[0])
        return f"PY_COMPARE({left}, {op}, {right})"

    def visit_Call(self, node):
        func = self.visit(node.func)
        args = ', '.join(self.visit(arg) for arg in node.args)
        if func == 'print':
            return f'PY_PRINT({args})'
        elif func == 'PY_LIST_APPEND':
            if isinstance(node.func, ast.Attribute):
                # Case: my_list.append(4)
                list_arg = self.visit(node.func.value)
                item_arg = self.visit(node.args[0])
                return f'{func}({list_arg}, {item_arg})'
            else:
                # Case: PY_LIST_APPEND(my_list, 4)
                return f'{func}({args})'
        return f"{func}({args})"

    def visit_Name(self, node):
        return node.id

    def visit_Constant(self, node):
        if isinstance(node.value, float):
            return f"PY_OJ({node.value}f)"
        elif isinstance(node.value, int):
            return f"PY_OJ({node.value})"
        elif isinstance(node.value, str):
            return f'PY_OJ("{node.value}")'
        else:
            return str(node.value)

    def visit_Expr(self, node):
        expr = self.visit(node.value)
        self.c_code.append(f"{self.indent()}{expr};")

    def visit_List(self, node):
        elements = [self.visit(elt) for elt in node.elts]
        return f"PY_OJ({{std::vector<PY_OJ>{{{', '.join(elements)}}}}})"

    def visit_Subscript(self, node):
        value = self.visit(node.value)
        index = self.visit(node.slice)
        return f"PY_LIST_GET({value}, {index})"

    def visit_Attribute(self, node):
        value = self.visit(node.value)
        if node.attr == 'append':
            return f"PY_LIST_APPEND"
        # Add other list methods as needed
        return f"{value}.{node.attr}"

    def visit(self, node):
        method = 'visit_' + node.__class__.__name__
        visitor = getattr(self, method, self.generic_visit)
        return visitor(node)

    def generic_visit(self, node):
        return f"/* Unhandled node type: {type(node).__name__} */"

    def generate_main(self):
        self.c_code.append("int main() {")
        self.c_code.append("    py_main();")
        self.c_code.append("    return 0;")
        self.c_code.append("}")

def python_to_c(python_code):
    tree = ast.parse(python_code)
    converter = PythonToCConverter()
    for node in tree.body:
        converter.visit(node)
    converter.generate_main()
    return '\n'.join(converter.c_code)

def read_file(file_path):
    with open(file_path, 'r') as file:
        return file.read()

def write_file(file_path, content):
    with open(file_path, 'w') as file:
        file.write(content)

# Read from file
input_file_path = 'PY-IN.py'  # Adjust this to your input file path
python_code = read_file(input_file_path)

# Convert to C++
cpp_code = python_to_c(python_code)

# Add necessary includes and import test.cpp functionality
cpp_code = '''#include <iostream>
#include <string>
#include <variant>
#include <stdexcept>
// Include PY.cpp functionality
#include "PY2.cpp"

''' + cpp_code

# Write to output file
output_file_path = 'PY-OUT.cpp'
write_file(output_file_path, cpp_code)
print(f"C++ code has been written to {output_file_path}")
print(cpp_code)

# Print AST
tree = ast.parse(python_code)
print(ast.dump(tree))