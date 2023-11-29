BUILD_DIR=build
LIB_DIR=$(BUILD_DIR)/lib
OBJ_DIR=$(BUILD_DIR)/obj
BIN_DIR=$(BUILD_DIR)/bin
CC := gcc
CC_FLAGS := -g -Wall -std=c11
LD_FLAGS :=

src-to-obj = $(addprefix $(OBJ_DIR)/,$(subst .c,.o,$1))
src-to-dep = $(addprefix $(OBJ_DIR)/,$(subst .c,.d,$1))

define make-lib
.PHONY += $1
lib-$1-include-dirs := $3
lib-$1-lib := $(LIB_DIR)/lib$1.a
lib-$1-src := $(wildcard $2/*.c) $(wildcard $2/**/*.c)

-include $$(call src-to-dep,$$(lib-$1-src))

$$(lib-$1-lib): $$(call src-to-obj,$$(lib-$1-src))
	@mkdir -p $$(@D)
	@echo "(AR) $$@"
	@$(AR) rcs $$@ $$^

$(OBJ_DIR)/$2/%.o: $2/%.c
	@mkdir -p $$(@D)
	@echo "(CC) $$@"
	@$(CC) $(CC_FLAGS) -MMD -c $$< -o $$@ $$(addprefix -I,$$(lib-$1-include-dirs))

$1: $$(lib-$1-lib)
endef

define make-bin
.PHONY += $1
BINS += $1
bin-$1-include-dirs := $3 $($(patsubst %,lib-%-include-dirs,$4))
bin-$1-libs := $($(patsubst %,lib-%-lib,$4))
bin-$1-bin := $(BIN_DIR)/$1

bin-$1-src := $(wildcard $2/*.c) $(wildcard $2/**/*.c)

-include $$(call src-to-dep,$$(bin-$1-src))

$$(bin-$1-bin): $$(call src-to-obj,$$(bin-$1-src)) $$(bin-$1-libs)
	@mkdir -p $$(@D)
	@echo "(LD) $$@"
	@$(CC) $(LD_FLAGS) -o $$@ $$^ $5

$(OBJ_DIR)/$2/%.o: $2/%.c
	@mkdir -p $$(@D)
	@echo "(CC) $$@"
	@$(CC) $(CC_FLAGS) -MMD -c $$< -o $$@ $$(addprefix -I,$$(bin-$1-include-dirs))

$1: $$(bin-$1-bin)
endef

all: bins

-include lib/module.mk
-include client/module.mk
-include server/module.mk

bins: $(BINS)

clean:
	rm -rf $(BUILD_DIR)
