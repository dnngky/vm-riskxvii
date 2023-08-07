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
unsigned volatile *ConsoleWriteChar = (unsigned *)0x0800;
unsigned volatile *ConsoleWriteInt = (unsigned *)0x0804;
unsigned volatile *ConsoleWriteUInt = (unsigned *)0x0808;

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

unsigned volatile *DumpPC = (unsigned *)0x0820;
unsigned volatile *DumpReg = (unsigned *)0x0824;
unsigned volatile *DumpMem = (unsigned *)0x0828;
/* End of copied code */

void writes(char *str)
{
    while (*str != 0)
    	*ConsoleWriteChar = *((int *)(str++));
}

int main(void)
{
    writes("Console Read Character: ");
    char c = ConsoleReadChar();
    writes("Console Read Signed Integer: ");
    int n = ConsoleReadInt();
    
    writes("Console Write Character: ");
    *ConsoleWriteChar = c;
    *ConsoleWriteChar = '\n';
    writes("Console Write Signed Integer: ");
    *ConsoleWriteInt = n;
    *ConsoleWriteChar = '\n';
    writes("Console Write Unsigned Integer: 0x");
    *ConsoleWriteUInt = n;
    *ConsoleWriteChar = '\n';
    
    writes("Dump PC: 0x");
    *DumpPC = 0;
    
    writes("Dump Register Banks:\n");
    *DumpReg = 0;
    
    writes("Dump Memory Word: 0x");
    *DumpMem = 0x0000; // First instruction
    
    return 0;
}

