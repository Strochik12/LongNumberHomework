# Компилятор
CXX = g++

# Флаги компиляции (можно добавить -O2 для оптимизации)
CXXFLAGS = -Wall -Wextra -std=c++20 -w -O2

SRCTESTS = tests.cpp LongNumber.cpp
SRCPI = pi.cpp LongNumber.cpp

# Список объектных файлов
OBJTESTS = $(SRCTESTS:.cpp=.o) 
OBJPI = $(SRCPI:.cpp=.o)

# Имя итогового исполняемого файла
TARGETTESTS = tests.exe
TARGETPI = pi.exe

all: $(TARGETTESTS) $(TARGETPI)

# Правило для сборки исполняемого файла
$(TARGETTESTS): $(OBJTESTS)
	$(CXX) $^ -o $@

$(TARGETPI): $(OBJPI)
	$(CXX) $^ -o $@


# Правило для компиляции .cpp в .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: test
test: $(TARGETTESTS)
	./$(TARGETTESTS)

.PHONY: pi
pi: $(TARGETPI)
	./$(TARGETPI)

# Команда для удаления скомпилированных файлов
clean:
	rm -f *.o $(TARGETTESTS) $(TARGETPI)
