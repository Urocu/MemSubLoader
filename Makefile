NAME = MemSubLoader
CC = g++
CFLAGS = -Wall
LINKS = -lstdc++ -lcomdlg32 -lshlwapi -lgdi32 -lgdiplus bin/Release/OutlineText.dll
LIBLINKS = -I./includes -I./resources -I./lib/OutlineText
SRC_PATH = srcs/
OBJ_PATH = bin/
OUT_PATH = bin/Release/
C_EXTENSION = .cpp

#		눈_눈			SOURCES			눈_눈

MAIN_PATH	=	
MAIN_FILES 	= 	main game mainWindow settingsWindow subtitlesWindow utils
SRCS_FILES	+=	$(addprefix $(MAIN_PATH), $(MAIN_FILES))

SRCS_FILES_EXT	+= 	$(addsuffix $(C_EXTENSION), $(SRCS_FILES))
SRCS 			+= 	$(addprefix $(SRC_PATH), $(SRCS_FILES_EXT))
OBJS 			= 	$(addprefix $(OBJ_PATH), $(SRCS_FILES_EXT:cpp=o))

OUTLINETEXT_PATH = lib/OutlineText/
OUTLINETEXT_FILES = BmpOutlineText CanvasHelper DiffusedShadowStrategy DrawGradient ExtrudeStrategy\
					GDIPath IOutlineText ITextStrategy MaskColor NonSystemFontLoader OutlineText PngOutlineText stdafx TextDblGlowStrategy\
					TextDblOutlineStrategy TextGlowStrategy TextGradOutlineLastStrategy TextGradOutlineStrategy TextImplGetHeight TextNoOutlineStrategy\
					TextOnlyOutlineStrategy TextOutlineStrategy
OUTLINETEXT_SRCS_FILES = $(addprefix $(OUTLINETEXT_PATH), $(OUTLINETEXT_FILES))

OUTLINETEXT_SRCS_FILES_EXT	+= 	$(addsuffix $(C_EXTENSION), $(OUTLINETEXT_SRCS_FILES))
OUTLINETEXT_SRCS 			+= 	$(addprefix $(OUTLINETEXT_PATH), $(OUTLINETEXT_SRCS_FILES_EXT))
OUTLINETEXT_OBJS 			= 	$(addprefix $(OBJ_PATH), $(OUTLINETEXT_SRCS_FILES_EXT:cpp=o))

#		(҂◡_◡)			UTILS			(҂◡_◡)

RM = rm -rf

#		( ಠ ʖ̯ ಠ)		RULES			( ಠ ʖ̯ ಠ)

all: folders lib $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT_PATH)$(NAME) $(OBJS) $(LINKS)

folders:
	@mkdir -p $(dir $(OBJ_PATH)$(OUTLINETEXT_PATH))
	@mkdir -p $(dir ${OUT_PATH})

lib: $(OUTLINETEXT_OBJS)
	$(CC) ${CFLAGS} -shared $(OUTLINETEXT_OBJS) -lgdiplus -lgdi32 -Wl,--export-all-symbols -o $(OUT_PATH)OutlineText.dll

.c.o:
	@$(CC) $(FLAGS) -c ${LIBLINKS} $< -o ${<:.cpp=.o}

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	${CC} ${CFLAGS} $(LIBLINKS) -c $< -o $@

$(OBJ_PATH)$(OUTLINETEXT_PATH)%.o: $(OUTLINETEXT_PATH)%.cpp
	${CC} ${CFLAGS} $(LIBLINKS) -c $< -o $@

clean:
	@$(RM) -r $(OBJ_PATH)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.phony: all clean fclean re lib folders