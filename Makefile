# 编译器与选项
CXX      := g++
CXXFLAGS := -std=c++17 -Wall

# 源、目标、输出目录
SRCDIR   := src
INCDIR   := include
OBJDIR   := build/obj
BINDIR   := build/bin

SRCS     := $(wildcard $(SRCDIR)/*.cpp)
OBJS     := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS     := $(OBJS:.o=.d)

TARGET   := calculator

# 默认目标
.PHONY: all
all: $(BINDIR)/$(TARGET)

# 链接可执行文件
$(BINDIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $^ -o $@

# 编译 .cpp -> .o，并生成依赖文件 .d
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# 包含自动生成的依赖
-include $(DEPS)

# 清理构建产物
.PHONY: clean
clean:
	rm -rf build/

# 安装（可选）
.PHONY: install
install: all
	@echo "Installing..."
	# cp $(BINDIR)/$(TARGET) /usr/local/bin/

# 伪目标，避免文件同名冲突
.PHONY: distclean
distclean: clean
	rm -f $(TARGET)
