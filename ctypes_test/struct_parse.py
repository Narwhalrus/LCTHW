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
        (r"//.*?", None),
        (r"/\*.*?\*/", None),
        ("#.*?\n", None),
        (r".", None),
        ], flags=re.DOTALL)

    results, remainder = scanner.scan(cFile)
   
    # debug
    for result in results:
        print result
    
    return results

class Parser:
    def __init__(self):
        self.tokens = []

    def __call__(self, fileName):
        self.tokens = lex(fileName)
        structName = "no_name"

        self.idx = 0
        while True:
            token = self.tokens[self.idx]
            if token.tokenVal == 'struct':
                # Not needed
                self.idx += 1
                if self.tokens[self.idx].tokenType == TokenEnum.IDENTIFIER:
                    structName = self.tokens[self.idx].tokenVal 
                res = self._parseStruct()
                self.idx += 1 
                # Not needed
                if self.tokens[self.idx].tokenType == TokenEnum.IDENTIFIER:
                    structName = self.tokens[self.idx].tokenVal 

                res.__name__ = structName
                return res

            self.idx += 1
                        
    def _parseStruct(self):
        # Struct parsing states
        S_NAME = 0
        TYPE = 1
        NAME = 2
        SIZE = 3

        # State variables
        state = S_NAME

        # Temp variables
        # per struct
        structName = ""
        renameStruct = False
        fields = []

        # per field
        typeStr = ""
        fieldName = ""
        fieldType = None

        while True and self.idx < len(self.tokens) - 1:
            token = self.tokens[self.idx]
            if state == S_NAME:
                if token.tokenType != TokenEnum.LEFT_BRACE:
                    structName = token.tokenVal
                
                state = TYPE
            elif state == TYPE:
                # End of struct
                if token.tokenType == TokenEnum.RIGHT_BRACE:
                    # Create and return class with constructed field list
                    class TempStruct(ctypes.Structure):
                        _fields_ = fields
                    return TempStruct
                if token.tokenVal == 'struct':
                    # Yeah...
                    self.idx += 1
                    fieldType = self._parseStruct()
                    renameStruct = True
                    state = NAME
                elif token.tokenVal in qualifiers:
                    typeStr += token.tokenVal + " "
                elif token.tokenVal in i_types:
                    typeStr += token.tokenVal
                    try:
                        fieldType = c_type_map[typeStr]
                    except KeyError:
                        print "Couldn't find type %s in c_type_map." % (typeStr)
                        break
                    state = NAME
            elif state == NAME:
                fieldName = token.tokenVal
                if renameStruct:
                    # So meta...
                    fieldType.__name__ = fieldName
                    renameStruct = False
                state = SIZE
            elif state == SIZE:
                if token.tokenType == TokenEnum.IDX:
                    fieldType *= int(token.tokenVal[1:-1])
                fields.append((fieldName, fieldType))
    
                typeStr = ""
                state = TYPE

            self.idx += 1

# Bind dat shit yo!        
parse = Parser()
               
# Test 
if __name__ == "__main__":        
    #structs = parse("lib_test.h")["TStruct"]
    struct = parse("./test.h") 
    print type(struct)
    print "FINAL OUTPUT:" 
    print struct._fields_
    s = struct()
    
    print len(s.structy)

    print s._fields_
  
    print 'structy:'
    print s.structy[0]._fields_ 
    print 'another:'
    print s.another[0]._fields_ 
#    print Outer._fields_
#    s = Outer
