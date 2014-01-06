import re
import ctypes

# Type & qualifier identifiers
i_types = [
    'char',
    'int',
    'float',
    'double'
    ]

qualifiers = [
    'unsigned',
    'static',
    'struct',
    'typedef'
    ]

# Map from C int types to ctypes equivalent
c_type_map = {
    'int': ctypes.c_int,
    'unsigned int': ctypes.c_uint,
    'char': ctypes.c_byte,
    'unsigned char': ctypes.c_ubyte,
    'float': ctypes.c_float,
    'double': ctypes.c_double
    }

class TokenEnum:
    IDENTIFIER = 0
    IDX = 1
    LEFT_BRACE = 2
    RIGHT_BRACE = 3
    SEMICOLON = 4
    PRE_PROC = 5

class TokenRE:
    IDENTIFIER = r"\w+"
    IDX = r"\[\d+\]"
    LEFT_BRACE = r"\{"
    RIGHT_BRACE = r"\}"
    SEMICOLON = r";"
    PRE_PROC = r"#.*"

class Token:
    def __init__(self, tokenType, tokenVal):
        self.tokenType = tokenType
        self.tokenVal = tokenVal
    
    def __repr__(self):
        return "Token(Type = %d, Value = %s)" % (self.tokenType, self.tokenVal)

# Uses super shitty grammar established above to generate a token
# list.
def lex(fileName):
    cFile = open(fileName, "r").read()
    #print cFile

    scanner = re.Scanner([
        (TokenRE.IDENTIFIER, lambda scanner, token: Token(TokenEnum.IDENTIFIER, token)),
        (TokenRE.IDX, lambda scanner, token: Token(TokenEnum.IDX, token)),
        (TokenRE.LEFT_BRACE, lambda scanner, token: Token(TokenEnum.LEFT_BRACE, token)),
        (TokenRE.RIGHT_BRACE, lambda scanner, token: Token(TokenEnum.RIGHT_BRACE, token)),
        (TokenRE.SEMICOLON, lambda scanner, token: Token(TokenEnum.SEMICOLON, token)),
        (r".", lambda scanner, token: None)
        ], flags=re.DOTALL)

    results, remainder = scanner.scan(cFile)
   
    return results

# Lex and parse C source file to extract fields from structures.
def parse(fileName):
    # Struct parsing states
    S_NAME = 0
    TYPE = 1
    NAME = 2
    SIZE = 3

    # State variables
    done = False
    inStruct = False
    state = S_NAME

    # Temp variables
    structName = ""
    typeStr = ""
    fieldName = ""
    fieldType = None
    fields = []

    # Return var. A dict of all structs in the input file. 
    # Key is struct name. Value is list of fields.
    structs = {}

    tokens = lex(fileName)
 
    for i, token in enumerate(tokens):
        if not inStruct:
            if token.tokenVal == 'struct':
                try:
                    
                    if (tokens[i+1].tokenType != TokenEnum.LEFT_BRACE and
                            tokens[i+2].tokenType != TokenEnum.LEFT_BRACE):
                        continue
                except IndexError:
                    print "shit."

                inStruct = True
                state = S_NAME
        else:
            if state == S_NAME:
                if token.tokenType != TokenEnum.LEFT_BRACE:
                    structName = token.tokenVal
                else:
                    structName = "no_name"
                state = TYPE
            elif state == TYPE:
                if token.tokenType == TokenEnum.RIGHT_BRACE:
                    structs[structName] = fields
                    
                    # reinit per struct temp vars
                    fields = [] 
                    structName = ""

                    inStruct = False
                    continue
                if token.tokenVal in qualifiers:
                    typeStr += token.tokenVal + " "
                elif token.tokenVal in i_types:
                    typeStr += token.tokenVal
                    state = NAME
            elif state == NAME:
                fieldName = token.tokenVal
                state = SIZE
            elif state == SIZE:
                try:
                    fieldType = c_type_map[typeStr]
                except KeyError:
                    print "Couldn't find type %s in c_type_map." % (typeStr)
                    break
                if token.tokenType == TokenEnum.IDX:
                    fieldType *= int(token.tokenVal[1:-1])
                fields.append((fieldName, fieldType))
                
                # reinit per field temp vars
                typeStr = ""
                state = TYPE

    return structs
               
# Test 
if __name__ == "__main__":        
    structs = parse("lib_test.h")["TStruct"]
   
    print "FINAL OUTPUT:" 
    print structs 
