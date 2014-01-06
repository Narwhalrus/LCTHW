import re
from ctypes import *

c_code = '''\
#include <stdio.h>

struct test
{
    unsigned int a;
    float b_0;
    float b_1;
    float b_2;
    char c;
    int arr[5];
};
'''

i_types = [
    'char',
    'int',
    'float',
    'double'
    ]

keywords = [
    'unsigned',
    'static',
    'struct',
    'typedef'
    ]

def s_words(scanner, token): 
    if token in i_types:
        token_t = 'type'
    elif token in keywords:
        token_t = 'keyword'
    else:
        token_t = 'name' 
    
    return (token_t, token)

def s_idx(scanner, token):
    return ('idx', int(token[1:-1]))
    

def s_symbols(scanner, token):
    return ('sym', token)

scanner = re.Scanner([
    (r"\w+", s_words),
    (r"\[\d+\]", lambda scanner, token: ('idx', int(token[1:-1]))),
    (r"\{", lambda scanner, token: ('sym', token)),
    (r"\}", lambda scanner, token: ('sym', token)),
    (r";", lambda scanner, token: ('sym', token)),
    #(r"[\{\};]", s_symbols), 
    (r"#.*\s*", lambda scanner, token: None),
    (r'.', lambda scanner, token: None),
    ], flags=re.DOTALL)

print c_code
print

results, remainder = scanner.scan(c_code)
print results

# C Type mapping
c_type_map = {
    'int': c_int,
    'unsigned int': c_uint,
    'char': c_byte,
    'unsigned char': c_ubyte,
    'float': c_float,
    'double': c_double
    }

# Construct structure
TYPE = 0
NAME = 1
IDX = 2

done = False
in_struct = False
state = TYPE 

s_name = ""
type_s = ""
fields = []
f_type = None
f_name = ""

i = 0
while not done:
    token_t = results[i][0]
    token_s = results[i][1]
    if not in_struct:
        if token_s == 'struct':
            i += 1
            s_name = token_s
            i += 1
            token_t = results[i][0]
            token_s = results[i][1]
            if token_s == '{':
                in_struct = True
                state = TYPE
            else:
                print "C struct formatting error."
                break
    else:
        if state == TYPE:
            if token_s == '}':
                print "Done with things!"
                break
            if token_t == 'keyword':
                type_s += token_s + ' '
            elif token_t == 'type':
                type_s += token_s
                state = NAME
        elif state == NAME:
            f_name = token_s
            state = IDX
        elif state == IDX:
            try:
                f_type = c_type_map[type_s]
            except KeyError:
                print 'Couldn\'t find type in c_type_map'
                break
            if token_t == 'idx':
                f_type *= token_s
            print (f_name, f_type)
            fields.append((f_name, f_type))
            type_s = ""
            state = TYPE
    i += 1
   
print fields 
