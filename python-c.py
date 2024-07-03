import ast

class PythonToCConverter(ast.NodeVisitor):
    def __init__(self):
        self.c_code = []
        self.indent_level = 0
        self.variables = {}
        self.function_return_types = {}

    def indent(self):
        return "  " * self.indent_level

    def visit_FunctionDef(self, node):
        args = ', '.join(self.get_type_and_name(arg) for arg in node.args.args)
        return_type = self.infer_return_type(node)
        self.function_return_types[node.name] = return_type
        self.c_code.append(f"{self.indent()}{return_type} {node.name}({args}) {{")
        self.indent_level += 1
        self.variables = {}  # Reset variables for each function, using a dictionary
        for stmt in node.body:
            self.visit(stmt)
        self.indent_level -= 1
        self.c_code.append(f"{self.indent()}}}")

    def get_type_and_name(self, arg):
        arg_type = 'float' if (arg.annotation and isinstance(arg.annotation, ast.Name) and arg.annotation.id == 'float') else 'int'
        return f"{arg_type} {arg.arg}"

    def infer_return_type(self, node):
        for stmt in ast.walk(node):
            if isinstance(node.returns, ast.Name):
                if node.returns.id == 'float':
                    return 'float'
                elif node.returns.id == 'int':
                    return 'int'
            if isinstance(stmt, ast.Return):
                if isinstance(stmt.value, ast.Num) and isinstance(stmt.value.n, float):
                    return 'float'
        return 'int'

    def infer_type(self, node):
        if isinstance(node, ast.Num):
            return 'float' if isinstance(node.n, float) else 'int'
        elif isinstance(node, ast.BinOp):
            left_type = self.infer_type(node.left)
            right_type = self.infer_type(node.right)
            return 'float' if 'float' in (left_type, right_type) else 'int'
        elif isinstance(node, ast.Name):
            return self.variables.get(node.id, 'int')  # Default to int if unknown
        elif isinstance(node, ast.Call):
            return self.function_return_types.get(node.func.id, 'int')  # Default to int if unknown
        return 'int'  # Default to int for other cases

    def visit_Assign(self, node):
        target = self.visit(node.targets[0])
        value = self.visit(node.value)
        inferred_type = self.infer_type(node.value)
        
        if target not in self.variables:
            self.variables[target] = inferred_type
            self.c_code.append(f"{self.indent()}{inferred_type} {target} = {value};")
        else:
            self.c_code.append(f"{self.indent()}{target} = {value};")

    def visit_BinOp(self, node):
        left = self.visit(node.left)
        right = self.visit(node.right)
        op = {ast.Add: '+', ast.Sub: '-', ast.Mult: '*', ast.Div: '/'}.get(type(node.op), '?')
        
        # If either operand is a float, cast the result to float
        if self.infer_type(node.left) == 'float' or self.infer_type(node.right) == 'float':
            return f"((float)({left} {op} {right}))"
        return f"({left} {op} {right})"

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
        op = {ast.Eq: '==', ast.LtE: '<=', ast.GtE: '>=', ast.Lt: '<', ast.Gt: '>'}.get(type(node.ops[0]), '?')
        right = self.visit(node.comparators[0])
        return f"{left} {op} {right}"

    def visit_Call(self, node):
        func_name = self.visit(node.func)
        args = ', '.join(self.visit(arg) for arg in node.args)
        if func_name == 'print':
            if len(node.args) == 1:
                arg = node.args[0]
                if isinstance(arg, ast.Call):
                    return_type = self.function_return_types.get(arg.func.id, 'int')
                    format_spec = '%f' if return_type == 'float' else '%d'
                else:
                    format_spec = '%f' if isinstance(arg, ast.Num) and isinstance(arg.n, float) else '%d'
                return f'printf("{format_spec}\\n", {args})'
            else:
                return f'printf({args})'
        return f"{func_name}({args})"

    def visit_Name(self, node):
        return node.id

    def visit_Num(self, node):
        if isinstance(node.n, float):
            return f"{node.n}f"
        return str(node.n)

    def visit_Expr(self, node):
        expr = self.visit(node.value)
        self.c_code.append(f"{self.indent()}{expr};")

# The rest of the code remains the same

def python_to_c(python_code):
    tree = ast.parse(python_code)
    converter = PythonToCConverter()
    converter.visit(tree)
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

# Convert to C
c_code = python_to_c(python_code)

# Add necessary C includes and main function
c_code = "#include <stdio.h>\n\n" + c_code

# Write to output file
output_file_path = 'test.c'
write_file(output_file_path, c_code)
print(f"C code has been written to {output_file_path}")
print(c_code)

# print ast
tree = ast.parse(python_code)
print(ast.dump(tree))