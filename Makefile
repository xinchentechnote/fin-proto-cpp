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
	$(BIN_DIR)/fin-protoc -f submodules/fin-proto/bse/bse_trade_bin_v0.9.pdsl -c $(OUTPUT_DIR)
	$(BIN_DIR)/fin-protoc -f submodules/fin-proto/sample/sample.pdsl -c $(OUTPUT_DIR)
	$(BIN_DIR)/fin-protoc -f submodules/fin-proto/risk/risk_v0.1.0.pdsl -c $(OUTPUT_DIR)
	$(BIN_DIR)/fin-protoc -f submodules/fin-proto/sse/binary/sse_bin_v0.57.pdsl -c $(OUTPUT_DIR)
	$(BIN_DIR)/fin-protoc -f submodules/fin-proto/szse/binary/szse_bin_v1.29.pdsl -c $(OUTPUT_DIR)
