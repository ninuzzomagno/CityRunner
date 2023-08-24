#one-line comment
TARGET = hello
OBJS = main.o Animations.o timeManager.o DtTimer.o Render.o Parallax.o Cubes.o ClassMyGame.o Player.o 
 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
 
LIBS=  -losl -ljpeg -lpng -lz\
         -lpsphprm -lpspsdk -lpspctrl -lpsprtc -lpsppower -lpspgu -lpspgum  -lpspaudiolib -lpspaudio  -lpspaudiocodec\
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -lstdc++

#EXTRA_TARGETS is the output file
#PSP_EBOOT_TITLE is the EBOOT's name
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = City runner
PSP_EBOOT_ICON= icon.png
PSP_EBOOT_PIC1= background.png
 
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak