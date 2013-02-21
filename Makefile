# Graham Greving
# ggreving
# Joe Rowley
# jrowley

MKFILE    = Makefile
DEPSFILE  = ${MKFILE}.deps
NOINCLUDE = clean spotless
NEEDINCL  = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}
GMAKE     = gmake --no-print-directory

GCC       = gcc -g -O0 -Wall -Wextra -std=gnu99
MKDEPS    = gcc -MM

CSOURCE   = debugf.c hashset.c strhash.c spellchk.c
CHEADER   = debugf.h hashset.h strhash.h yyextern.h
OBJECTS   = ${CSOURCE:.c=.o} scanner.o
EXECBIN   = spellchk
SUBMITS   = ${CHEADER} ${CSOURCE} scanner.l ${MKFILE}
SOURCES   = ${SUBMITS}
PROJECT   = cmps012b-wm.f12 asg5

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

scanner.o : scanner.l
	flex -oscanner.c scanner.l
	gcc -g -O0 -std=gnu99 -c scanner.c

%.o : %.c
	${GCC} -c $<

clean :
	- rm ${OBJECTS} ${DEPSFILE} core scanner.c ${EXECBIN}.errs

spotless : clean
	- rm ${EXECBIN}

git:
	- git commit -a

submit : ${SUBMITS}
	submit ${PROJECT} ${SUBMITS} PARTNER README

deps : ${CSOURCE} ${CHEADER}
	@ echo "# ${DEPSFILE} created `date`" >${DEPSFILE}
	${MKDEPS} ${CSOURCE} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${GMAKE} deps

again :
	${GMAKE} spotless deps all

ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif

