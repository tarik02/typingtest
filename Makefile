LOCALE ?= en

CC ?= cc
CFLAGS += -std=c99 -W -Wall -Werror
LDFLAGS += -lncursesw -lm

SOURCES = locale/messages_$(LOCALE).c locale/conclusions.c str.c list.c util.c words.c platform.c typingtest.c
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)
DEPENDENCIES = $(OBJECTS:%.o=%.d)

TARGET = typingtest
PREFIX ?= /usr/local
BUILD_DIR ?= build

.PHONY: all clean install uninstall

all: $(TARGET)

clean:
	@rm -rf $(BUILD_DIR)
install:
	install $(TARGET) $(PREFIX)/bin
uninstall:
	rm -f $(PREFIX)/bin/$(TARGEgT)


$(TARGET): $(BUILD_DIR)/$(TARGET)
	cp $^ $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

-include $(DEPENDENCIES)

$(BUILD_DIR)/%.d: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -MM -MT $(patsubst %.d,%.o,$@) -MF $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
