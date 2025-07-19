// Copyright 2025 xinchentechnote
#pragma once

#include <sstream>
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
// put/get dynamic string
// ----------------------------
template <typename T>
void put_string_le(ByteBuf& buf, const std::string& s) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  buf.write_le<T>(static_cast<T>(s.size()));
  buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
}

template <typename T>
std::string get_string_le(ByteBuf& buf) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  T length = buf.read_le<T>();
  std::vector<uint8_t> bytes(length);
  buf.read_bytes(bytes.data(), length);
  return std::string(reinterpret_cast<char*>(bytes.data()), length);
}

template <typename T>
void put_string(ByteBuf& buf, const std::string& s) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  buf.write<T>(static_cast<T>(s.size()));
  buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
}

template <typename T>
std::string get_string(ByteBuf& buf) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  T length = buf.read<T>();
  std::vector<uint8_t> bytes(length);
  buf.read_bytes(bytes.data(), length);
  return std::string(reinterpret_cast<char*>(bytes.data()), length);
}

// ----------------------------
// put/get Fixed-length string
// ----------------------------
inline void put_fixed_string(ByteBuf& buf, const std::string& s, size_t fixedLen) {
  if (s.size() >= fixedLen) {
    buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), fixedLen);
  } else {
    buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
    std::vector<uint8_t> padding(fixedLen - s.size(), 0);
    buf.write_bytes(padding.data(), padding.size());
  }
}

inline std::string get_fixed_string(ByteBuf& buf, size_t fixedLen) {
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
void put_string_list_le(ByteBuf& buf, const std::vector<std::string>& list) {
  static_assert(std::is_unsigned<T>::value && std::is_unsigned<K>::value,
                "T and K must be unsigned");
  buf.write_le<T>(static_cast<T>(list.size()));
  for (const auto& s : list) {
    buf.write_le<K>(static_cast<K>(s.size()));
    buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
  }
}

template <typename T, typename K>
std::vector<std::string> get_string_list_le(ByteBuf& buf) {
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

template <typename T, typename K>
void put_string_list(ByteBuf& buf, const std::vector<std::string>& list) {
  static_assert(std::is_unsigned<T>::value && std::is_unsigned<K>::value,
                "T and K must be unsigned");
  buf.write<T>(static_cast<T>(list.size()));
  for (const auto& s : list) {
    buf.write<K>(static_cast<K>(s.size()));
    buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
  }
}

template <typename T, typename K>
std::vector<std::string> get_string_list(ByteBuf& buf) {
  static_assert(std::is_unsigned<T>::value && std::is_unsigned<K>::value,
                "T and K must be unsigned");
  T count = buf.read<T>();
  std::vector<std::string> result;
  result.reserve(count);

  for (size_t i = 0; i < count; ++i) {
    K len = buf.read<K>();
    std::vector<uint8_t> bytes(len);
    buf.read_bytes(bytes.data(), len);
    result.emplace_back(reinterpret_cast<char*>(bytes.data()), len);
  }
  return result;
}

// ----------------------------
// fixed String List
// ----------------------------
template <typename T>
void put_fixed_string_list_le(ByteBuf& buf, const std::vector<std::string>& list, size_t fixedLen) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  buf.write_le<T>(static_cast<T>(list.size()));
  for (const auto& s : list) {
    put_fixed_string(buf, s, fixedLen);
  }
}

template <typename T>
std::vector<std::string> get_fixed_string_list_le(ByteBuf& buf, size_t fixedLen) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  T count = buf.read_le<T>();
  std::vector<std::string> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.push_back(get_fixed_string(buf, fixedLen));
  }
  return result;
}

template <typename T>
void put_fixed_string_list(ByteBuf& buf, const std::vector<std::string>& list, size_t fixedLen) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  buf.write<T>(static_cast<T>(list.size()));
  for (const auto& s : list) {
    put_fixed_string(buf, s, fixedLen);
  }
}

template <typename T>
std::vector<std::string> get_fixed_string_list(ByteBuf& buf, size_t fixedLen) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  T count = buf.read<T>();
  std::vector<std::string> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.push_back(get_fixed_string(buf, fixedLen));
  }
  return result;
}

// ----------------------------
// basic type List
// ----------------------------
template <typename T, typename K>
void put_basic_type_le(ByteBuf& buf, const std::vector<K>& list) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  buf.write_le<T>(static_cast<T>(list.size()));
  for (const auto& v : list) {
    buf.write_le<K>(static_cast<K>(v));
  }
}

template <typename T, typename K>
std::vector<K> get_basic_type_le(ByteBuf& buf) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  T count = buf.read_le<T>();
  std::vector<K> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.push_back(buf.read_le<K>());
  }
  return result;
}

template <typename T, typename K>
void put_basic_type(ByteBuf& buf, const std::vector<K>& list) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  buf.write<T>(static_cast<T>(list.size()));
  for (const auto& v : list) {
    buf.write<K>(static_cast<K>(v));
  }
}

template <typename T, typename K>
std::vector<K> get_basic_type(ByteBuf& buf) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  T count = buf.read<T>();
  std::vector<K> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.push_back(buf.read<K>());
  }
  return result;
}

template <typename T>
std::string join_vector(const std::vector<T>& vec, const std::string& sep = ", ") {
  std::ostringstream vss;
  vss << "[";
  for (size_t i = 0; i < vec.size(); ++i) {
    if (i > 0) vss << sep;
    vss << vec[i];
  }
  vss << "]";
  return vss.str();
}

// ----------------------------
// object List
// ----------------------------
template <typename T, typename K>
void put_object_List_le(ByteBuf& buf, const std::vector<K>& list) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  buf.write_le<T>(static_cast<T>(list.size()));
  for (const auto& obj : list) {
    obj.encode(buf);
  }
}

template <typename T, typename K>
std::vector<K> get_object_List_le(ByteBuf& buf) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  T count = buf.read_le<T>();
  std::vector<K> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    K obj;
    obj.decode(buf);
    result.push_back(obj);
  }
  return result;
}

}  // namespace codec
