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

int __attribute__((optimize("O0"))) multiply(int x, int y)
{
    if (x == 0 || y == 0)
    	return 0;
    
    unsigned i = 1;
    int n = x;
    int m = y;
    if (y < 0)
    	m = -y;
    while (i++ < m)
    	n += x;
    if (y < 0)
    	n = -n;
    
    return n;
}

int floor_divide(int x, int y)
{
    if (x == 0)
    	return 0;
    int n = 0;
    while (n++ < x) {
    	if (multiply(y, n) == x)
    	    return n;
    	if (multiply(y, n - 1) <= x && multiply(y, n) > x)
    	    return n - 1;
    }
    return -1;
}

int main(void)
{   
    int x = ConsoleReadInt();
    int y = ConsoleReadInt();
    if (y == 0)
    	*ConsoleWriteInt = -1; // zero division error
    else {
    	*ConsoleWriteChar = 'Q';
    	*ConsoleWriteChar = '=';
    	*ConsoleWriteInt = floor_divide(x, y);
    }
    *ConsoleWriteChar = '\n';

    return 0;
}

