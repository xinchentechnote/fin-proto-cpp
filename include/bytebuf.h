#pragma once

#include <cstdint>
#include <cstring>
#include <endian.h>
#include <stdexcept>
#include <type_traits>
#include <vector>

class ByteBuf {
public:
  ByteBuf(size_t capacity = 256);

  void writeU8(uint8_t val);
  uint8_t readU8();

  void writeU16(uint16_t val);
  uint16_t readU16();

  void writeU32(uint32_t val);
  uint32_t readU32();

  void writeU64(uint64_t val);
  uint64_t readU64();

  void writeBytes(const void *data, size_t len);
  void readBytes(void *dst, size_t len);

  template <typename T> void writeLE(T val) {
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic");

    if constexpr (sizeof(T) == 1) {
      // int8_t and uint8_t are the same in memory, write as byte
      writeU8(static_cast<uint8_t>(val));
    } else if constexpr (sizeof(T) == 2) {
      // Signed or unsigned 16-bit
      using U = std::make_unsigned_t<T>;
      writeU16(static_cast<U>(val));
    } else if constexpr (sizeof(T) == 4) {
      if constexpr (std::is_same_v<T, float>) {
        // Bitwise write for float
        uint32_t bits;
        std::memcpy(&bits, &val, sizeof(bits));
        writeU32(bits);
      } else {
        using U = std::make_unsigned_t<T>;
        writeU32(static_cast<U>(val));
      }
    } else if constexpr (sizeof(T) == 8) {
      if constexpr (std::is_same_v<T, double>) {
        // Bitwise write for double
        uint64_t bits;
        std::memcpy(&bits, &val, sizeof(bits));
        writeU64(bits);
      } else {
        using U = std::make_unsigned_t<T>;
        writeU64(static_cast<U>(val));
      }
    } else {
      static_assert(sizeof(T) == 0, "Unsupported type size");
    }
  }

  template <typename T> T readLE() {
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic");

    if constexpr (sizeof(T) == 1) {
      return static_cast<T>(readU8());
    } else if constexpr (sizeof(T) == 2) {
      uint16_t val = readU16();
      return static_cast<T>(val);
    } else if constexpr (sizeof(T) == 4) {
      if constexpr (std::is_same_v<T, float>) {
        uint32_t bits = readU32();
        float val;
        std::memcpy(&val, &bits, sizeof(val));
        return val;
      } else {
        uint32_t val = readU32();
        return static_cast<T>(val);
      }
    } else if constexpr (sizeof(T) == 8) {
      if constexpr (std::is_same_v<T, double>) {
        uint64_t bits = readU64();
        double val;
        std::memcpy(&val, &bits, sizeof(val));
        return val;
      } else {
        uint64_t val = readU64();
        return static_cast<T>(val);
      }
    } else {
      static_assert(sizeof(T) == 0, "Unsupported type size");
    }
  }

  const std::vector<uint8_t> &data() const { return buffer_; }

  void reset();

  size_t readableBytes() const;
  size_t writableBytes() const;

private:
  std::vector<uint8_t> buffer_;
  size_t readerIndex_;
  size_t writerIndex_;

  void ensureWritable(size_t len);
};
