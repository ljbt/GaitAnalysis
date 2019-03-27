CFLAGS=-g -O2

projet: main.o IHM.o Mat_Im_Recadrage.o libisentlib.a
	gcc -Wall main.o IHM.o Mat_Im_Recadrage.o -o projet libisentlib.a -lm -lglut -lGL -lX11
# Sous MacOSX, commenter la ligne de commande ci-dessus (en mettant un # au debut)
# et de-commenter la ligne de commande ci-apres :
#	gcc -Wall exemple.o -o exemple libisentlib.a -lm -framework OpenGL -framework GLUT

main.o: main.c GfxLib.h BmpLib.h ESLib.h definitions.h IHM.h Mat_Im_Recadrage.h 
	gcc -Wall $(CFLAGS) -c main.c
	
IHM.o: IHM.c GfxLib.h ESLib.h BmpLib.h definitions.h IHM.h Mat_Im_Recadrage.h
	gcc -Wall $(CFLAGS) -c IHM.c
	
Mat_Im_Recadrage.o: Mat_Im_Recadrage.c GfxLib.h ESLib.h BmpLib.h definitions.h IHM.h Mat_Im_Recadrage.h
	gcc -Wall $(CFLAGS) -c Mat_Im_Recadrage.c


libisentlib.a: BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o SocketLib.o ThreadLib.o VectorLib.o 
	ar r libisentlib.a BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o SocketLib.o ThreadLib.o VectorLib.o 
	ranlib libisentlib.a

BmpLib.o: BmpLib.c BmpLib.h OutilsLib.h
	gcc -Wall -O2 -c BmpLib.c

ESLib.o: ESLib.c ESLib.h ErreurLib.h
	gcc -Wall -O2 -c ESLib.c

ErreurLib.o: ErreurLib.c ErreurLib.h
	gcc -Wall -O2 -c ErreurLib.c

GfxLib.o: GfxLib.c GfxLib.h ESLib.h
	gcc -Wall -O2 -c GfxLib.c -I/usr/include/GL
# Sous MacOSX, commenter la ligne de commande ci-dessus (en mettant un # au debut)
# et de-commenter la ligne de commande ci-apres :
#	gcc -Wall -O2 -c GfxLib.c -Wno-deprecated-declarations

OutilsLib.o: OutilsLib.c OutilsLib.h
	gcc -Wall -O2 -c OutilsLib.c

#SocketLib.o: SocketLib.c SocketLib.h
#	gcc -Wall -O2 -c SocketLib.c

#ThreadLib.o: ThreadLib.c ThreadLib.h
#	gcc -Wall -O2 -c ThreadLib.c

#VectorLib.o: VectorLib.c VectorLib.h
#	gcc -Wall -O2 -c VectorLib.c -msse3

#WavLib.o: WavLib.c WavLib.h OutilsLib.h
#	gcc -Wall -O2 -c WavLib.c -Wno-unused-result
# Sous MacOSX, commenter la ligne de commande ci-dessus (en mettant un # au debut)
# et de-commenter la ligne de commande ci-apres :
#	gcc -Wall -O2 -c WavLib.c

zip:
	tar -cvzf libisentlib.tgz *.[ch] *.bmp *.pdf makefile

clean:
	rm -f *~ *.o

deepclean: clean
	rm -f projet

