TARGET := test
SRC := main.cpp

$(TARGET): $(SRC)
	g++ -o $@ $^ -std=c++11

clean:
	rm -rf $(TARGET)