#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void yeet(char *shellyeet)
{
    char c[32];
    printf("%p\n", c);
    memcpy(c, shellyeet, 103);
}

int main(int argc, char **argv){
    char a[8] = "yeet!\n";
    char b[8];
    setuid(1002);
    memset(b,0x00,8);
    printf("Enter the super top secret password\n");
    read(STDIN_FILENO, b, 7);
    if(!strcmp(a,b)) {
        printf("beep boop password accepted doot doot\n");
        yeet(argv[1]);
    }
    else {
        printf("bloop bleep invalid password doot doot heres your working directory\n");
        system("ls");
    }
}

