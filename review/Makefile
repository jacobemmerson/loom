CXX      = g++
CXXFLAGS = -std=c++17 -Wall
TARGET   = main
SRCS     = main.cpp bank.cpp

$(TARGET): $(SRCS) bank.h
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
