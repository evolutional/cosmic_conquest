CFLAGS= -E500 +c +C -B
cosmic:                 conquest.o main.o rev.o makefile
        increv
        ln main.o conquest.o rev.o +Cd -lcl -O cosmic
conquest.o:             conquest.c defs.h romulan.c klingon.c berserker.c alien.c ships.h makefile
main.o:                 main.c human.c debug.c defs.h makefile
zoo:     
        zoo aunPP ARC:cosmic.zoo *.c *.h *.info cosmic.doc makefile rev.o
        delete ARC:cosmic.bak
dist:
        zoo aunPP ARC:cosmic-dist.zoo main.c human.c conquest.c romulan.c klingon.c berserker.c alien.c *.h *.info cosmic.doc makefile games/ cosmic rev.o C:increv
        delete ARC:cosmic-dist.bak
all:                    cosmic cosmic.zoo cosmic-dist.zoo
run:                    cosmic
        cosmic
