VAR = main.o noeud.o fils.o commande.o

nulix: $(VAR)
	gcc -Wall $(VAR) -o nulix

main.o: main.c commande.h
	gcc -Wall -c main.c

noeud.o: noeud.c noeud.h
	gcc -Wall -c noeud.c

fils.o: fils.c fils.h noeud.h
	gcc -Wall -c fils.c

commande.o: commande.c commande.h fils.h noeud.h
	gcc -Wall -c commande.c

