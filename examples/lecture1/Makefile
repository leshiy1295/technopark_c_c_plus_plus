ifeq ("$(STD)", "")
	STD_OPTION =
else
	STD_OPTION = -std=gnu$(STD)
endif
ADDITIONAL_OPTIONS ?=
OPTIONS ?= $(ADDITIONAL_OPTIONS) $(STD_OPTION) -Wall -Wpedantic
OUT_FILE ?= a.out

default: run clean_after

clean: clean_before
	

clean%:
	@rm -f $(OUT_FILE)

build: clean_before
	gcc $(FILE) $(OPTIONS) -o $(OUT_FILE)

run: build
	./$(OUT_FILE) $(ARGS)
