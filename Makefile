CONFIG_MODULE_SIG=n
obj-m += mytime.o

OBJ_FILE = mytime_user_program.o
EXEC_FILE = mytime_user_program
c_flags = -Wall

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm $(OBJ_FILE) $(EXEC_FILE)

$(EXEC_FILE): $(OBJ_FILE)
	cc $(c_flags) -o $(EXEC_FILE) $(OBJ_FILE)
