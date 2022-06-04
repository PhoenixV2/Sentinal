# Andrew Seddon
# Makefile for building the OpenGl learning project
# Because I really don't want to use visual studio

CC = g++
CFLAGS = -g -Wall


SRC =		main.cpp renderer.cpp
OBJ =		main.o renderer.o
EXE =		Sentinel.exe

# Adding compilation for ImGui
IMGUI_INC = ImGui/include
IMGUI_SRC = ImGui/sources
GLAD = Glad/src
IM_SRC = $(IMGUI_INC)/imgui.cpp
IM_SRC += $(IMGUI_INC)/imgui_demo.cpp $(IMGUI_INC)/imgui_draw.cpp $(IMGUI_INC)/imgui_tables.cpp $(IMGUI_INC)/imgui_widgets.cpp
IM_SRC += $(IMGUI_SRC)/imgui_impl_glfw.cpp $(IMGUI_SRC)/imgui_impl_opengl3.cpp $(GLAD)/glad.c
# Adding the object files via fancy
OBJ += $(addsuffix .o, $(basename $(notdir $(IM_SRC))))
SRC += $(IM_SRC)

IDFLAGS = -Iglfw-337/include -IGlad/include
LDFLAGS = -Lglfw-337/lib-mingw-w64
LDLIBS = -lopengl32 -lglu32 -lglfw3dll

# Adding linked libraries for ImGui
IDFLAGS += -I$(IMGUI_INC) -I$(IMGUI_SRC)

ICON = resources\icon.res

$(EXE): $(OBJ)
		$(CC) $(CFLAGS) $(IDFLAGS) $(LDFLAGS) -o $(EXE) $(OBJ) $(LDLIBS) $(ICON)

clean:
		del $(OBJ)

clobber: clean
		del $(EXE) 

%.o:%.cpp
	$(CC) $(CFLAGS) $(IDFLAGS) -c "$<" -o "$@"
# $(CC) $(CFLAGS) -c -o $@ $<

%.o:$(IMGUI_INC)/%.cpp
	$(CC) $(CFLAGS) $(IDFLAGS) -c "$<" -o "$@"
# $(CC) $(CFLAGS) -c -o $@ $<

%.o:$(IMGUI_SRC)/%.cpp
	$(CC) $(CFLAGS) $(IDFLAGS) -c "$<" -o "$@"
# $(CC) $(CFLAGS) -c -o $@ $<

# main.o:	$(SRC)
# 	$(CC) $(CFLAGS) $(IDFLAGS) -c "$<" -o "$@"

%.o:$(GLAD)/%.c
	$(CC) $(CFLAGS) $(IDFLAGS) -c "$<" -o "$@"

# imgui.o: $(IMGUI_INC)/imgui.cpp
# 	$(CC) $(CFLAGS) $(IDFLAGS) -c "$<" -o "$@"

# Build the icon
icon.res: resources\icon.rc
	windres resources\icon.rc -O coff -o resources\icon.res

