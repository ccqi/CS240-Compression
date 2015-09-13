CXX= g++
CXXFLAGS = -g -Wall -MMD -std=c++0x 
OBJECTS = main.o TextComponent.o Decorator.o HuffmanEncoder.o LZWEncoder.o MTFEncoder.o PlainText.o RLEEncoder.o BWTransform.o Encoding.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = compression

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

clean :
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}

Decorator.o: TextComponent.h
