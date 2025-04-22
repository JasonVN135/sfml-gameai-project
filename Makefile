# Compiler
CXX = g++

# Flags
CXXFLAGS = -std=c++17 -Wall -I.

# SFML libraries
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Executable name
TARGET = main

# Source files
SRCS = main.cpp \
	    Game.cpp \
		Entity.cpp \
		Monster.cpp \
		LearningMonster.cpp \
		SteeringBehavior.cpp \
		DecisionTreeNode.cpp \
		BehaviorTreeNode.cpp \
		DecisionTreeLearner.cpp \
		Breadcrumb.cpp \
		VectorUtils.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Build target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)
