# ANSI color codes
BLACK := \033[0;31
RED := \033[0;31m
GREEN := \033[0;32m
YELLOW := \033[0;33m
BLUE := \033[0;34m
PURPLE := \033[0;35m
CYAN := \033[0;36m
WHITE := \033[0;37m
RESET := \033[0m

# Target directories
BUILD_DIR := build
INSTALL_DIR := install

# CMake and Make options
CMAKE_OPTIONS := -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(abspath $(INSTALL_DIR))
MAKE_OPTIONS := --no-print-directory

.PHONY: all
all: ${BUILD_DIR}/Makefile build install update_bashrc

$(BUILD_DIR)/Makefile:
	@echo "Creating build directory if not exists..."
	@mkdir -p $(BUILD_DIR)
	@echo "Configuring the project with CMake..."
	@cd $(BUILD_DIR) && cmake $(CMAKE_OPTIONS) .. || { echo "$(RED)CMake configuration failed!$(RESET)"; exit 1; }

.PHONY: update_bashrc
update_bashrc:
	@DRIVE_DIR=$$(pwd)/install/lib/cmake; \
	if grep -q "export OMNIKIT_PATH=" ~/.bashrc; then \
		sed -i "s|^export OMNIKIT_PATH=.*|export OMNIKIT_PATH=$$DRIVE_DIR|" ~/.bashrc; \
	else \
		echo "export OMNIKIT_PATH=$$DRIVE_DIR" >> ~/.bashrc; \
	fi; 
	@echo "$(GREEN)Updating .bashrc with current directory...$(RESET)"
	@. ~/.bashrc

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(INSTALL_DIR)
	@echo "$(RED)Cleaning up..."
	@echo "Clean complete.$(RESET)"

.PHONY: rebuild
rebuild: clean all

reconfigure:
	@echo "Reconfiguring the project..."
	@cd $(BUILD_DIR) && cmake $(CMAKE_OPTIONS) .. || { echo "$(RED)CMake reconfiguration failed!$(RESET)"; exit 1; }

.PHONY: build
build:
	@echo "$(GREEN)Building the project..."
	@echo "Building the project without reconfiguring...$(RESET)"
	@$(MAKE) $(MAKE_OPTIONS) -C $(BUILD_DIR)

.PHONY: install
install:
	@$(MAKE) $(MAKE_OPTIONS) -C $(BUILD_DIR) install