SRC = $(wildcard *.cpp)
APP = bulldozer

All: $(APP)
	#Making

$(APP): $(SRC)
	g++ -o $(APP) -std=c++11 $(SRC) -lpanel -lncurses

test: $(APP)
	./$(APP)
