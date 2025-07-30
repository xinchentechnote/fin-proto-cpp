// Copyright 2025 xinchentechnote
#pragma once
#include <zlib.h>

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "bytebuf.hpp"

class IChecksumService {
 public:
  virtual ~IChecksumService() = default;
  virtual std::string algorithm() const = 0;
};

template <typename Input, typename Output>
class ChecksumService : public IChecksumService {
 public:
  using input_type = Input;
  using output_type = Output;

  virtual Output calc(const Input& data) const = 0;
};

class Crc16ChecksumService : public ChecksumService<ByteBuf, uint16_t> {
 public:
  std::string algorithm() const override { return "CRC16"; }

  uint16_t calc(const ByteBuf& data) const override {
    uint16_t crc = 0xFFFF;
    for (uint8_t b : data.data()) {
      crc ^= static_cast<uint16_t>(b);
      for (int i = 0; i < 8; i++) {
        if (crc & 0x0001)
          crc = (crc >> 1) ^ 0xA001;
        else
          crc >>= 1;
      }
    }
    return crc;
  }
};

class Crc32ChecksumService : public ChecksumService<ByteBuf, uint32_t> {
 public:
  std::string algorithm() const override { return "CRC32"; }

  uint32_t calc(const ByteBuf& data) const override {
    uint32_t crc = crc32(0, data.data().data(), data.readable_bytes());
    return crc;
  }
};

class SseBinChecksumService : public ChecksumService<ByteBuf, uint32_t> {
 public:
  std::string algorithm() const override { return "SSE_BIN"; }

  uint32_t calc(const ByteBuf& data) const override {
    uint32_t checksum = 0;
    for (uint8_t b : data.data()) {
      checksum = (checksum + b) & 0xFF;
    }
    return checksum;
  }
};

class SzseBinChecksumService : public ChecksumService<ByteBuf, int32_t> {
 public:
  std::string algorithm() const override { return "SZSE_BIN"; }

  int32_t calc(const ByteBuf& data) const override {
    int32_t checksum = 0;
    for (uint8_t b : data.data()) {
      checksum += b;
    }
    return checksum % 256;
  }
};

class ChecksumServiceContext {
 public:
  static ChecksumServiceContext& instance() {
    static ChecksumServiceContext ctx;
    ctx.registerService(std::make_shared<Crc16ChecksumService>());
    ctx.registerService(std::make_shared<Crc32ChecksumService>());
    ctx.registerService(std::make_shared<SseBinChecksumService>());
    ctx.registerService(std::make_shared<SzseBinChecksumService>());
    return ctx;
  }

  bool registerService(std::shared_ptr<IChecksumService> service) {
    std::lock_guard<std::mutex> lock(mu_);
    auto algo = service->algorithm();
    if (services_.count(algo)) return false;
    services_[algo] = std::move(service);
    return true;
  }

  template <typename Input, typename Output>
  std::shared_ptr<ChecksumService<Input, Output>> get(const std::string& algo) {
    std::lock_guard<std::mutex> lock(mu_);
    auto it = services_.find(algo);
    if (it != services_.end()) {
      return std::dynamic_pointer_cast<ChecksumService<Input, Output>>(it->second);
    }
    return nullptr;
  }

 private:
  ChecksumServiceContext() = default;
  std::unordered_map<std::string, std::shared_ptr<IChecksumService>> services_;
  std::mutex mu_;
};