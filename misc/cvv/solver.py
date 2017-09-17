from pwn import *
import random, sys

def Check_CC_Number(number):
    number_as_list = [int(char) for char in str(number)]
    for ind in range(16-2, -1, -2):
        number_as_list[ind] *= 2
        if number_as_list[ind] > 9:
            number = str(number_as_list[ind])
            sum_of_digits = 0
            for char in number:
                sum_of_digits += int(char)
            number_as_list[ind] = sum_of_digits
    if int(str(9 * (sum(number_as_list[:-1])))[-1]) == number_as_list[-1]:
        return '1'
    return '0'

def genCustCC(prefix):
	before_checksum = [int(x) for x in prefix]
	for i in range(11):
		before_checksum.append(random.randint(0, 9))
	after_checksum = before_checksum[:]
	for i in range(14, -1, -2):
	    before_checksum[i] *= 2
	    if before_checksum[i] > 9:
	        number = str(before_checksum[i])
	        sum_of_digits = 0
	        for char in number:
	            sum_of_digits += int(char)
	        before_checksum[i] = sum_of_digits
	checksum = 0
	for item in before_checksum:
	    checksum += item
	checksum *= 9
	after_checksum.append(int(str(checksum)[-1]))
	return int("".join(str(char) for char in after_checksum))

def genCC_Checksum(checksum):
	x = genCC("Visa")
	while str(x)[-1] != checksum:
		x = genCC("Visa")
	return x

def genCC_Suff(suffix):
	x = genCC("Visa")
	while str(x)[12:] != suffix:
		x = genCC("Visa")
	return x

def genCC(type):
    if type == "MasterCard":
        before_checksum = [5]
        for i in range(14):
            before_checksum.append(random.randint(0, 9))
        after_checksum = before_checksum[:]
        for i in range(14, -1, -2):
            before_checksum[i] *= 2
            if before_checksum[i] > 9:
                number = str(before_checksum[i])
                sum_of_digits = 0
                for char in number:
                    sum_of_digits += int(char)
                before_checksum[i] = sum_of_digits
        checksum = 0
        for item in before_checksum:
            checksum += item
        checksum *= 9
        after_checksum.append(int(str(checksum)[-1]))
        return int("".join(str(char) for char in after_checksum))
    elif type == "Visa":
    	before_checksum = [4]
        for i in range(14):
            before_checksum.append(random.randint(0, 9))
        after_checksum = before_checksum[:]
        for i in range(14, -1, -2):
            before_checksum[i] *= 2
            if before_checksum[i] > 9:
                number = str(before_checksum[i])
                sum_of_digits = 0
                for char in number:
                    sum_of_digits += int(char)
                before_checksum[i] = sum_of_digits
        checksum = 0
        for item in before_checksum:
            checksum += item
        checksum *= 9
        after_checksum.append(int(str(checksum)[-1]))
        return int("".join(str(char) for char in after_checksum))
    elif type == "Discover":
    	before_checksum = [6]
        for i in range(14):
            before_checksum.append(random.randint(0, 9))
        after_checksum = before_checksum[:]
        for i in range(14, -1, -2):
            before_checksum[i] *= 2
            if before_checksum[i] > 9:
                number = str(before_checksum[i])
                sum_of_digits = 0
                for char in number:
                    sum_of_digits += int(char)
                before_checksum[i] = sum_of_digits
        checksum = 0
        for item in before_checksum:
            checksum += item
        checksum *= 9
        after_checksum.append(int(str(checksum)[-1]))
        return int("".join(str(char) for char in after_checksum))
    else:
    	before_checksum = [3]
        for i in range(13):
            before_checksum.append(random.randint(0, 9))
        after_checksum = before_checksum[:]
        for i in range(13, -1, -2):
            before_checksum[i] *= 2
            if before_checksum[i] > 9:
                number = str(before_checksum[i])
                sum_of_digits = 0
                for char in number:
                    sum_of_digits += int(char)
                before_checksum[i] = sum_of_digits
        checksum = 0
        for item in before_checksum:
            checksum += item
        checksum *= 9
        after_checksum.append(int(str(checksum)[-1]))
        return int("".join(str(char) for char in after_checksum))


r = remote(*sys.argv[1:])

for i in range(25):
	x = r.recvuntil("!")
	print x
	y = x.split()
	z = str(genCC(y[4][:-1]))
	print z
	r.sendline(z)
	x = r.recvuntil("!")
	print x

for i in range(25):
	x = r.recvuntil("!")
	print x
	y = x.split()
	z = str(genCustCC(y[8][:-1]))
	print z
	r.sendline(z)
	x = r.recvuntil("!")
	print x

for i in range(25):
	x = r.recvuntil("!")
	print x
	y = x.split()
	z = str(genCC_Checksum(y[8][0]))
	print z
	r.sendline(z)
	x = r.recvuntil("!")
	print x

for i in range(25):
	x = r.recvuntil("!")
	print x
	y = x.split()
	z = str(genCC_Suff(y[8][:-1]))
	print z
	r.sendline(z)
	x = r.recvuntil("!")
	print x

for i in range(25):
	x = r.recvuntil(")")
	print x
	y = x.split()
	print y[5]
	z = Check_CC_Number(y[5])
	print z
	r.sendline(z)
	#x = r.recvline()
	x = r.recvuntil('!', timeout=0.3)
	#x = r.recvuntil('\n', timeout=2)
	print x

x = r.recvall()
print x
