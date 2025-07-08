CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
TARGET = reactor_server
SOURCES = main.cpp Acceptor.cpp InetAddress.cpp Socket.cpp SocketIO.cpp TcpConnection.cpp EventLoop.cpp TcpServer.cpp TaskQueue.cpp ThreadPool.cpp HeadServer.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean