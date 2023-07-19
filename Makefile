KULINA=/usr
#CC	=g++ -pthread
CC	=cc -pthread
kgfilebrowser	: kgfilebrowser.o kgfilebrowserCallbacks.o kgfilebrowsermain.o kgPopUp.o infobox.o statbox.o
	 $(CC) -o kgfilebrowser kgfilebrowser.o kgfilebrowserCallbacks.o kgfilebrowsermain.o kgPopUp.o infobox.o statbox.o -I$(KULINA)/include $(KULINA)/lib/libkulina.a $(KULINA)/lib/libgm.a -L/usr/X11R6/lib -lX11 -lXext -lm -lpthread 
	 rm -f kfbilebrowser.tgz
	 cp kgfilebrowser TARBALL/
	 tar czf kgfilebrowser.tgz TARBALL
kgfilebrowser.o	: kgfilebrowser.c Gclrkgfilebrowser.c 
	 $(CC) -c kgfilebrowser.c
kgfilebrowserCallbacks.o	: kgfilebrowserCallbacks.c  icons.c
	 $(CC) -c kgfilebrowserCallbacks.c
kgfilebrowsermain.o	: kgfilebrowsermain.c 
	 $(CC) -c kgfilebrowsermain.c
kgPopUp.o	: kgPopUp.c
	 $(CC) -c kgPopUp.c
infobox.o	: infobox.c
	 $(CC) -c infobox.c
statbox.o	: statbox.c
	 $(CC) -c statbox.c
clean	:  
	   rm -f *.o kgfilebrowser kgfilebrowser.tgz TARBALL/kgfilebrowser
	   


install	: kgfilebrowser
	 mkdir -p /usr/share/Kfb
	 install  -m 755 TARBALL/Kfb.png /usr/share/Kfb/
	 install  -m 755 TARBALL/Kfb.desktop  /usr/share/applications/
	 install -m 755 -s kgfilebrowser /usr/bin/Kfb
