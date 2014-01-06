# void *create_memory_space(void);
# void destroy_memory_space(void *buffer);

from ctypes import *
from generate_struct import *

#def getOffsetAddr(baseAddr, byteOffset)
#	offsetAddr = baseAddr + by
class TStruct(Structure):
    _fields_ = [
        ("a", c_int),
        ("b", c_byte),
        ("c", c_float),
        ("d", c_double)
    ]

class TStruct2(Structure):
    _fields_ = [
        ("a", c_int),
        ("spares", c_byte * 10),
        ("b", c_int)
    ]

class ByteBuffer:
    def __init__(self, ptr):
        # This is of type c_void_p
        self.ptr = ptr

    def as_int32(self, idx = 0):
        return cast(self.ptr, POINTER(c_int))[idx]

    def as_uint32(self, idx = 0):
        return cast(self.ptr, POINTER(c_uint))[idx]

    def as_byte(self, idx = 0):
        return cast(self.ptr, POINTER(c_byte))[idx]

    def as_ubyte(self, idx = 0):
        return cast(self.ptr, POINTER(c_ubyte))[idx]

    def as_float32(self, idx = 0):
        return cast(self.ptr, POINTER(c_float))[idx]

    def to_int32(self, val, idx = 0):
        cast(self.ptr, POINTER(c_int))[idx] = val

    def to_uint32(self, val, idx = 0):
        cast(self.ptr, POINTER(c_uint))[idx] = val
    
    def to_byte(self, val, idx = 0):
        cast(self.ptr, POINTER(c_byte))[idx] = val
    
    def to_ubyte(self, val, idx = 0):
        cast(self.ptr, POINTER(c_ubyte))[idx] = val

    def to_float32(self, val, idx = 0):
        cast(self.ptr, POINTER(c_float))[idx] = val

class MemSect:
    def __init__(self, base_ptr, size):
        self.base = base_ptr
        self.size = size

    def __getitem__(self, index):
        if index > self.size:
            raise IndexError 
        return ByteBuffer(self.base + index)


clib = CDLL('./libtest.so')

#print clib

clib.create_memory_space.restype = c_void_p
clib.create_memory_space.argtypes = []

raw_ptr = clib.create_memory_space()

print "Py addr:", hex(raw_ptr)
#print hex(raw_ptr + 2)

other_ptr = cast(raw_ptr, POINTER(c_int))

#print other_ptr[0]
#print cast(raw_ptr+4, POINTER(c_int))[0]

clib.alter_memory.restype = None
clib.alter_memory.argtypes = [c_void_p, c_uint]

offset = c_uint(4)

#print "Before alter mem:", cast(raw_ptr+offset.value, POINTER(c_ubyte))[0]
#clib.alter_memory(raw_ptr, offset)
#print "Before alter mem:", cast(raw_ptr+offset.value, POINTER(c_ubyte))[0]

clib.get_memory.restype = None
clib.get_memory.argtypes = [c_void_p, c_uint]

ptr_wrap = ByteBuffer(raw_ptr + 4)

#print "Altering memory on python side"
#print "Before: "
#clib.get_memory(raw_ptr, 4)
cast(raw_ptr + 4, POINTER(c_int))[0] = 10
#print "After: "
#clib.get_memory(raw_ptr, 4)

my_ptr = MemSect(raw_ptr, 200 * 4)

#print my_ptr[4].as_byte()
#print my_ptr[4].as_float32()
my_ptr[4].to_float32(2.0)
#print my_ptr[4].as_float32()

my_ptr[8].to_int32(21)
my_ptr[4].to_int32(22)
#print "things..."
#print my_ptr[4].as_int32()
#print my_ptr[4].as_int32(1)
#clib.get_memory(raw_ptr, 4)

# One way
my_struct = clib.create_struct()
#print cast(my_struct, POINTER(TStruct)).contents.a
s_conts = cast(my_struct, POINTER(TStruct)).contents
#clib.print_struct(my_struct);

s_conts.a = 2
#clib.print_struct(my_struct)
s_conts.b = 5
#clib.print_struct(my_struct)

# Better way
clib.create_struct_v.restypes = c_void_p
clib.create_struct_v.argtypes = []
structName = "TStruct"
basePtr = clib.create_struct_v()
print type(basePtr)
print hex(basePtr)
boundStruct = attachStruct(basePtr, "./lib_test.h", structName)

boundStruct.a = 100
print "better method a = 100"
clib.print_struct(basePtr)
print "better method b = 99"
boundStruct.b = 99
clib.print_struct(basePtr)
print "Size:", sizeof(boundStruct


my_struct2 = clib.create_struct2()
s2_conts = cast(my_struct2, POINTER(TStruct2)).contents
s2_conts.a = 4
s2_conts.spares[3] = 5
#clib.print_struct2(my_struct2)

#for i in xrange(200):
#	print other_ptr[i]
