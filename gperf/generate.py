operations_dict = {
    'eq': 'Equal',
    'ne': 'NotEqual',
    'gt': 'Greater',
    'ge': 'GreaterEqual',
    'lt': 'Less',
    'le': 'LessEqual',
    'add': 'Add',
    'sub': 'Subtract',
    'mul': 'Multiply',
    'truediv': 'TrueDivide',
    'floordiv': 'FloorDivide',
    'mod': 'Modulo',
    'pow': 'Power',
    'pos': 'Positive',
    'neg': 'Negative',
    'and': 'And',
    'xor': 'Xor',
    'or': 'Or',
    'invert': 'Invert',
    'lshift': 'LeftShift',
    'rshift': 'RightShift',
    'getattr': 'GetAttr',
    'setattr': 'SetAttr',
    'delattr': 'DelAttr',
    'getitem': 'GetItem',
    'setitem': 'SetItem',
    'delitem': 'DelItem',
    'hash': 'Hash',
    'len': 'Len',
    'bool': 'ToBool',
    'int': 'ToInt',
    'float': 'ToFloat',
    'str': 'ToStr',
    'repr': 'ToRepr'
}

types = {
    'None': ['eq', 'ne', 'getattr'],
    'Bool': ['eq', 'ne', 'gt', 'ge', 'lt', 'le', 'add', 'sub', 'mul', 'truediv', 'floordiv', 'mod', 'pow', 'pos', 'neg', 'and', 'xor', 'or', 'invert', 'lshift', 'rshift', 'getattr'],
    'Int': ['eq', 'ne', 'gt', 'ge', 'lt', 'le', 'add', 'sub', 'mul', 'truediv', 'floordiv', 'mod', 'pow', 'pos', 'neg', 'and', 'xor', 'or', 'invert', 'lshift', 'rshift', 'getattr'],
    'Float': ['eq', 'ne', 'gt', 'ge', 'lt', 'le', 'add', 'sub', 'mul', 'truediv', 'floordiv', 'mod', 'pow', 'pos', 'neg', 'getattr'],
    'String': ['eq', 'ne', 'gt', 'ge', 'lt', 'le', 'add', 'mul', 'getattr'],
}

operator_map = {
    'eq': '==',
    'ne': '!=',
    'gt': '>',
    'ge': '>=',
    'lt': '<',
    'le': '<=',
    'add': '+',
    'sub': '-',
    'mul': '*',
    'truediv': '/',
    'floordiv': '//',
    'mod': '%',
    'pow': '**',
    'pos': '+',  
    'neg': '-',  
    'and': '&',
    'xor': '^',
    'or': '|',
    'invert': '~',
    'lshift': '<<',
    'rshift': '>>',
}

def binary(type_, method):
    return f'''
Value Py{type_}_{operations_dict[method]}(int argc, Value *argv) {{
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = {type_}_{operations_dict[method]}(argv[0], argv[1]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplemented("{operator_map[method]}", argv[0], argv[1]);
    return res;
}}
'''

def unary(type_, method):
    return f'''
Value Py{type_}_{operations_dict[method]}(int argc, Value *argv) {{
    if (argc != 0)
        reportArityError(0, 0, argc);
    Value res = {type_}_{operations_dict[method]}(argv[0]);
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplementedUnary("{operator_map[method]}", argv[0]);
    return res;
}}
'''

def getattr_(type_, method):
    return f'''
Value Py{type_}_{operations_dict[method]}(int argc, Value *argv) {{
    if (argc != 1)
        reportArityError(1, 1, argc);
    Value res = {type_}_GetAttr(argv[0], AS_STRING(argv[1]));
    if (IS_NOT_IMPLEMENTED(res))
        operatorNotImplementedUnary("getattr", argv[0]);
    return res;
}}
'''

def filename(type_):
    if type_ in ['None', 'Bool', 'Int', 'Float']:
        return f'value_{type_.lower()}.h'
    return f'object_{type_.lower()}.h'

def header(type_):
    return f'''%{{
#include <string.h>
#include "object_string.h"
#include "vm.h"
#include "value.h"
#include "{filename(type_)}"
'''

def middle(type_):
    return f'''
%}}

struct GperfMethod

%readonly-tables
%struct-type
%define lookup-function-name in_{type_.lower()}_set
'''

operations_func = {
    'eq': binary, 
    'ne': binary, 
    'gt': binary, 
    'ge': binary, 
    'lt': binary, 
    'le': binary, 
    'add': binary, 
    'sub': binary, 
    'mul': binary, 
    'truediv': binary, 
    'floordiv': binary, 
    'mod': binary, 
    'pow': binary, 
    'pos': unary, 
    'neg': unary,
    'and': binary, 
    'xor': binary, 
    'or': binary, 
    'invert': unary,
    'lshift': binary, 
    'rshift': binary, 
    'getattr': getattr_
}


for type_ in types:
    with open(f'gperf/methods_{type_.lower()}.txt', 'w') as file:
        file.write(header(type_))

        for method in types[type_]:
            func = operations_func[method]
            file.write(func(type_, method))

        file.write(middle(type_))

        file.write('\n%%\n')

        for method in types[type_]:
            file.write(f'__{method}__, Py{type_}_{operations_dict[method]}\n')
        
        file.write('%%\n')