NAME = MICROSHELL


SRC = microshell.c


OBJ = ${SRC:.c=.o}


CC = cc
FLAG = -Wall -Wextra -Werror


VIOLET = \033[0;35m
CYAN = \033[0;96m
DEF = \033[0m


%.o: %.c
	@${CC} ${FLAG} -s -c $< -o $@


${NAME} : ${OBJ}
	@${CC} ${FLAG} ${OBJ} -s -o ${NAME}
	@echo "${CYAN}./${NAME} created${DEF}"


all : ${NAME}


clean :
	@rm -rf ${OBJ}
	@echo "${VIOLET}OBJ deleted${DEF}"


fclean : clean
	@rm -rf ${NAME}
	@echo "${VIOLET}./${NAME} deleted${DEF}"


re : fclean all clean


.PHONY : all clean re fclean