all : synthese

synthese: synthese.c
	gcc -o synthese	synthese.c -lm -I.