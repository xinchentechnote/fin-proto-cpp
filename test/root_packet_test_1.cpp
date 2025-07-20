// Copyright 2025 xinchentechnote
#include <gtest/gtest.h>

#include "include/root_packet.hpp"

TEST(BasicPacketTest, ToStringOutput) {
  BasicPacket packet;
  packet.fieldI8 = -1;
  packet.fieldI16 = -12345;
  packet.fieldI32 = -12345678;
  packet.fieldI64 = -1234567890123LL;
  packet.fieldChar = "hello";
  packet.fieldU8 = 255;
  packet.fieldU16 = 65535;
  packet.fieldU32 = 4294967295U;
  packet.fieldU64 = 18446744073709551615ULL;
  packet.fieldF32 = 3.14f;
  packet.fieldF64 = 2.718281828459;

  packet.fieldI8List = {1, 2, 3};
  packet.fieldI16List = {100, 200};
  packet.fieldI32List = {300000};
  packet.fieldI64List = {4000000000LL};
  packet.fieldCharList = {"a", "b", "c"};
  packet.fieldU8List = {10, 20};
  packet.fieldU16List = {60000};
  packet.fieldU32List = {123456};
  packet.fieldU64List = {999999999};
  packet.fieldF32List = {1.1f, 2.2f};
  packet.fieldF64List = {3.3, 4.4};

  std::string str = packet.toString();
  EXPECT_NE(str.find("fieldI8: -1"), std::string::npos);
  EXPECT_NE(str.find("fieldChar: hello"), std::string::npos);
  EXPECT_NE(str.find("fieldU64: 18446744073709551615"), std::string::npos);
  EXPECT_NE(str.find("fieldF32: 3.14"), std::string::npos);
  EXPECT_NE(str.find("fieldF64: 2.71828"), std::string::npos);
  EXPECT_NE(str.find("fieldI8List: [1, 2, 3]"), std::string::npos);
  EXPECT_NE(str.find("fieldCharList: [\"a\", \"b\", \"c\"]"), std::string::npos);
}
