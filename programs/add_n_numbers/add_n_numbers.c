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
inline int ConsoleReadInt(void)
{
    int result;
    int addr = 0x0816;
    asm volatile("lw %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));
    return result;
}
unsigned volatile *const ConsoleWriteInt = (unsigned *)0x0804;
unsigned volatile *const ConsoleWriteChar = (unsigned *)0x0800;
/* End of copied code */

int main(void)
{    
    *ConsoleWriteChar = 'n';
    *ConsoleWriteChar = '=';
    int n = ConsoleReadInt();
    int i = 0;
    int x = 0;
    if (n < 0) {
    	*ConsoleWriteInt = -1;
    	*ConsoleWriteChar = '\n';
    	return 0;
    }
    while (i++ < n) {
    	int y = ConsoleReadInt();
    	if (y == 0)
    	    break;
    	x += y;
    }
    *ConsoleWriteChar = 'S';
    *ConsoleWriteChar = '=';
    *ConsoleWriteInt = x;
    *ConsoleWriteChar = '\n';

    return 0;
}

