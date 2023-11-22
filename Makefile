	# Doc compiling variables
# Code compiling variables
ECHO = echo
GCC = clang
CCFLAGS = -g -ansi -pedantic -Wall -std=c11 -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = -g -fsanitize=address -fno-omit-frame-pointer -lncurses
EXE_NAME = awale
SOURCEDIR=src
BUILDDIR=build
BINDIR=bin
EXE = $(BINDIR)/$(EXE_NAME)
SOURCES = $(wildcard $(SOURCEDIR)/**/*.c) $(wildcard $(SOURCEDIR)/*.c)
OBJECTS = $(SOURCES:$(SOURCEDIR)/%.c=$(BUILDDIR)/%.o)
DEP=$(OBJECTS:%.o=%.d)

.PHONY = run code

all: code

code: $(EXE)

run: code
	@./$(EXE)

# Doc compiling rules

# Clean up
clean:
	rm $(DEP) $(OBJECTS) $(EXE)

# Code rules
$(EXE) : $(OBJECTS)
	@mkdir -p $(@D)
	@$(GCC) -o $(@) $(OBJECTS) $(LDFLAGS)

-include $(DEP)

$(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
	@mkdir -p $(@D)
	@$(ECHO) "Compiling $<..."
	@$(GCC) $(CCFLAGS) -MMD -c $< -o $@

dir: 
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BINDIR)
