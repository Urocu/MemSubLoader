NAME = MemSubLoader
CC = g++
CFLAGS = -Wall
LINKS = -lstdc++ -lcomdlg32 -lshlwapi -lgdi32 -lgdiplus bin/Release/OutlineText.dll bin/Release/JsonCpp.dll
LIBLINKS = -I./includes -I./resources -I./lib -I./lib/json -I./lib/OutlineText
SRC_PATH = srcs/
OBJ_PATH = bin/
OUT_PATH = bin/Release/
C_EXTENSION = .cpp

#		눈_눈			SOURCES			눈_눈

MAIN_PATH	=	
MAIN_FILES 	= 	main game mainWindow settingsWindow subtitlesWindow configuratorWindow utils Win32InputBox
SRCS_FILES	+=	$(addprefix $(MAIN_PATH), $(MAIN_FILES))

SRCS_FILES_EXT	+= 	$(addsuffix $(C_EXTENSION), $(SRCS_FILES))
SRCS 			+= 	$(addprefix $(SRC_PATH), $(SRCS_FILES_EXT))
OBJS 			= 	$(addprefix $(OBJ_PATH), $(SRCS_FILES_EXT:cpp=o))

LIB_OUTLINE_TEXT_PATH = lib/OutlineText/
LIB_OUTLINE_TEXT_FILES = BmpOutlineText CanvasHelper DiffusedShadowStrategy DrawGradient ExtrudeStrategy\
					GDIPath IOutlineText ITextStrategy MaskColor NonSystemFontLoader OutlineText PngOutlineText stdafx TextDblGlowStrategy\
					TextDblOutlineStrategy TextGlowStrategy TextGradOutlineLastStrategy TextGradOutlineStrategy TextImplGetHeight TextNoOutlineStrategy\
					TextOnlyOutlineStrategy TextOutlineStrategy
LIB_OUTLINE_TEXT_SRCS_FILES = $(addprefix $(LIB_OUTLINE_TEXT_PATH), $(LIB_OUTLINE_TEXT_FILES))

LIB_OUTLINE_TEXT_SRCS_FILES_EXT	+= 	$(addsuffix $(C_EXTENSION), $(LIB_OUTLINE_TEXT_SRCS_FILES))
LIB_OUTLINE_TEXT_SRCS 			+= 	$(addprefix $(LIB_OUTLINE_TEXT_PATH), $(LIB_OUTLINE_TEXT_SRCS_FILES_EXT))
LIB_OUTLINE_TEXT_OBJS 			= 	$(addprefix $(OBJ_PATH), $(LIB_OUTLINE_TEXT_SRCS_FILES_EXT:cpp=o))

LIB_JSON_CPP_PATH = lib/json/
LIB_JSON_CPP_FILES = jsoncpp
LIB_JSON_CPP_SRCS_FILES = $(addprefix $(LIB_JSON_CPP_PATH), $(LIB_JSON_CPP_FILES))

LIB_JSON_CPP_SRCS_FILES_EXT	+= 	$(addsuffix $(C_EXTENSION), $(LIB_JSON_CPP_SRCS_FILES))
LIB_JSON_CPP_SRCS 			+= 	$(addprefix $(LIB_JSON_CPP_PATH), $(LIB_JSON_CPP_SRCS_FILES_EXT))
LIB_JSON_CPP_OBJS 			= 	$(addprefix $(OBJ_PATH), $(LIB_JSON_CPP_SRCS_FILES_EXT:cpp=o))

#		(҂◡_◡)			UTILS			(҂◡_◡)

RM = rm -rf

#		( ಠ ʖ̯ ಠ)		RULES			( ಠ ʖ̯ ಠ)

all: folders libOutlineText libJsonCpp $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT_PATH)$(NAME) $(OBJS) $(LINKS)

folders:
	@mkdir -p $(dir $(OBJ_PATH)$(LIB_OUTLINE_TEXT_PATH))
	@mkdir -p $(dir $(OBJ_PATH)$(LIB_JSON_CPP_PATH))
	@mkdir -p $(dir $(OBJ_PATH)$(LIB_INPUT_BOX_PATH))
	@mkdir -p $(dir ${OUT_PATH})

libOutlineText: $(LIB_OUTLINE_TEXT_OBJS)
	$(CC) ${CFLAGS} -shared $(LIB_OUTLINE_TEXT_OBJS) -lgdiplus -lgdi32 -Wl,--export-all-symbols -o $(OUT_PATH)OutlineText.dll

libJsonCpp: $(LIB_JSON_CPP_OBJS)
	$(CC) ${CFLAGS} -shared $(LIB_JSON_CPP_OBJS) -Wl,--export-all-symbols -o $(OUT_PATH)JsonCpp.dll

.c.o:
	@$(CC) $(FLAGS) -c ${LIBLINKS} $< -o ${<:.cpp=.o}

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	${CC} ${CFLAGS} $(LIBLINKS) -c $< -o $@

$(OBJ_PATH)$(LIB_OUTLINE_TEXT_PATH)%.o: $(LIB_OUTLINE_TEXT_PATH)%.cpp
	${CC} ${CFLAGS} $(LIBLINKS) -c $< -o $@

$(OBJ_PATH)$(LIB_JSON_CPP_PATH)%.o: $(LIB_JSON_CPP_PATH)%.cpp
	${CC} ${CFLAGS} $(LIBLINKS) -c $< -o $@

$(OBJ_PATH)$(LIB_INPUT_BOX_PATH)%.o: $(LIB_INPUT_BOX_PATH)%.cpp
	${CC} ${CFLAGS} $(LIBLINKS) -c $< -o $@

clean:
	@$(RM) -r $(OBJ_PATH)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.phony: all clean fclean re libOutlineText libJsonCpp folders