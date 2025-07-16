// Copyright 2025 xinchentechnote
#pragma once

#include <string>
#include <type_traits>
#include <vector>

#include "bytebuf.hpp"

namespace codec {

// ----------------------------
// BinaryCodec Interface
// ----------------------------
struct BinaryCodec {
  virtual void encode(ByteBuf& buf) const = 0;
  virtual void decode(ByteBuf& buf) = 0;
  virtual std::string toString() const = 0;
  virtual ~BinaryCodec() = default;
};

// ----------------------------
// put/get String LE
// ----------------------------
template <typename T>
void putStringLE(ByteBuf& buf, const std::string& s) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  buf.write_le<T>(static_cast<T>(s.size()));
  buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
}

template <typename T>
std::string getStringLE(ByteBuf& buf) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  T length = buf.read_le<T>();
  std::vector<uint8_t> bytes(length);
  buf.read_bytes(bytes.data(), length);
  return std::string(reinterpret_cast<char*>(bytes.data()), length);
}

// ----------------------------
// Fixed-length string
// ----------------------------
inline void putFixedString(ByteBuf& buf, const std::string& s, size_t fixedLen) {
  if (s.size() >= fixedLen) {
    buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), fixedLen);
  } else {
    buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
    std::vector<uint8_t> padding(fixedLen - s.size(), 0);
    buf.write_bytes(padding.data(), padding.size());
  }
}

inline std::string getFixedString(ByteBuf& buf, size_t fixedLen) {
  std::vector<uint8_t> bytes(fixedLen);
  buf.read_bytes(bytes.data(), fixedLen);
  std::string s(reinterpret_cast<char*>(bytes.data()), fixedLen);
  size_t end = s.find_last_not_of('\0');
  if (end != std::string::npos) {
    s.erase(end + 1);
  } else {
    s.clear();
  }
  return s;
}

// ----------------------------
// String List
// ----------------------------
template <typename T, typename K>
void putStringListLE(ByteBuf& buf, const std::vector<std::string>& list) {
  static_assert(std::is_unsigned<T>::value && std::is_unsigned<K>::value,
                "T and K must be unsigned");
  buf.write_le<T>(static_cast<T>(list.size()));
  for (const auto& s : list) {
    buf.write_le<K>(static_cast<K>(s.size()));
    buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
  }
}

template <typename T, typename K>
std::vector<std::string> getStringListLE(ByteBuf& buf) {
  static_assert(std::is_unsigned<T>::value && std::is_unsigned<K>::value,
                "T and K must be unsigned");
  T count = buf.read_le<T>();
  std::vector<std::string> result;
  result.reserve(count);

  for (size_t i = 0; i < count; ++i) {
    K len = buf.read_le<K>();
    std::vector<uint8_t> bytes(len);
    buf.read_bytes(bytes.data(), len);
    result.emplace_back(reinterpret_cast<char*>(bytes.data()), len);
  }
  return result;
}

// ----------------------------
// Basic Type Read
// ----------------------------
template <typename T>
T getBasicType(ByteBuf& buf) {
  static_assert(std::is_arithmetic<T>::value, "T must be a basic type");
  return buf.read_le<T>();
}

}  // namespace codec
