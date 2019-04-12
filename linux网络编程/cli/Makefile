main:=sys_pro_cli
object:=cli.o  cli_main.o  
cc:=gcc
option:=-g -lm -lpthread
$(main):$(object)
	$(cc) $^ -o $@ $(option)
%.o:%.c
	$(cc) -c  $< -o $@
clean:
	rm -rf *.o $(main)
