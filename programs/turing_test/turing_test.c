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
unsigned volatile *const ConsoleWriteChar = (unsigned *)0x0800;
/* End of copied code */

void __attribute__((optimize("O0"))) writes(char *str)
{
    while (*str != 0)
    	*ConsoleWriteChar = *((int *)(str++));
}

int main()
{    
    writes("I am a human being.\n");
    
    return 0;
}
