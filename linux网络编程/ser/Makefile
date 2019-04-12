main:=sys_pro_ser
object:=ser.o ser_main.o 
cc:=gcc
option:=-g -lm -lpthread
$(main):$(object)
	$(cc) $^ -o $@ $(option)
%.o:%.c
	$(cc) -c  $< -o $@
clean:
	rm -rf *.o $(main)
