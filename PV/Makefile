# Makefile for C++ Semaphore Project

CXX = g++
CXXFLAGS = -Wall -g -pthread
TARGET = test
BUILD_DIR = build
OBJS = $(BUILD_DIR)/Semaphore.o $(BUILD_DIR)/expr.o $(BUILD_DIR)/P.o $(BUILD_DIR)/V.o $(BUILD_DIR)/log.o $(BUILD_DIR)/thread.o $(BUILD_DIR)/test.o

.PHONY: all clean bundle

all: $(BUILD_DIR) $(TARGET)

# 自动检测依赖顺序并生成 PV.hpp
# 使用 PV.h 中的 include 顺序作为打包顺序标准

# 从 PV.h 中提取包含顺序（去掉注释和空行）
HEADER_ORDER := $(shell grep '#include' PV.h | grep -v '//' | sed 's/#include "//g; s/"//g')

# 生成 cpp 文件列表（只包含存在对应 .cpp 的头文件）
CPP_ORDER := $(patsubst %.h,%.cpp,$(filter %.h,$(HEADER_ORDER)))
CPP_ORDER := $(foreach cpp,$(CPP_ORDER),$(if $(wildcard $(cpp)),$(cpp),))

# 所有参与打包的文件（按依赖顺序）
BUNDLE_FILES := $(HEADER_ORDER) $(CPP_ORDER)

bundle: ../PV.hpp

../PV.hpp: PV.h $(BUNDLE_FILES)
	@echo "Generating PV.hpp based on dependency order from PV.h..."
	@echo "Header order: $(HEADER_ORDER)"
	@echo "/* ============================================" > $@
	@echo " * PV.hpp - Combined Single-File Library" >> $@
	@echo " * Generated automatically from source files" >> $@
	@echo " * Dependency order from PV.h" >> $@
	@echo " * ============================================ */" >> $@
	@echo "" >> $@
	@echo "#ifndef PV_HPP" >> $@
	@echo "#define PV_HPP" >> $@
	@echo "" >> $@
	@echo "/* ============================================" >> $@
	@echo " * DECLARATIONS (Headers in dependency order)" >> $@
	@echo " * ============================================ */" >> $@
	@echo "" >> $@
	@for header in $(HEADER_ORDER); do \
		echo "/* ---------- $$header ---------- */" >> $@; \
		sed 's/#include.*".*"//g' $$header >> $@; \
		echo "" >> $@; \
	done
	@echo "/* ============================================" >> $@
	@echo " * IMPLEMENTATIONS (CPP files in dependency order)" >> $@
	@echo " * ============================================ */" >> $@
	@echo "" >> $@
	@for cpp in $(CPP_ORDER); do \
		if [ -f $$cpp ]; then \
			echo "/* ---------- $$cpp ---------- */" >> $@; \
			sed 's/#include.*".*"//g' $$cpp | sed '/^$$/d' >> $@; \
			echo "" >> $@; \
		fi \
	done
	@echo "#endif // PV_HPP" >> $@
	@echo "" >> $@
	@echo "/* ============================================" >> $@
	@echo " * END OF PV.hpp" >> $@
	@echo " * ============================================ */" >> $@
	@echo "PV.hpp generated successfully!"
	@echo "Files included: $(BUNDLE_FILES)"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/Semaphore.o: Semaphore.cpp Semaphore.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/expr.o: expr.cpp expr.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/P.o: P.cpp P.h Semaphore.h expr.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/V.o: V.cpp V.h Semaphore.h expr.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/log.o: log.cpp log.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/thread.o: thread.cpp thread.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test.o: test.cpp P.h V.h Semaphore.h log.h thread.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
