NAME = MemSubLoader
CC = g++
CFLAGS = -Wall
LINKS = -lstdc++ -lcomdlg32 -lshlwapi -lgdi32
LIBLINKS = -I./includes
SRC_PATH = srcs/
OBJ_PATH = bin/
OUT_PATH = bin/Release/
C_EXTENSION = .cpp

#		눈_눈			SOURCES			눈_눈

MAIN_PATH	=	
MAIN_FILES 	= 	main game windows utils
SRCS_FILES	+=	$(addprefix $(MAIN_PATH), $(MAIN_FILES))

SRCS_FILES_EXT	+= 	$(addsuffix $(C_EXTENSION), $(SRCS_FILES))
SRCS 			+= 	$(addprefix $(SRC_PATH), $(SRCS_FILES_EXT))
OBJS 			= 	$(addprefix $(OBJ_PATH), $(SRCS_FILES_EXT:cpp=o))

#		(҂◡_◡)			UTILS			(҂◡_◡)

RM = rm -rf

#		( ಠ ʖ̯ ಠ)		RULES			( ಠ ʖ̯ ಠ)

all: $(NAME)

$(NAME):  $(OBJS)
	@$(CC) $(CFLAGS) -o $(OUT_PATH)$(NAME) $(OBJS) $(LINKS)

.c.o:
	@$(CC) $(FLAGS) -c ${LIBLINKS} $< -o ${<:.cpp=.o} 


$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir -p $(dir ${OUT_PATH})
	${CC} ${CFLAGS} $(LIBLINKS) -c $< -o $@

clean:
	@$(RM) -r $(OBJ_PATH)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.phony: all clean fclean re