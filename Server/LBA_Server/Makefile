
OBJ=ActorsParticipantServant.o ChatParticipantServant.o ClientSessionManagerServant.o main.o SessionServant.o

CC=g++
INCDIR=-I. -I../infrastructure/include -I/usr/include
LIBDIR=-L/usr/lib -L../infrastructure/lib
LIBS=-lIce -lIceStorm -lIceutil -lGlacier2 -lgenerated


all: clean depend lbaserver install


lbaserver: $(OBJ)
	@echo "\n         **********creation de l'exe************\n"
	$(CC) $(INCDIR) $(OBJ) -o lbaserver $(LIBDIR) $(LIBS) 
	@echo "\n"		

.cpp.o:
	@echo "\n         **********creation des .o************\n"
	$(CC) -I. $(INCDIR) -c $<
	@echo "\n"

depend: 
	@echo "\n         **********creation des dependances************\n"	
	@$(CC) $(INCDIR) -MM *.cpp  > dep



install:
	@echo "\n         ********** copie des fichiers dans include & bib ************\n"
	cp lbaserver ../bin
	@echo "\n"
clean:
	@echo "\n        ********** nettoyage ************\n"
	rm -f $(OBJ) lbaserver dep
	@touch dep
	@echo "\n"
		
include dep
