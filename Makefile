# AI Generated Makefile

PY  ?= python
CXX ?= g++

OBJ_DIR := build
OUT_DIR := simulation
MODULE  := simulation

# Sources
PYBIND_SRC := pybind/pybind.cc
SRC_CC     := $(wildcard src/*.cc)
SRCS       := $(PYBIND_SRC) $(SRC_CC)

# Objects mirror source tree under build/
OBJS := $(patsubst %.cc,$(OBJ_DIR)/%.o,$(SRCS))

# Python/pybind flags
PY_INCLUDES := $(shell $(PY) -m pybind11 --includes)
EXT_SUFFIX  := $(shell $(PY) -c "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX') or '.pyd')")

# Python import library - handles both UCRT64 (.dll.a) and Windows python.org (.lib)
PY_LINK := $(shell $(PY) -c "\
import sysconfig, os; \
d = sysconfig.get_config_var('LIBDIR') or ''; \
l = sysconfig.get_config_var('LDLIBRARY') or ''; \
l = l[:-4] + '.lib' if l.endswith('.dll') else l; \
print(os.path.join(d, l).replace(chr(92), '/'))")

TARGET := $(OUT_DIR)/$(MODULE)$(EXT_SUFFIX)

INCLUDES := -Iinclude -Isrc -Ipybind
CXXFLAGS := -O3 -Wall -Wextra -std=c++17 $(INCLUDES) $(PY_INCLUDES)
LDFLAGS  := -shared -static-libgcc -static-libstdc++
LDLIBS   := -Wl,-Bstatic -lpthread -Wl,-Bdynamic

.PHONY: all clean info

all: info $(TARGET)

info:
	@echo "PY=$(PY)"
	@echo "EXT_SUFFIX=$(EXT_SUFFIX)"
	@echo "PY_INCLUDES=$(PY_INCLUDES)"
	@echo "PY_LINK=$(PY_LINK)"

$(OUT_DIR):
	@mkdir -p $(OUT_DIR)

$(OBJ_DIR)/%.o: %.cc | $(OUT_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(PY_LINK) $(LDLIBS) -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(OUT_DIR)/$(MODULE).*\.pyd $(OUT_DIR)/$(MODULE).*\.so $(OUT_DIR)/$(MODULE).*\.dll
	rm -f $(OUT_DIR)/$(MODULE).*.pyd $(OUT_DIR)/$(MODULE).*.so $(OUT_DIR)/$(MODULE).*.dll