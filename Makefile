# tool macros
CXXFLAGS += -std=c++11 -O3 -fopenmp -mavx2 
DBGFLAGS := -g
CCOBJFLAGS := $(CXXFLAGS) -c

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

# compile macros
TARGET_NAMES := main verify test txt2bin # only modify this line to add new target
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
			  $(TARGETS_OBJ) \
			  $(DISTCLEAN_LIST)

# default rule
default: makedir all
.SECONDARY: $(OBJ) $(TARGETS_OBJ)

# non-phony targets
$(BIN_PATH)/%: $(OBJ) $(OBJ_PATH)/%.o
	$(CXX) $(CXXFLAGS) -o $@ $^

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
