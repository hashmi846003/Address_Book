CC := cc

MAIN_FILE := abk_main.c
SRC_FILE := abk_fileops.c abk_log.c abk_menus.c

MAIN_OBJECTS := abk_main.o
OBJECTS := abk_fileops.o abk_log.o abk_menus.o

TRGT := abk

${TRGT} : ${OBJECTS} ${MAIN_OBJECTS}
	${CC} $^ -o $@

${MAIN_OBJECTS} : ${MAIN_FILE}
	${CC} -c $^

${OBJECTS} : ${SRC_FILE} ${MAIN_FILE}
	${CC} -c $^


clean:
	rm -f *.o ${TRGT}
