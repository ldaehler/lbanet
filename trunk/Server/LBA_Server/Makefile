
OBJ=ActorsParticipantServant.o ChatParticipantServant.o ClientSessionManagerServant.o main.o SessionServant.o ../../common/ActivableActor.o ../../common/Actor.o ../../common/MapInfoXmlReader.o ../../common/AreaSwitch.o ../../common/ContainerActor.o ../../common/DoorActor.o ../../common/ExitActor.o ../../common/FloorSwitch.o ../../common/LadderActor.o ../../common/LiftActor.o ../../common/LivingActor.o ../../common/SwitchActor.o ../../common/TextActor.o ../../common/UpExitActor.o ../../common/ZoneActivableActor.o ../../common/ZoneActor.o ../../common/HurtArea.o

CC=g++
INCDIR=-I. -I../infrastructure/include -I/usr/include -I../../common -I../../tinyxml/include
LIBDIR=-L/usr/lib -L../infrastructure/lib -L../../tinyxml/lib
LIBS=-lIce -lIceStorm -lIceUtil -lGlacier2 -lgenerated -ltinyxml


all: clean depend lbaserver install


lbaserver: $(OBJ)
	@echo "\n         **********creation de l'exe************\n"
	$(CC) $(INCDIR) $(OBJ) -o lbaserver $(LIBDIR) $(LIBS) 
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
	cp lbaserver ../bin
	@echo "\n"
clean:
	@echo "\n        ********** nettoyage ************\n"
	rm -f $(OBJ) lbaserver dep
	@touch dep
	@echo "\n"
		
include dep
