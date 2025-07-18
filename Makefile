# Makefile for fin-protoc project

# Variables
PROTO_DSL := ../submodules/fin-proto/sample/sample.pdsl
PROTO_DIR := proto/
OUTPUT_DIR := ./
BIN_DIR := ~/workspace/fin-protoc/bin/

.PHONY: all compile format fix clean

all: compile

build: compile

set_env:
	export PATH=$(BIN_DIR):$$PATH

compile:
	@echo "Compiling protocol..."
	$(BIN_DIR)/fin-protoc compile -f submodules/fin-proto/sample/sample.pdsl -c $(OUTPUT_DIR)
