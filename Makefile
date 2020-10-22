SRCDIR = src
OBJSDIR = build
INCLUDESDIR = include
TESTDIR = tests

#put source files here
SOURCES = main.cpp ast.cpp ast_parser.cpp errors.cpp files.cpp lexer.cpp statement_parser.cpp token.cpp symbol.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = eme

TESTS = $(wildcard $(TESTDIR)/test_*.cpp)


CXX = g++
CXXFLAGS = -Wall -g -I$(INCLUDESDIR)

all: directories eme

directories:
	mkdir -p $(OBJSDIR)

# Create the program binary
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o eme \
			$(OBJSDIR)/*.o

# Compile rules for each dependecy
main.o: $(SRCDIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $(OBJSDIR)/main.o $(SRCDIR)/main.cpp

ast.o: $(SRCDIR)/ast.cpp $(SRCDIR)/ast.cpp
	$(CXX) $(CXXFLAGS) -c -o $(OBJSDIR)/ast.o $(SRCDIR)/ast.cpp
ast_parser.o: $(SRCDIR)/ast_parser.cpp $(SRCDIR)/ast_parser.cpp
	$(CXX) $(CXXFLAGS) -c -o $(OBJSDIR)/ast_parser.o $(SRCDIR)/ast_parser.cpp
errors.o: $(SRCDIR)/errors.cpp $(SRCDIR)/errors.cpp
	$(CXX) $(CXXFLAGS) -c -o $(OBJSDIR)/errors.o $(SRCDIR)/errors.cpp
lexer.o: $(SRCDIR)/lexer.cpp $(SRCDIR)/lexer.cpp
	$(CXX) $(CXXFLAGS) -c -o $(OBJSDIR)/lexer.o $(SRCDIR)/lexer.cpp
statement_parser.o: $(SRCDIR)/statement_parser.cpp $(SRCDIR)/statement_parser.cpp
	$(CXX) $(CXXFLAGS) -c -o $(OBJSDIR)/statement_parser.o $(SRCDIR)/statement_parser.cpp
token.o: $(SRCDIR)/token.cpp $(SRCDIR)/token.cpp
	$(CXX) $(CXXFLAGS) -c -o $(OBJSDIR)/token.o $(SRCDIR)/token.cpp
files.o: $(SRCDIR)/files.cpp $(SRCDIR)/files.cpp
	$(CXX) $(CXXFLAGS) -c -o $(OBJSDIR)/files.o $(SRCDIR)/files.cpp
symbol.o: $(SRCDIR)/symbol.cpp $(SRCDIR)/symbol.cpp
	$(CXX) $(CXXFLAGS) -c -o $(OBJSDIR)/symbol.o $(SRCDIR)/symbol.cpp
# Make the test_runner
test: $(TESTS) 
	$(CXX) $(CXXFLAGS) -Isrc/ -o test_runner $^


# Rule to clean up intermediate file and executable
clean:
	rm -rf $(OBJSDIR)
	rm -f eme test_runner *.o
