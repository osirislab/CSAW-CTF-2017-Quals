import binascii
import time
from z3.z3 import *

mask = b'\xff\xff\xff\xff\xff\xff\xff\xff\x00\xff\xff\xff\xff\xff\xff\xff\x00\xff\xff\xff\xff\xff\xff\xff'

split_results = [
    # H    L
    (0x270, 0x211),
    (0x255, 0x229),
    (0x291, 0x25e),
    (0x233, 0x1f9),
    (0x278, 0x27b),
    (0x221, 0x209),
    (0x25d, 0x290),
    (0x28f, 0x2df),
]

def z3abs(x):
    return If(x >= 0, x, -x)

def sad(a, b):
    return Sum(
        *(z3abs(ZeroExt(64, Extract(i, i-7, a)) - ZeroExt(64, Extract(i, i-7, b))) for i in range(63, 0, -8))
    )

def bin2int(b):
    return sum(ord(x) << (8*i) for i,x in enumerate(b))

def split_xlate(xmm5, comp, flag_h, flag_l, solver):
    xmm5_h = BitVecVal(bin2int(xmm5[8:]), 64)
    xmm5_l = BitVecVal(bin2int(xmm5[:8]), 64)

    print "xmm5 high:", hex(xmm5_h.as_long())
    print "xmm5 low: ", hex(xmm5_l.as_long())

    for si in range(8, 0, -1):
        m_h = BitVecVal(bin2int(mask[si+8:si+16]), 64)
        m_l = BitVecVal(bin2int(mask[si:si+8]), 64)
        print "Mask high:", hex(m_h.as_long())
        print "Mask low: ", hex(m_l.as_long())

        xmm2_h = flag_h & m_h
        xmm2_l = flag_l & m_l

        s_high = sad(xmm2_h, xmm5_h)
        s_low = sad(xmm2_l, xmm5_l)

        solver.add(s_high == comp[si-1][0])
        solver.add(s_low == comp[si-1][1])

        xmm5_h = s_high
        xmm5_l = s_low

if __name__ == "__main__":
    s = Solver()
    flag_h = BitVec('flag_h', 64)
    flag_l = BitVec('flag_l', 64)

    shuf_flag_h = Concat(Extract(31, 0, flag_l), Extract(63, 32, flag_l))
    shuf_flag_l = flag_h

    split_xlate(open('main.bin', 'rb').read(16), split_results, shuf_flag_h, shuf_flag_l, s)

    print(time.time())
    s.check()
    print(time.time())
    stuff = s.model()
    flag = hex(stuff[flag_h].as_long())[2:].decode('hex') + hex(stuff[flag_l].as_long())[2:].decode('hex')
    print(flag[::-1])
