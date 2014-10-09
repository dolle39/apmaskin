USER_INFO_VALID_VARIANTS="DEBUG", "RELEASE", "UNIT_TEST", "HSIT_MASTER" of "HSIT_SLAVE"

CFLAGS += -Wall -Werror

# Variant santy-check and variant settings
ifeq ($(VARIANT), DEBUG)
  CC = $(BCM_CROSS_COMPILER)
  CFLAGS += -DACTIVATE_DEBUG_PRINTF
  CFLAGS += -ggdb -O0
  BUILD_OUTPUT_DIR = $(DEBUG_BUILD_OUTPUT_DIR)
else ifeq ($(VARIANT), RELEASE)
  CC = $(BCM_CROSS_COMPILER)
  BUILD_OUTPUT_DIR = $(RELEASE_BUILD_OUTPUT_DIR)
else ifeq ($(VARIANT), UNIT_TEST)
  CC = $(HOST_CC)
  AR = $(HOST_AR)
  UNIT_TEST_BINARIES =
  BUILD_OUTPUT_DIR = $(UNIT_TEST_BUILD_OUTPUT_DIR)
else ifeq ($(VARIANT), HSIT_MASTER)
  CC = $(HOST_CC)
  AR = $(HOST_AR)
  HSIT_MASTER_BINARIES =
  BUILD_OUTPUT_DIR = $(HSIT_MASTER_BUILD_OUTPUT_DIR)
else ifeq ($(VARIANT), HSIT_SLAVE)
  CC = $(BCM_CROSS_COMPILER)
  HSIT_SLAVE_BINARIES =
  BUILD_OUTPUT_DIR = $(HSIT_SLAVE_BUILD_OUTPUT_DIR)
else ifeq ($(MAKECMDGOALS),clean)
  # Do nothing - 'clean' does not require any 'VARIANT'
else
  $(error Variable 'VARIANT' must be $(USER_INFO_VALID_VARIANTS). VARIANT=$(VARIANT))
endif

CFLAGS += -DVARIANT=\"$(VARIANT)\"