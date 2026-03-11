CXX = g++
CXXFLAGS = -std=c++17 -pthread -Iinclude

SRC = src/main.cpp src/KVStore.cpp src/CommandParser.cpp src/Snapshot.cpp

TARGET = kvstore

all:
	$(CXX) $(SRC) $(CXXFLAGS) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	del $(TARGET).exe 2>nul || rm -f $(TARGET)