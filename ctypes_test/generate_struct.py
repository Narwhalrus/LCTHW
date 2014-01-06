import struct_parse
from ctypes import *

def attachStruct(basePtr, src, structName):
    try:
        class TempStruct(Structure):
            _fields_ = struct_parse.parse(src)[structName] 
    except KeyError:
        print "No such struct %s in source file %s." % (structName, src)

    return cast(basePtr, POINTER(TempStruct)).contents
