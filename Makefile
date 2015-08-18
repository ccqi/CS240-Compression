CXX= g++
CXXFLAGS = -g -Wall -MMD -std=c++0x 
OBJECTS = main.o TextComponent.o Decorator.o HuffmanEncoder.o LZWEncoder.o MTFEncoder.o PlainText.o RLEEncoder.o BWTransform.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = hangman

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

clean :
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}

Decorator.o: TextComponent.h