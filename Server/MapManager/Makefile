TARGET=mapmanager
OBJ=ActorReceiverServant.o InfosReceiverServant.o main.o MapHandler.o MapHandlerThread.o MapManagerServant.o SharedData.o ../../common/ActivableActor.o ../../common/Actor.o ../../common/MapInfoXmlReader.o

CC=g++
INCDIR=-I. -I../infrastructure/include -I/usr/include -I../../common -I../../tinyxml/include
LIBDIR=-L/usr/lib -L../infrastructure/lib -L../../tinyxml/lib
LIBS=-lIce -lIceStorm -lIceUtil -lGlacier2 -lgenerated -ltinyxml


all: clean depend $(TARGET) install


$(TARGET): $(OBJ)
	@echo "\n         **********creation de l'exe************\n"
	$(CC) $(INCDIR) $(OBJ) -o $(TARGET) $(LIBDIR) $(LIBS) 
	@echo "\n"		

.cpp.o:
	@echo "\n         **********creation des .o************\n"
	$(CC) -O2 -D_LBANET_SERVER_SIDE_ -DTIXML_USE_STL -I. $(INCDIR) -c $<
	@echo "\n"

depend: 
	@echo "\n         **********creation des dependances************\n"	
	@$(CC) $(INCDIR) -MM *.cpp  > dep



install:
	@echo "\n         ********** copie des fichiers dans include & bib ************\n"
	cp $(TARGET) ../bin
	@echo "\n"
clean:
	@echo "\n        ********** nettoyage ************\n"
	rm -f $(OBJ) $(TARGET) dep
	@touch dep
	@echo "\n"
		
include dep
