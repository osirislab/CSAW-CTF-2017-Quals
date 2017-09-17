#!/usr/bin/env python3
from pwn import *
import sys

def read(r):
    msg = r.recvline().strip()
    assert msg[0] == '0'
    assert msg[-1] == '1'
    bits = map(int, list(msg))[1:-1]
    parity = bits[-1]
    c_bits = bits[:-1]
    if c_bits.count(1) % 2 == parity:
        sys.stdout.write(chr(int(msg[1:-2], 2)))
        r.sendline('1')
    else:
        r.sendline('0')

if __name__ == "__main__":
    r = remote(sys.argv[1], sys.argv[2])
    r.recvline()
    while True:
        read(r)
