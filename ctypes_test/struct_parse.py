import re
import ctypes

# Type & keyword identifiers
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

# Map from C int types to ctypes equivalent
c_type_map = {
    'int': c_int,
    'unsigned int': c_uint,
    'char': c_byte,
    'unsigned char': c_ubyte,
    'float': c_float,
    'double': c_double
    }

class TokenEnum:
    IDENTIFIER = 0
    IDX = 1
    PRE_PROC = 2
    LEFT_BRACE = 3
    RIGHT_BRACE = 4
    SEMICOLON = 5

class TokenRE:
    IDENTIFIER = r"\w+"
    IDX = r"\[(\d+)\]"
    PRE_PROC = r"#.*"
    LEFT_BRACE = r"{"
    RIGHT_BRACE = r"}"
    SEMICOLON = r";"

class Token:
    def __init__(self, tokenType, tokenVal):
        self.tokenType = tokenType
        self.tokenVal = tokenVal

def lex(fileName):
    cFile = open(fileName, "r").read()

    scanner = re.Scanner([
        (TokenRE.IDENTIFIER, lambda scanner, token: Token(TokenEnum.IDENTIFIER, 

    
