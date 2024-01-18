a.out: main.cpp
	g++ -O0 -g3 $^ -o $@
clean:
	-rm a.out
.PHONY: clean
