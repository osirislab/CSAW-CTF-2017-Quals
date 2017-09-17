"""
ciphertext:
809fdd88dafa96e3ee60c8f179f2d88990ef4fe3e252ccf4
62deae51872673dcd34cc9f55380cb86951b8be3d8429839

flag{>x0r_i5_Add1+10n-m0D-2,'bU+_+h15_Wa5_m0d=8}

Notice how the challenge is about almost xor. Hopefully, the competitors can
draw the fact that the xor operation between two bits is simply an addition of
the bits modulo 2. With some examination of the encryption method, they will
realize that the encryption method uses addition modulo 2^n. For example, if
n=2, then groups of 2 bits are added to other groups of 2 bits. Taking the
modulo 2^2 of the result will restrict the result to 2 bits. Suppose n=3, then
groups of 3 bits are added to groups of 3 bits. Taking the modulo 2^3 of the
result will restrict the result to 2 bits.

Example:
a = 101100
b = 011010

n = 2:
  a = 10  11  00
+ b = 01  10  10
----------------
      11 101  10
---------------- <-- modulo 2^2 (keep 2 least significant bits)
      11  01  10  =>  110110

n = 3:
  a = 101  100
+ b = 011  010
--------------
     1000  110
-------------- <-- modulo 2^3 (keep 3 least significant bits)
      000  110  =>  000110

Notice how if n is 8 or greater, the encrypt function throws an exception. The
encryption method probably does work if n >= 8 but this serves as a convenience
for the competitors that they don't have to guess many n.

With the encryption method known and understood, ciphertext c can be decrypted
with the key k under modulo n, using the following functions:
"""

def decr_vals(c_chr, k_chr, n):
	return ((1 << n) + c_chr - k_chr) & ((1 << n) - 1)

def decrypt(k, c, n):
	rep_k = k * (len(c) // len(k)) + k[:len(c) % len(k)] # repeated key
	c_val_list = [get_vals(x, n) for x in get_nums(c, n)]
	k_val_list = [get_vals(x, n) for x in get_nums(rep_k, n)]
	c_vals, k_vals, m_vals = [], [], []
	for lst in c_val_list: c_vals += lst
	for lst in k_val_list: k_vals += lst
	m_vals = [decr_vals(c_vals[i], k_vals[i % len(k_vals)], n)
		for i in range(0, len(c_vals))]
	m_val_list = [m_vals[i:i+8] for i in range(0, len(m_vals), 8)]
	return "".join([get_chrs(lst, n) for lst in m_val_list])

"""
Finding the key is not easy.

Take advantage that the flag is in flag{} format, so that you can get the key
(or at least part of the key) by decrypting that portion of the ciphertext. The
facts that the ciphertext is 48 bytes long and that you know six of the
characters will be flag{}, give the players the hint that the key may be 6 (or
a multiple of 6) characters long.

For n=2 and n=4, the guess for the key is pretty easy to generate, since groups
of 2 bits and 4 bits fits nicely between each byte. Thus, the guesses for the
key for n=2 and n=4 are easy enough to not worry about. However, for n=3, it is
more difficult. Below details how someone could figure it out.

With "flag{" and the first five bytes of the ciphertext, you can guarantee that
most of the first five bytes of the key are:
00111110 10110011 10111100 00100101 1110000x  with the last bit inconclusive

With "}" and the last byte of the ciphertext, you can guarantee that most of
the last byte of the key is:
xx000100  with the first two bits inconclusive

However, we can figure out what the last 3 bits are. Reminder, that we so far
have this as the key:
00111110 10110011 10111100 00100101 1110000x xx110110
or in groups of 3 bits:
001 111 101 011 001 110 111 100 001 001 011 110 000 xxx 000 100

Let's analyze the first 6 bytes of the ciphertext as a binary. We can also
figure out parts of the message below, because most of the key is revealed.
message:      011 001 100 110 110 001 100 001 011 001 110 111 101 1?? 111 110
key:        + 001 111 101 011 001 110 111 100 001 001 011 110 000 xxx 000 100
-----------------------------------------------------------------------------
ciphertext:   100 000 001 001 111 111 011 101 100 010 001 101 101 011 111 010

Let's analyze the last 2 bytes of the ciphertext as a binary. Again, we can
figure out parts of the message below, because most of the key is revealed.
message:      0 011 100 ?01 111 101
key:        + 1 110 000 xxx 000 100
-----------------------------------
ciphertext:   1 001 100 000 111 001

Thus, we have two statements:
(1?? + xxx) % 8 = 011
(?01 + xxx) % 8 = 000
Let's take a look at the second statement. The last bit in ?01 can either be 1
or 0. If it's 1, then xxx must be 011 so that (101 + 011) % 8 = 000. If that
were the case, then (1?? + 011) % 8 = 011, so 1?? must equal 000, which is
impossible. Therefore, the last bit in ?01 must be 0. Thus, xxx = 111,
1?? = 100, and ?01 = 001; (100 + 111) % 8 = 011, (001 + 111) % 8 = 000.

Thus, the key is:
001 111 101 011 001 110 111 100 001 001 011 110 000 111 000 100
00111110 10110011 10111100 00100101 11100001 11000100
0x3EB3BC25E1C4
"\x3E\xB3\xBC\x25\xE1\xC4"

Since n=3 for this ciphertext, use this key to decrypt the ciphertext (encoded
in hexadecimal), and the flag will pop out.
"""