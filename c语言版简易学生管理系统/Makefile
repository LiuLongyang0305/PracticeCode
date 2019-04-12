main:=C_project
object:=main.o teacher.o student.o menu.o super_user.o system_function.o terminal_set.o
cc:=gcc
option:=-g -lm
$(main):$(object)
	$(cc) $^ -o $@ $(option)
%.o:%.c
	$(cc) -c  $< -o $@
clean:
	rm -rf *.o $(main)
