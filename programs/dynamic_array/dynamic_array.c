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
unsigned volatile *const ConsoleWriteInt = (unsigned *)0x0804;
unsigned volatile *const ConsoleWriteChar = (unsigned *)0x0800;
unsigned volatile *const Malloc = (unsigned *)0x0830;
unsigned volatile *const Free = (unsigned *)0x0834;
/* End of copied code */

// Number of times elements may be loaded
unsigned const LOAD_LIMIT = 16;

int __attribute__((optimize("O0"))) _multiply(unsigned x, unsigned y)
{    
    unsigned i = 1;
    int n = x;
    while (i++ < y)
    	n += x;
    
    return n;
}

inline void add(unsigned addr, int num)
{
    asm volatile("sw %[nin], 0(%[adr])": : [nin]"r"(num), [adr]"r"(addr));
}

inline int get(unsigned addr)
{
    int num;
    asm volatile("lw %[nout], 0(%[adr])": [nout]"=r"(num): [adr]"r"(addr));
    
    return num;
}

unsigned allocate(int n)
{
    unsigned addr;
    *Malloc = _multiply((n + 1), 4);
    asm volatile("addi %[adr], t3, 0": [adr]"=r"(addr) :);
        
    return addr;
}

void load_elements(unsigned addr, int limit)
{   
    int num;
    int i = 4;
    int n = 0;
    while (n < limit) {
    	if ((num = ConsoleReadInt()) == 0)
    	    break;
        add(addr+i, num);
        i += 4;
        n++;
    }
    // Store num of elements in this load at index 0
    add(addr, n);
}

void print_list(unsigned mem_storage_addr, int num_loads)
{
    int i;
    int num_elems;
    unsigned addr;
    int num;
    int l = 0;
    
    *ConsoleWriteChar = '[';
    while (l < _multiply(num_loads, 4))
    {
    	i = 4;
    	addr = get(mem_storage_addr+l);
    	num_elems = get(addr) + 1;
    	while (i < _multiply(num_elems, 4))
    	{
    	    num = get(addr+i);
    	    if (l > 0 || i > 4)
    	    	*ConsoleWriteChar = ',';
    	    *ConsoleWriteInt = num;
    	    i += 4;
    	}
    	l += 4;
    }
    *ConsoleWriteChar = ']';
}

void writes(char *str)
{
    while (*str != 0)
    	*ConsoleWriteChar = *((int *)(str++));
}

int main(void)
{    
    // Allocate memory to store loads
    unsigned mem_storage_addr;
    *Malloc = LOAD_LIMIT;
    asm volatile("addi %[adr], t3, 0": [adr]"=r"(mem_storage_addr) :);
    
    int num_loads = 0;
    int i = 0;
    
    unsigned addr;
    int n;
    char ans;
    
    while (num_loads < LOAD_LIMIT)
    {
    	if (num_loads)
    	{
    	    writes("Load more elements? [Y/n]: ");
    	    ConsoleReadChar();
    	    ans = ConsoleReadChar();
    	    if (ans == 'N' || ans == 'n') {
    	        break;
    	    }
   	    if (ans != 'Y' && ans != 'y') {
   	        writes("Invalid input.\n");
   	        continue;
   	    }
   	}
   	writes("n=");
    	int n = ConsoleReadInt();
    	if (n < 1) {
    	    writes("Invalid input.\n");
    	    continue;
    	}
    	addr = allocate(n);
    	if (addr == 0) {
    	    writes("Insufficient memory.\n");
    	    continue;
    	}
    	writes("Supply at most n non-zero integers.\n");
    	writes("Enter 0 to halt early.\n");
    	
    	add(mem_storage_addr+i, addr);
    	load_elements(addr, n);
    	
    	writes("Your list:\n");
    	print_list(mem_storage_addr, ++num_loads);
    	*ConsoleWriteChar = '\n';
    	
    	writes("Loads remaining: ");
    	*ConsoleWriteInt = LOAD_LIMIT - num_loads;
    	*ConsoleWriteChar = '\n';
    	
    	i += 4;
    }
    
    writes("Your list:\n");
    print_list(mem_storage_addr, ++num_loads);
    *ConsoleWriteChar = '\n';
    
    // Free all malloced memories
    int l = 0;
    while (l < num_loads) {
    	*Free = get(mem_storage_addr+l);
    	l += 4;
    }
        
    return 0;
}

