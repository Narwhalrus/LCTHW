# void *create_memory_space(void);
# void destroy_memory_space(void *buffer);

from ctypes import *

#def getOffsetAddr(baseAddr, byteOffset)
#	offsetAddr = baseAddr + by

clib = CDLL('./libtest.so')

print clib

clib.create_memory_space.restype = c_void_p
clib.create_memory_space.argtypes = []

raw_ptr = clib.create_memory_space()

print "Py addr:", hex(raw_ptr)
print hex(raw_ptr + 2)

other_ptr = cast(raw_ptr, POINTER(c_int))

print other_ptr[0]
print cast(raw_ptr+4, POINTER(c_int))[0]
#for i in xrange(200):
#	print other_ptr[i]
