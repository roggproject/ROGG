NAME =	analysistool

SRC =	./sources/main.cpp \
	./sources/AnalysisService.cpp \
	./sources/ADetection.cpp \
	./sources/CommunicationPair.cpp \
	./sources/State.cpp \
	./sources/AnalysisManager.cpp \
	./sources/detections/TTLDetection.cpp \
	./sources/detections/ToSDetection.cpp \
	./sources/detections/TimeTCPDetection.cpp \
	./sources/detections/IpChecksumDetection.cpp \
	./sources/detections/IpidDetection.cpp

INCLUDE =	-I ./headers/ -I ./headers/detections/ -I$(BOOST)/include

LIBS =		-ltins -L$(BOOST)/lib -lboost_system -lboost_thread -std=c++11

$(NAME):
		g++ -o $(NAME) $(SRC) $(INCLUDE) $(LIBS)

all:		$(NAME)

clean:
		rm -vf $(OBJ)

fclean:		clean
		rm -vf $(NAME)

re:		fclean all