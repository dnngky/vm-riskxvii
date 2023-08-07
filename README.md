# VM-RISKXVII: An RV32I-based virtual machine

This repository contains the source code for VM-RISKXVII. Its ISA is a reduced version of RV32I, comprising 33 instructions (which can be found in `stdvmop.h`). For enquiries, contact dan.kynguyen2@outlook.com.

A Makefile has been provided for compiling and executing programs. To compile the VM, enter (from the root directory):

```bash
$ make
```

To execute a program on the VM, simply supply the file path as a command line argument, e.g., to execute `hello_world.mi` (in `./examples`), simply enter (from the root directory):

```bash
$ ./vm_riskxvii ./examples/hello_world/hello_world.mi
```

Note that the VM can only execute `.mi` files. All executables can be found in `./examples` (simple programs) and `./programs` (more advanced programs). For a summary of what each executable in `./programs` does, simply enter (from the root directory):

```bash
$ make summary
```

To run all executables in `./programs` (and verify their correctness), simply enter (from the root directory):

```bash
$ make run
```
