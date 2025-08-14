// Copyright 2025 xinchentechnote
#pragma once

#include <endian.h>

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <type_traits>
#include <vector>

class ByteBuf {
 public:
  ByteBuf(size_t capacity = 256) : buffer_(), reader_index_(0), writer_index_(0) {
    buffer_.reserve(capacity);
  }

  void write_u8(uint8_t val) { buffer_.push_back(val); }
  void write_u8_at(size_t pos, uint8_t val) { write_at(pos, val); }
  uint8_t read_u8() {
    check_read(1);
    return buffer_[reader_index_++];
  }

  void write_u8_le(uint8_t val) { buffer_.push_back(val); }
  void write_u8_le_at(size_t pos, uint8_t val) { write_le_at(pos, val); }
  uint8_t read_u8_le() {
    check_read(1);
    return buffer_[reader_index_++];
  }

  void write_u16(uint16_t val) { write(val); }
  void write_u16_at(size_t pos, uint16_t val) { write_at(pos, val); }
  uint16_t read_u16() {
    check_read(2);
    return read<uint16_t>();
  }

  void write_u16_le(uint16_t val) { write_le(val); }
  void write_u16_le_at(size_t pos, uint16_t val) { write_le_at(pos, val); }
  uint16_t read_u16_le() {
    check_read(2);
    return read_le<uint16_t>();
  }

  void write_u32(uint32_t val) { write(val); }
  void write_u32_at(size_t pos, uint32_t val) { write_at(pos, val); }
  uint32_t read_u32() {
    check_read(4);
    return read<uint32_t>();
  }

  void write_u32_le(uint32_t val) { write_le(val); }
  void write_u32_le_at(size_t pos, uint32_t val) { write_le_at(pos, val); }
  uint32_t read_u32_le() {
    check_read(4);
    return read_le<uint32_t>();
  }

  void write_u64(uint64_t val) { write(val); }
  void write_u64_at(size_t pos, uint64_t val) { write_at(pos, val); }
  uint64_t read_u64() {
    check_read(8);
    return read<uint64_t>();
  }

  void write_u64_le(uint64_t val) { write_le(val); }
  void write_u64_le_at(size_t pos, uint64_t val) { write_le_at(pos, val); }
  uint64_t read_u64_le() {
    check_read(8);
    return read_le<uint64_t>();
  }

  void write_i8(int8_t val) { buffer_.push_back(static_cast<uint8_t>(val)); }
  void write_i8_at(size_t pos, int8_t val) { write_at(pos, val); }
  int8_t read_i8() {
    check_read(1);
    return static_cast<int8_t>(buffer_[reader_index_++]);
  }

  void write_i8_le(int8_t val) { buffer_.push_back(static_cast<uint8_t>(val)); }
  void write_i8_le_at(size_t pos, int8_t val) { write_le_at(pos, val); }
  int8_t read_i8_le() {
    check_read(1);
    return static_cast<int8_t>(buffer_[reader_index_++]);
  }
  void write_i16(int16_t val) { write(val); }
  void write_i6_at(size_t pos, int16_t val) { write_at(pos, val); }
  int16_t read_i16() {
    check_read(2);
    return read<int16_t>();
  }

  void write_i16_le(int16_t val) { write_le(val); }
  void write_i16_le_at(size_t pos, int16_t val) { write_at(pos, val); }
  int16_t read_i16_le() {
    check_read(2);
    return read_le<int16_t>();
  }

  void write_i32(int32_t val) { write(val); }
  void write_i32_at(size_t pos, int32_t val) { write_at(pos, val); }
  int32_t read_i32() {
    check_read(4);
    return read<int32_t>();
  }

  void write_i32_le(int32_t val) { write_le(val); }

  void write_i32_le_at(size_t pos, int32_t val) { write_le_at(pos, val); }
  int32_t read_i32_le() {
    check_read(4);
    return read_le<int32_t>();
  }

  void write_i64(int64_t val) { write(val); }
  void write_i64_at(size_t pos, int64_t val) { write_at(pos, val); }
  int64_t read_i64() {
    check_read(8);
    return read<int64_t>();
  }

  void write_i64_le(int64_t val) { write_le(val); }
  void write_i64_le_at(size_t pos, int64_t val) { write_le_at(pos, val); }
  int64_t read_i64_le() {
    check_read(8);
    return read_le<int64_t>();
  }

  void write_f32(float val) { write(val); }
  float read_f32() {
    check_read(4);
    return read<float>();
  }

  void write_f32_le(float val) { write_le(val); }
  float read_f32_le() {
    check_read(4);
    return read_le<float>();
  }

  void write_f64(double val) { write(val); }
  double read_f64() {
    check_read(8);
    return read<double>();
  }

  void write_f64_le(double val) { write_le(val); }
  double read_f64_le() {
    check_read(8);
    return read<double>();
  }

  void write_bytes(const void* data, size_t len) {
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    buffer_.insert(buffer_.end(), bytes, bytes + len);
  }
  void read_bytes(void* dest, size_t len) {
    check_read(len);
    std::memcpy(dest, &buffer_[reader_index_], len);
    reader_index_ += len;
  }

  const std::vector<uint8_t>& data() const { return buffer_; }

  void reset() {
    buffer_.clear();
    reader_index_ = 0;
    writer_index_ = 0;
  }

  size_t readable_bytes() const { return buffer_.size() - reader_index_; }

  // 修改后的模板函数
  template <typename T>
  void write_le(T value) {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");
    if constexpr (std::is_floating_point_v<T>) {
      // 浮点数的处理方式
      uint8_t bytes[sizeof(T)];
      std::memcpy(bytes, &value, sizeof(T));
      write_bytes(bytes, sizeof(T));
      writer_index_ += sizeof(T);
    } else {
      // 整数的处理方式
      uint8_t bytes[sizeof(T)];
      for (size_t i = 0; i < sizeof(T); ++i) {
        bytes[i] = static_cast<uint8_t>(value >> (i * 8));
      }
      write_bytes(bytes, sizeof(T));
      writer_index_ += sizeof(T);
    }
  }

  template <typename T>
  void write_be(T value) {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");
    if constexpr (std::is_floating_point_v<T>) {
      // 浮点数的处理方式
      uint8_t bytes[sizeof(T)];
      std::memcpy(bytes, &value, sizeof(T));
      write_bytes(bytes, sizeof(T));
      writer_index_ += sizeof(T);
    } else {
      // 整数的处理方式
      uint8_t bytes[sizeof(T)];
      for (size_t i = 0; i < sizeof(T); ++i) {
        bytes[i] = static_cast<uint8_t>(value >> ((sizeof(T) - 1 - i) * 8));
      }
      write_bytes(bytes, sizeof(T));
      writer_index_ += sizeof(T);
    }
  }

  template <typename T>
  void write_le_at(size_t pos, T value) {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");

    if (pos + sizeof(T) > buffer_.size()) {
      throw std::out_of_range("Position out of bounds for write_le_at");
    }

    if constexpr (std::is_floating_point_v<T>) {
      uint8_t bytes[sizeof(T)];
      std::memcpy(bytes, &value, sizeof(T));
      for (size_t i = 0; i < sizeof(T); ++i) {
        buffer_[pos + i] = bytes[i];
      }
    } else {
      for (size_t i = 0; i < sizeof(T); ++i) {
        buffer_[pos + i] = static_cast<uint8_t>(value >> (i * 8));
      }
    }
  }

  template <typename T>
  void write_be_at(size_t pos, T value) {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");

    if (pos + sizeof(T) > buffer_.size()) {
      throw std::out_of_range("Position out of bounds for write_be_at");
    }

    if constexpr (std::is_floating_point_v<T>) {
      uint8_t bytes[sizeof(T)];
      std::memcpy(bytes, &value, sizeof(T));
      for (size_t i = 0; i < sizeof(T); ++i) {
        buffer_[pos + i] = bytes[i];
      }
    } else {
      for (size_t i = 0; i < sizeof(T); ++i) {
        buffer_[pos + i] = static_cast<uint8_t>(value >> ((sizeof(T) - 1 - i) * 8));
      }
    }
  }

  template <typename T>
  T read_le() {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");
    check_read(sizeof(T));
    if constexpr (std::is_floating_point_v<T>) {
      // 浮点数的处理方式
      T value;
      std::memcpy(&value, &buffer_[reader_index_], sizeof(T));
      reader_index_ += sizeof(T);
      return value;
    } else {
      // 整数的处理方式
      T value = 0;
      for (size_t i = 0; i < sizeof(T); ++i) {
        value |= static_cast<T>(buffer_[reader_index_ + i]) << (i * 8);
      }
      reader_index_ += sizeof(T);
      return value;
    }
  }

  template <typename T>
  T read_be() {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");
    check_read(sizeof(T));
    if constexpr (std::is_floating_point_v<T>) {
      // 浮点数的处理方式
      T value;
      uint8_t bytes[sizeof(T)];
      std::memcpy(bytes, &buffer_[reader_index_], sizeof(T));
      std::memcpy(&value, bytes, sizeof(T));
      reader_index_ += sizeof(T);
      return value;
    } else {
      // 整数的处理方式
      T value = 0;
      for (size_t i = 0; i < sizeof(T); ++i) {
        value |= static_cast<T>(buffer_[reader_index_ + i]) << ((sizeof(T) - 1 - i) * 8);
      }
      reader_index_ += sizeof(T);
      return value;
    }
  }

  // 通用写入函数，默认小端序
  template <typename T>
  void write(T value, bool big_endian = false) {
    if (big_endian) {
      write_be(value);
    } else {
      write_le(value);
    }
  }
  template <typename T>
  void write_at(size_t pos, T value, bool big_endian = false) {
    if (big_endian) {
      write_be_at(pos, value);
    } else {
      write_le_at(pos, value);
    }
  }

  // 通用读取函数，默认小端序
  template <typename T>
  T read(bool big_endian = false) {
    if (big_endian) {
      return read_be<T>();
    } else {
      return read_le<T>();
    }
  }

  void check_read(size_t required) {
    if (readable_bytes() < required) {
      throw std::out_of_range("Not enough data to read");
    }
  }

  size_t writer_index() const { return writer_index_; }

 private:
  std::vector<uint8_t> buffer_;
  size_t reader_index_;
  size_t writer_index_;
};
