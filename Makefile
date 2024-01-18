a.out: main.cpp
	g++ -O0 -g3 $^ -o $@
solution.exe: solution.cpp
	g++ -O0 -g3 $^ -o $@
clean:
	-rm a.out solution.exe
.PHONY: clean
