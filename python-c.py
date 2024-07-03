import ast

class PythonToCConverter(ast.NodeVisitor):
    def __init__(self):
        self.c_code = []
        self.indent_level = 0

    def indent(self):
        return "  " * self.indent_level

    def visit_FunctionDef(self, node):
        if node.name == 'main':
            self.has_main = True
            self.c_code.append(f"{self.indent()}void py_main() {{")
        else:
            self.c_code.append(f"{self.indent()}PY_OJ {node.name}({', '.join(f'PY_OJ {arg.arg}' for arg in node.args.args)}) {{")
        
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
        func_name = self.visit(node.func)
        args = ', '.join(self.visit(arg) for arg in node.args)
        if func_name == 'print':
            return f'PY_PRINT({args})'
        return f"{func_name}({args})"

    def visit_Name(self, node):
        return node.id

    def visit_Num(self, node):
        if isinstance(node.n, float):
            return f"PY_OJ({node.n}f)"
        return f"PY_OJ({node.n})"

    def visit_Str(self, node):
        return f'PY_OJ("{node.s}")'

    def visit_Expr(self, node):
        expr = self.visit(node.value)
        self.c_code.append(f"{self.indent()}{expr};")

    def generate_main(self):
        self.c_code.append("int main() {")
        self.c_code.append("    py_main();")
        self.c_code.append("    return 0;")
        self.c_code.append("}")

def python_to_c(python_code):
    tree = ast.parse(python_code)
    converter = PythonToCConverter()
    converter.visit(tree)
    converter.generate_main()
    return '\n'.join(converter.c_code)

def read_file(file_path):
    with open(file_path, 'r') as file:
        return file.read()

def write_file(file_path, content):
    with open(file_path, 'w') as file:
        file.write(content)

# Read from file
input_file_path = 'input.py'  # Adjust this to your input file path
python_code = read_file(input_file_path)

# Convert to C++
cpp_code = python_to_c(python_code)

# Add necessary includes and import test.cpp functionality
cpp_code = '''#include <iostream>
#include <string>
#include <variant>
#include <stdexcept>
// Include test.cpp functionality
#include "test-oj.cpp"

''' + cpp_code

# Write to output file
output_file_path = 'output.cpp'
write_file(output_file_path, cpp_code)
print(f"C++ code has been written to {output_file_path}")
print(cpp_code)

# Print AST
tree = ast.parse(python_code)
print(ast.dump(tree))