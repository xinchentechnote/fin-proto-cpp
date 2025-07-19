// Copyright 2025 xinchentechnote
#include "include/codec.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

TEST(CodecTest, PutAndGetStringLE) {
  ByteBuf buf;
  std::string input = "hello world";
  codec::put_string_le<uint16_t>(buf, input);

  std::string output = codec::get_string_le<uint16_t>(buf);

  EXPECT_EQ(output, input);
}

TEST(CodecTest, PutAndGetFixedString) {
  ByteBuf buf;
  std::string input = "hi";
  codec::put_fixed_string(buf, input, 8);

  std::string output = codec::get_fixed_string(buf, 8);

  EXPECT_EQ(output, input);
}

TEST(CodecTest, FixedStringTruncation) {
  ByteBuf buf;
  std::string input = "1234567890";
  codec::put_fixed_string(buf, input, 5);

  std::string output = codec::get_fixed_string(buf, 5);

  EXPECT_EQ(output, "12345");
}

TEST(CodecTest, PutAndGetStringListLE) {
  ByteBuf buf;
  std::vector<std::string> input = {"one", "two", "three"};
  codec::put_string_list_le<uint8_t, uint8_t>(buf, input);

  auto output = codec::get_string_list_le<uint8_t, uint8_t>(buf);

  EXPECT_EQ(output.size(), input.size());
  for (size_t i = 0; i < input.size(); ++i) {
    EXPECT_EQ(output[i], input[i]);
  }
}
