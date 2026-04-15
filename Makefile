# 408 Project Root Makefile
# 自动调用 PV 目录生成 PV.hpp

.PHONY: all bundle clean

# 默认目标：生成 PV.hpp
all: bundle

# 调用 PV 目录生成 PV.hpp
bundle:
	@echo "Building PV.hpp..."
	$(MAKE) -C PV bundle

# 清理
clean:
	rm -f PV.hpp
	$(MAKE) -C PV clean
