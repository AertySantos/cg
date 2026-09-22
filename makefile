CXX = g++ -o
NOME = test
LIBS = -lGL -lGLU -lglut
all:	codigo
codigo: main.cpp
	$(CXX) $(NOME) main.cpp robo.cpp tiro.cpp alvo.cpp $(LIBS)
	./test