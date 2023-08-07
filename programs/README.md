This README provides an overview of the programs used for testing.
For your convenience, all C programs have been pre-compiled into memory image
binary files.

`add_n_numbers`: A generalisation of `add_2_numbers`. User is first required
to enter a value for n. Note that n >= 0 and ensure that the number of
additions do not exceed n. Then, the user is to supply the numbers to be
added, one-by-one. If the user wishes to supply less than n numbers, simply
enter 0. The output S is the sum of the numbers.

`mult_n_numbers`: A generalisation of `mult_2_numbers`. Inputs are supplied
in a similar fashion to `add_n_numbers`.

`floor`: Computes the floor division between x and y, i.e., floor(x/y). If
y = 0, -1 is output, indicating zero division error. Else, the output Q is
the floored quotient of the numbers.

`factorial`: Computes the factorial of a number n, using a naive recursive
approach. Note that n > 10 may take very long to compute.

`turing_test`: A simple program which outputs "I am a human being." to stdout.

`greet`: Asks for the name of the user and outputs "Greetings, <name>!" to
stdout. This programs utilises dynamic memory allocation: it first asks for
the length of the name and mallocs (to `vm_riskxvii`'s self-implemented heap)
an appropriate area of memory to store/load the name.

`dynamic_array`: Creates a dynamic array using dynamic memory allocation via
`vm_riskxvii`'s self-implemented heap. User can supply non-zero integers as
elements via "loads". User will need to indicate the number of elements n they
would like to supply on each load. After that, they are to supply each element
one-by-one, up to n element (to halt early, enter 0). The memory for each load
is allocated via a malloc call. To keep track of the loads, a separate area of
memory is requested at the start to store the starting address of each load.
The fixed size of this area of memory is set to 16, i.e., a maximum of 16
loads can be made (less if the heap runs out of free banks before that).

`virtual_routines`: This program simply invokes all possible virtual routines
to ensure their behaviours are as expected.
