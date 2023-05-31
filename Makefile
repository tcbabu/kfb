KULINA=/usr
#CC	=g++ -pthread
CC	=cc -pthread
kgfilebrowser	: kgfilebrowser.o kgfilebrowserCallbacks.o kgfilebrowsermain.o kgPopUp.o
	 $(CC) -o kgfilebrowser kgfilebrowser.o kgfilebrowserCallbacks.o kgfilebrowsermain.o kgPopUp.o -I$(KULINA)/include $(KULINA)/lib/libkulina.a $(KULINA)/lib/libgm.a -L/usr/X11R6/lib -lX11 -lXext -lm -lpthread -lz -lbz2 -lGL
kgfilebrowser.o	: kgfilebrowser.c Gclrkgfilebrowser.c 
	 $(CC) -c kgfilebrowser.c
kgfilebrowserCallbacks.o	: kgfilebrowserCallbacks.c  icons.c
	 $(CC) -c kgfilebrowserCallbacks.c
kgfilebrowsermain.o	: kgfilebrowsermain.c 
	 $(CC) -c kgfilebrowsermain.c
kgPopUp.o	: kgPopUp.c
	 $(CC) -c kgPopUp.c
clean	:  
	   rm -f *.o kgfilebrowser

install	: kgfilebrowser
	 cp kgfilebrowser /usr/bin/Kfb
