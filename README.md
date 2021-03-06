# CS350-Operating-Systems

## Assignment 02

### Part C - Kernel Module

a. The program implements a kernel module of a charecter device named `mytime`.
b. It   calls the `read()` function. It is passed as a reference to the file operation structure. This `read()` function returns a string which contains the result of both `current_kernel_time()` and `getnstimeofday()`. The output is as follows:

`current_kernel_time: 378959784 384958
 getnstimeofday: 378959784 384990`
The second program is the userspace program named `mytime_user_program`. It calls the charecter device 'N' times and also calls `gettimeofday()` showing the following output:

1. There exists a time difference between the time results generated by the 3 functions. The reason is - the three functions are executed with some delays between them as there are multiple other instructions which are getting executed in between, like moving data to/from registers, incrementing the program counter, and so on, which, when measured in the nanosecond scale, the difference is prominent.

2. When the program runs for large 'N's, the driver returns accurate values while `getnstimeofday()` returns different value. The reason for this behaivour is that the kernel module takes the time value from the CPU clock ticks while the getnstimeofday() makes a kernel call for fetching the time.

3. When the return value of the init module is changed to -1, the output is "Unable to load module : Operation not permitted" and the kernel crashes. This requires a reboot.

### Executing the program

To use this module, use the following commands:

1. `make`
2. `sudo insmod mytime.ko`
3. `sudo mknod /dev/mytime c <major_number> 0`  - Run this only for the first time or when the major number changes.
4. `make mytime_user_program`
5. `./mytime_user_program`

To remove the module, run:

 `sudo rmmod mytime`
