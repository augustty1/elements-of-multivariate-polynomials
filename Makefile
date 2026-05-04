CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I./include
DEBUGFLAGS = -g -O0

SRCDIR = src
INCDIR = include
BUILDDIR = build

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

TARGET = poly

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

debug: CXXFLAGS += -DDEBUG
debug: clean all

release: CXXFLAGS += -O3 -DNDEBUG
release: DEBUGFLAGS =
release: clean all

test: $(TARGET)
	./$(TARGET) < input.poly

clean:
	rm -rf $(BUILDDIR) $(TARGET)

help:
	@echo "  make        - compile"
	@echo "  make debug  - compile with debug flags"
	@echo "  make release- compile with optimizations"
	@echo "  make test   - executes with the poly example"
	@echo "  make clean  - remove compiled files"

.PHONY: all clean debug release test help
