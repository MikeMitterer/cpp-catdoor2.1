# Self-Documented Makefile see https://marmelab.com/blog/2016/02/29/auto-documented-makefile.html
#
# Basics zum Makefile:
#	https://makefiletutorial.com/#getting-started
#
# You can set these variables from the command line.
# BUILDDIR      = _build

# https://www.chiark.greenend.org.uk/doc/make-doc/make.html/Recipes.html#One-Shell
# .ONESHELL:

# Explizit setzt, denn unter Debian ist /bin/sh nicht gleich /bin/bash
# (per default ist /bin/sh ein symlink auf /bin/dash)
SHELL       := /bin/bash
# .SHELLFLAGS := -eu -o pipefail -c

.PHONY: help libs build deploy

.DEFAULT_GOAL := help

# Python
# init init312: ENVIRONMENT := ".vMLUtils"
# ENVIRONMENT   	:= ".vPetShopSample"

# Definiert in tests/unit/_resources/settings.toml
# UPLOAD_PATH     := "tests/unit/_resources/uploads"

DEPENDENCIES := ".dependencies"

include ${DEV_MAKE}/colours.mk

help:
	@echo
	@echo "Please use \`make <${YELLOW}target${RESET}>' where <target> is one of"
	@echo
	@echo "    ${YELLOW}help       ${GREEN}This help message${RESET}"
	@echo
	@echo "    ${YELLOW}libs       ${GREEN}Verlinkt die internen Libs${RESET}"
#	@echo "    ${YELLOW}build      ${GREEN}Build your setup${RESET}"
#	@echo "    ${YELLOW}deploy     ${GREEN}Package wird gebaut. ${WHITE}'git push' muss per Hand ausgeführt werden!${RESET}"
	@echo
	@echo "${BLUE}Hints:${NC}"
	@echo

colors:
	$(call print_colours)

libs:
	@cd $(DEPENDENCIES) && ln -sfn ../../../libs/oneled oneled

build:
	@echo Build called

deploy: build

all: deploy