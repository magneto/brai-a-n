NAME = client

SRCS =	\
	./Protocol/Client.cpp \
	./Protocol/Connection.cpp \
	./Protocol/main.cpp \
	./Protocol/Message.cpp \
	./Protocol/MsgFactory.cpp \
	./Protocol/MsgQueue.cpp \
	./Protocol/Player.cpp \

OBJS = $(SRCS:.cpp=.o)

LDFLAGS = -lboost_system

CXXFLAGS = -W -Wall -Wextra -std=c++0x

CXX = g++

DBG = true

ifeq ($(DBG), true)
	CXXFLAGS += -g3
endif

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
