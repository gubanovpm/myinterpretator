# **My Interpretator RISC-V**

###### I apologize in advance for my poor English :(

## You can build Makefile by yourself:
```
$ cmake .
$ make
```

### After all, you can <b> run </b> executable file (where <file_name_NUMBER> - name of load file with hex format instruction):
```
$ ./interpretator <file_name_1> <file_name_2> ...
``` 

### When you load files you start step-by-step execution each of them. To controll execution you have some commands: 
```
n                // move to the next step
p <register_num> // print value of register_num
r                // print all values of registers
```
