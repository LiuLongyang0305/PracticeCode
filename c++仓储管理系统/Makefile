main:=c++_project
object:=main.o class.o class_goods.o class_menu.o class_system.o derived_class.o
cc:=g++
option:=-g -lm -std=c++11
$(main):$(object)
	$(cc) $^ -o $@ $(option)
%.o:%.cpp
	$(cc) -c  $< -o $@ $(option)

clean:
	rm -rf *.o $(main)
