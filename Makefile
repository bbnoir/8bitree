# tool macros
CXXFLAGS += -std=c++11 -O1 
DBGFLAGS := -g
CCOBJFLAGS := $(CXXFLAGS) -c

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

# compile macros
TARGET_NAMES := main test # only modify this line to add new target
ifeq ($(OS),Windows_NT)
	TARGET_NAMES := $(addsuffix .exe,$(TARGET_NAMES))
endif
TARGETS := $(foreach target, $(TARGET_NAMES), $(BIN_PATH)/$(target))
TARGETS_OBJ := $(addsuffix .o, $(foreach target, $(TARGET_NAMES), $(OBJ_PATH)/$(target)))

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ := $(filter-out $(TARGETS_OBJ), $(OBJ))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG)
CLEAN_LIST := $(TARGETS) \
			  $(TARGET_DEBUG) \
			  $(DISTCLEAN_LIST)

# default rule
default: makedir all

# non-phony targets
$(BIN_PATH)/main: $(OBJ) $(OBJ_PATH)/main.o
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(OBJ_PATH)/main.o

$(BIN_PATH)/test: $(OBJ) $(OBJ_PATH)/test.o
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(OBJ_PATH)/test.o

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CXX) $(CCOBJFLAGS) -o $@ $<

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) $(OBJ_DEBUG) -o $@

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) 

.PHONY: all
all: $(TARGETS)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)