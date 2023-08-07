/*
Dan Nguyen
kngu7458
510594877
USYD CODE CITATION ACKNOWLEDGEMENT
This file contains acknowledgements of code
*/

/*
USYD CODE CITATION ACKNOWLEDGEMENT
I declare that the following section of code is largely inspired from another
C program provided by a COMP2017 staff titled add_2_numbers.c and it is not
my own work.
*/
inline char ConsoleReadChar(void)
{
    char result;
    int addr = 0x0812;
    asm volatile("lb %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));
    return result;
}
inline int ConsoleReadInt(void)
{
    int result;
    int addr = 0x0816;
    asm volatile("lw %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));
    return result;
}
unsigned volatile *const ConsoleWriteChar = (unsigned *)0x0800;
unsigned volatile *const Malloc = (unsigned *)0x0830;
unsigned volatile *const Free = (unsigned *)0x0834;
/* End of copied code */

void writes(char *str)
{
    while (*str != 0)
    	*ConsoleWriteChar = *((int *)(str++));
}

int main(void)
{   
    writes("How many characters long is your name?\n");
    int len = ConsoleReadInt();
    ConsoleReadChar();
    
    // Request memory from heap
    writes("What is your name?\n");
    *Malloc = len + 1;
    unsigned addr;
    asm volatile("addi %[adr], t3, 0": [adr]"=r"(addr) :);
    
    char chr;
    unsigned n;
    
    // Read name and store each character in malloced address
    n = 0;
    while ((chr = ConsoleReadChar()) != '\n') {
    	asm volatile("sb %[cin], 0(%[adr])": : [cin]"r"(chr), [adr]"r"(addr+n));
    	n++;
    }
    asm volatile("sb %[cin], 0(%[adr])": : [cin]"r"('\0'), [adr]"r"(addr+n));
    
    // Retrieve and write each character from malloced address
    writes("Greetings, ");
    n = 0;
    while (1) {
    	asm volatile("lb %[cout], 0(%[adr])": [cout]"=r"(chr): [adr]"r"(addr+n));
    	if (chr == '\0')
    	    break;
    	*ConsoleWriteChar = chr;
    	n++;
    }
    writes("! :)\n");
    *Free = addr;
    
    return 0;
}

