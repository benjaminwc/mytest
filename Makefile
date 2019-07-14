objects = main.o myclock.o elements.o
hello : $(objects)
	g++ -o hello $(objects)
main.o : 
myclock.o : myclock.h
elements.o : elements.h
.PHONY : clean
clean :
	rm hello $(objects)
