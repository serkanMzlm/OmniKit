# ANSI color codes
BLACK := \033[0;31
RED := \033[0;31m
GREEN := \033[0;32m
YELLOW := \033[0;33m
BLUE := \033[0;34m
PURPLE := 033[0;35m
CYAN := 033[0;36m
WHITE := 033[0;37m
RESET := \033[0m

# Target directories
BUILD_DIR := build
INSTALL_DIR := install

# CMake and Make options
CMAKE_OPTIONS := -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(abspath $(INSTALL_DIR))
MAKE_OPTIONS := --no-print-directory

.PHONY: all
all: $(BUILD_DIR)/Makefile
	@echo "Building the project..."
	@$(MAKE) $(MAKE_OPTIONS) -C $(BUILD_DIR)
	@echo "Installing the project..."
	@$(MAKE) $(MAKE_OPTIONS) -C $(BUILD_DIR) install

$(BUILD_DIR)/Makefile:
	@echo "Creating build directory if not exists..."
	@mkdir -p $(BUILD_DIR)
	@echo "Configuring the project with CMake..."
	@cd $(BUILD_DIR) && cmake $(CMAKE_OPTIONS) .. || { echo "CMake configuration failed!"; exit 1; }

.PHONY: clean
clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD_DIR) $(INSTALL_DIR)
	@echo "Clean complete."

.PHONY: rebuild
rebuild: clean all

reconfigure:
	@echo "Reconfiguring the project..."
	@cd $(BUILD_DIR) && cmake $(CMAKE_OPTIONS) .. || { echo "CMake reconfiguration failed!"; exit 1; }

.PHONY: build
build:
	@echo "\033[0;32mBuilding the project...\033[0m"
	@echo "Building the project without reconfiguring..."
	@$(MAKE) $(MAKE_OPTIONS) -C $(BUILD_DIR)

.PHONY: install
install:
	@echo "Installing the project..."
	@$(MAKE) $(MAKE_OPTIONS) -C $(BUILD_DIR) install