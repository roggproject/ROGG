##
## Makefile for Makefile in /home/olivie_d//afs/fileslinux/makesfiles/makecpp
##
## Made by david olivier
## Login   <olivie_d@epitech.net>
##
## Started on  Thu Jan 24 17:43:51 2013 david olivier
## Last update Sun May  5 11:30:49 2013 david olivier
##

NAME =			Covert_Channel_TCP_IP

NAME_SENDER =		SenderTool

NAME_RECEIVER =		ReceiverTool

SRC_SHARED =		./sources/shared/tools/Exception.cpp \
			./sources/shared/tools/CUSocket.cpp \
			./sources/shared/tools/Packet.cpp \
			./sources/shared/tools/CUIPHeader.cpp \
			./sources/shared/tools/CUTCPHeader.cpp \
			./sources/shared/algorithms/AlgorithmManager.cpp \
			./sources/shared/algorithms/Storage-TTL.cpp \
			./sources/shared/algorithms/Checksum.cpp \
			./sources/shared/algorithms/TypeOfService.cpp \
			./sources/shared/algorithms/Timestamp.cpp \
			./sources/shared/algorithms/IPCoverTiming.cpp \
			./sources/shared/commands/Help.cpp

SRC_SENDER =		$(SRC_SHARED) \
			./sources/sender/main.cpp \
			./sources/sender/core/SenderClient.cpp \
			./sources/sender/commands/AddressManager.cpp \
			./sources/sender/inputs/InputManager.cpp \
			./sources/sender/inputs/PlainText.cpp

SRC_RECEIVER =		$(SRC_SHARED) \
			./sources/receiver/main.cpp \
			./sources/receiver/core/ReceiverClient.cpp \
			./sources/receiver/commands/PortManager.cpp \
			./sources/receiver/outputs/OutputManager.cpp \
			./sources/receiver/outputs/Terminal.cpp

OBJ_SENDER =		$(SRC_SENDER:.cpp=.o)

OBJ_RECEIVER =		$(SRC_RECEIVER:.cpp=.o)


INCLUDE_RECEIVER =	-I./headers/receiver/core/ -I./headers/receiver/outputs/ -I./headers/receiver/commands/

INCLUDE_SENDER =	-I./headers/sender/core/ -I./headers/sender/inputs/ -I./headers/sender/commands/

INCLUDE_SHARED =	-I./headers/shared/ -I./headers/shared/core/ -I./headers/shared/algorithms/ -I./headers/shared/tools/ -I./headers/shared/commands/

CXXFLAGS =		-W -Wall -Wextra -std=c++11 $(INCLUDE_SHARED) $(INCLUDE_SENDER) $(INCLUDE_RECEIVER)

FLAGSDEBUG =		-g3

COMPIL =		g++

REMOVE =		rm -rvf

FILES_TMP =		*~ \#*\#

VAL_DEBUG =		valgrind

$(NAME):		$(NAME_SENDER) $(NAME_RECEIVER)

$(NAME_SENDER):		$(OBJ_SENDER)
			$(COMPIL) -o $(NAME_SENDER) $(OBJ_SENDER) $(INCLUDE_SHARED) $(INCLUDE_SENDER)

$(NAME_RECEIVER):	$(OBJ_RECEIVER) 
			$(COMPIL) -o $(NAME_RECEIVER) $(OBJ_RECEIVER) $(INCLUDE_SHARED) $(RECEIVER)

all:			$(NAME)

clean:
			$(REMOVE) $(OBJ_SENDER) $(OBJ_RECEIVER) $(FILES_TMP)

fclean:			clean
			$(REMOVE) $(NAME_SENDER) $(NAME_RECEIVER)

re:			fclean all

.PHONY:			all clean fclean re
