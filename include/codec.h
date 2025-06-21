// codec.h
#pragma once

#include <cstring>
#include <folly/io/Cursor.h>
#include <folly/io/IOBuf.h>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace codec {

// ----------------------------
// BinaryCodec Interface
// ----------------------------
struct BinaryCodec {
  virtual void encode(folly::io::RWPrivateCursor &cursor) const = 0;
  virtual void decode(folly::io::Cursor &cursor) = 0;
  virtual ~BinaryCodec() = default;
};

// ----------------------------
// Helpers for writing strings
// ----------------------------
template <typename T>
void putStringLE(folly::io::RWPrivateCursor &cursor, const std::string &s) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  cursor.writeLE<T>(static_cast<T>(s.size()));
  cursor.push(reinterpret_cast<const uint8_t *>(s.data()), s.size());
}

template <typename T> std::string getStringLE(folly::io::Cursor &cursor) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  T length = cursor.readLE<T>();
  std::string result(reinterpret_cast<const char *>(cursor.data()), length);
  cursor.skip(length);
  return result;
}

inline void putFixedString(folly::io::RWPrivateCursor &cursor,
                           const std::string &s, size_t fixedLen) {
  if (s.size() >= fixedLen) {
    cursor.push(reinterpret_cast<const uint8_t *>(s.data()), fixedLen);
  } else {
    cursor.push(reinterpret_cast<const uint8_t *>(s.data()), s.size());
    std::vector<uint8_t> padding(fixedLen - s.size(), 0);
    cursor.push(padding.data(), padding.size());
  }
}

inline std::string getFixedString(folly::io::Cursor &cursor, size_t fixedLen) {
  std::string result(reinterpret_cast<const char *>(cursor.data()), fixedLen);
  cursor.skip(fixedLen);
  result.erase(result.find_last_not_of('\0') + 1);
  return result;
}

// ----------------------------
// List of Strings
// ----------------------------
template <typename T, typename K>
void putStringListLE(folly::io::RWPrivateCursor &cursor,
                     const std::vector<std::string> &values) {
  static_assert(std::is_unsigned<T>::value && std::is_unsigned<K>::value,
                "T and K must be unsigned");
  cursor.writeLE<T>(static_cast<T>(values.size()));
  for (const auto &s : values) {
    cursor.writeLE<K>(static_cast<K>(s.size()));
    cursor.push(reinterpret_cast<const uint8_t *>(s.data()), s.size());
  }
}

template <typename T, typename K>
std::vector<std::string> getStringListLE(folly::io::Cursor &cursor) {
  static_assert(std::is_unsigned<T>::value && std::is_unsigned<K>::value,
                "T and K must be unsigned");
  T count = cursor.readLE<T>();
  std::vector<std::string> result;
  result.reserve(count);

  for (size_t i = 0; i < count; ++i) {
    K len = cursor.readLE<K>();
    std::string s(reinterpret_cast<const char *>(cursor.data()), len);
    cursor.skip(len);
    result.push_back(std::move(s));
  }
  return result;
}

// ----------------------------
// BasicType Read
// ----------------------------
template <typename T> T getBasicType(folly::io::Cursor &cursor) {
  static_assert(std::is_arithmetic<T>::value, "T must be a basic type");
  return cursor.readLE<T>();
}

} // namespace codec
