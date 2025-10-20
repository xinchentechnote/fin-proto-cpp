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
  virtual ~BinaryCodec() = default;
  virtual void encode(ByteBuf& buf) const = 0;
  virtual void decode(ByteBuf& buf) = 0;
  virtual std::string toString() const = 0;
  // Add virtual equality comparison
  virtual bool equals(const BinaryCodec& other) const = 0;
  // Non-virtual helper for derived classes
  bool operator==(const BinaryCodec& other) const { return equals(other); }
  bool operator!=(const BinaryCodec& other) const { return !(*this == other); }
};

// ----------------------------
// put/get dynamic string
// ----------------------------
template <typename T>
void write_string_le(ByteBuf& buf, const std::string& s) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  buf.write_le<T>(static_cast<T>(s.size()));
  buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
}

template <typename T>
std::string read_string_le(ByteBuf& buf) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  T length = buf.read_le<T>();
  std::vector<uint8_t> bytes(length);
  buf.read_bytes(bytes.data(), length);
  return std::string(reinterpret_cast<char*>(bytes.data()), length);
}

template <typename T>
void write_string(ByteBuf& buf, const std::string& s) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  buf.write<T>(static_cast<T>(s.size()));
  buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
}

template <typename T>
std::string read_string(ByteBuf& buf) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned integral");
  T length = buf.read<T>();
  std::vector<uint8_t> bytes(length);
  buf.read_bytes(bytes.data(), length);
  return std::string(reinterpret_cast<char*>(bytes.data()), length);
}

// ----------------------------
// put/get Fixed-length string
// ----------------------------

inline void write_fixed_string(ByteBuf& buf, const std::string& s, size_t fixedLen, char padChar,
                             bool padLeft) {
  if (s.size() >= fixedLen) {
    buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), fixedLen);
  } else {
    std::vector<uint8_t> padCharBytes(fixedLen - s.size(), padChar);
    if (padLeft) {
      buf.write_bytes(padCharBytes.data(), padCharBytes.size());
    }
    buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
    if (!padLeft) {
      buf.write_bytes(padCharBytes.data(), padCharBytes.size());
    }
  }
}

inline void write_fixed_string(ByteBuf& buf, const std::string& s, size_t fixedLen) {
  write_fixed_string(buf, s, fixedLen, ' ', false);
}

inline std::string read_fixed_string(ByteBuf& buf, size_t fixedLen, char trimPadChar,
                                    bool padLeft) {
  std::vector<uint8_t> bytes(fixedLen);
  buf.read_bytes(bytes.data(), fixedLen);
  std::string s(reinterpret_cast<char*>(bytes.data()), fixedLen);

  if (padLeft) {
    size_t start = s.find_first_not_of(trimPadChar);
    if (start != std::string::npos)
      s.erase(0, start);
    else
      s.clear();
  } else {
    size_t end = s.find_last_not_of(trimPadChar);
    if (end != std::string::npos)
      s.erase(end + 1);
    else
      s.clear();
  }

  return s;
}

inline std::string read_fixed_string(ByteBuf& buf, size_t fixedLen) {
  return read_fixed_string(buf, fixedLen, ' ', false);
}

// ----------------------------
// String List
// ----------------------------
template <typename T, typename K>
void write_string_list_le(ByteBuf& buf, const std::vector<std::string>& list) {
  static_assert(std::is_unsigned<T>::value && std::is_unsigned<K>::value,
                "T and K must be unsigned");
  buf.write_le<T>(static_cast<T>(list.size()));
  for (const auto& s : list) {
    buf.write_le<K>(static_cast<K>(s.size()));
    buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
  }
}

template <typename T, typename K>
std::vector<std::string> read_string_list_le(ByteBuf& buf) {
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
void write_string_list(ByteBuf& buf, const std::vector<std::string>& list) {
  static_assert(std::is_unsigned<T>::value && std::is_unsigned<K>::value,
                "T and K must be unsigned");
  buf.write<T>(static_cast<T>(list.size()));
  for (const auto& s : list) {
    buf.write<K>(static_cast<K>(s.size()));
    buf.write_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
  }
}

template <typename T, typename K>
std::vector<std::string> read_string_list(ByteBuf& buf) {
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
void write_fixed_string_list_le(ByteBuf& buf, const std::vector<std::string>& list, size_t fixedLen,
                              char padChar, bool padLeft) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  buf.write_le<T>(static_cast<T>(list.size()));
  for (const auto& s : list) {
    write_fixed_string(buf, s, fixedLen, padChar, padLeft);
  }
}

template <typename T>
void write_fixed_string_list_le(ByteBuf& buf, const std::vector<std::string>& list, size_t fixedLen) {
  write_fixed_string_list_le<T>(buf, list, fixedLen, ' ', false);
}

template <typename T>
std::vector<std::string> read_fixed_string_list_le(ByteBuf& buf, size_t fixedLen, char trimPadChar,
                                                  bool padLeft) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  T count = buf.read_le<T>();
  std::vector<std::string> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.push_back(read_fixed_string(buf, fixedLen, trimPadChar, padLeft));
  }
  return result;
}

template <typename T>
std::vector<std::string> read_fixed_string_list_le(ByteBuf& buf, size_t fixedLen) {
  return read_fixed_string_list_le<T>(buf, fixedLen, ' ', false);
}

template <typename T>
void write_fixed_string_list(ByteBuf& buf, const std::vector<std::string>& list, size_t fixedLen,
                           char padChar, bool padLeft) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  buf.write<T>(static_cast<T>(list.size()));
  for (const auto& s : list) {
    write_fixed_string(buf, s, fixedLen, padChar, padLeft);
  }
}

template <typename T>
void write_fixed_string_list(ByteBuf& buf, const std::vector<std::string>& list, size_t fixedLen) {
  write_fixed_string_list<T>(buf, list, fixedLen, ' ', false);
}

template <typename T>
std::vector<std::string> read_fixed_string_list(ByteBuf& buf, size_t fixedLen, char trimPadChar,
                                               bool padLeft) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  T count = buf.read<T>();
  std::vector<std::string> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.push_back(read_fixed_string(buf, fixedLen, trimPadChar, padLeft));
  }
  return result;
}

template <typename T>
std::vector<std::string> read_fixed_string_list(ByteBuf& buf, size_t fixedLen) {
  return read_fixed_string_list<T>(buf, fixedLen, ' ', false);
}

// ----------------------------
// basic type List
// ----------------------------
template <typename T, typename K>
void write_basic_type_le(ByteBuf& buf, const std::vector<K>& list) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  buf.write_le<T>(static_cast<T>(list.size()));
  for (const auto& v : list) {
    buf.write_le<K>(static_cast<K>(v));
  }
}

template <typename T, typename K>
std::vector<K> read_basic_type_le(ByteBuf& buf) {
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
void write_basic_type(ByteBuf& buf, const std::vector<K>& list) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  buf.write<T>(static_cast<T>(list.size()));
  for (const auto& v : list) {
    buf.write<K>(static_cast<K>(v));
  }
}

template <typename T, typename K>
std::vector<K> read_basic_type(ByteBuf& buf) {
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
    if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>) {
      vss << static_cast<int>(vec[i]);
    } else if constexpr (std::is_same_v<T, std::string>) {
      // Handle strings with escaping
      vss << "\"" << vec[i] << "\"";
    } else if constexpr (std::is_same_v<T, char>) {
      // Handle single characters
      vss << "'" << vec[i] << "'";
    } else {
      vss << vec[i];
    }
  }
  vss << "]";
  return vss.str();
}

// ----------------------------
// object List
// ----------------------------
template <typename T, typename K>
void write_object_List(ByteBuf& buf, const std::vector<K>& list) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  buf.write<T>(static_cast<T>(list.size()));
  for (const auto& obj : list) {
    obj.encode(buf);
  }
}

template <typename T, typename K>
std::vector<K> read_object_List(ByteBuf& buf) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  T count = buf.read<T>();
  std::vector<K> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    K obj;
    obj.decode(buf);
    result.push_back(obj);
  }
  return result;
}

template <typename T, typename K>
void write_object_List_le(ByteBuf& buf, const std::vector<K>& list) {
  static_assert(std::is_unsigned<T>::value, "T must be unsigned");
  buf.write_le<T>(static_cast<T>(list.size()));
  for (const auto& obj : list) {
    obj.encode(buf);
  }
}

template <typename T, typename K>
std::vector<K> read_object_List_le(ByteBuf& buf) {
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
