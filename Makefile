# Název spustitelného souboru
TARGET = program

# Seznam objektových souborů
OBJECTS = parser.o expand_array.o lexer.o error.o stack.o testy.o front.o symTable.o

# Kompilátor
CC = gcc

# Kompilační vlajky
CFLAGS = -Wall -Wextra -std=c11

# Hlavní cíl - jakmile spustíš make, zkompiluje a slinkuje program
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Pravidlo pro kompilaci parser.c do parser.o
parser.o: parser.c parser.h lexer.h expand_array.h error.h stack.h testy.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o

# Pravidlo pro kompilaci expand_array.c do expand_array.o
expand_array.o: expand_array.c expand_array.h lexer.h error.h stack.h testy.h
	$(CC) $(CFLAGS) -c expand_array.c -o expand_array.o

# Pravidlo pro kompilaci lexer.c do lexer.o
lexer.o: lexer.c lexer.h expand_array.h error.h stack.h testy.h
	$(CC) $(CFLAGS) -c lexer.c -o lexer.o

# Pravidlo pro kompilaci error.c do error.o
error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

# Pravidlo pro kompilaci stack.c do stack.o
stack.o: stack.c stack.h testy.h
	$(CC) $(CFLAGS) -c stack.c -o stack.o

# Pravidlo pro kompilaci testy.c do testy.o
testy.o: testy.c testy.h
	$(CC) $(CFLAGS) -c testy.c -o testy.o

# Pravidlo pro kompilaci front.c do front.o
front.o: front.c front.h error.h stack.h
	$(CC) $(CFLAGS) -c front.c -o front.o

# Pravidlo pro kompilaci front.c do front.o
symTable.o: symTable.c symTable.h error.h
	$(CC) $(CFLAGS) -c symTable.c -o symTable.o

# Pravidlo pro vyčištění kompilovaných souborů
clean:
	rm -f $(OBJECTS) $(TARGET)

# Pravidlo pro spuštění programu
run: $(TARGET)
	./$(TARGET)
