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

TEST(JoinVectorTest, IntVector) {
  std::vector<int> nums = {1, 2, 3};
  EXPECT_EQ("[1, 2, 3]", codec::join_vector(nums));
}

TEST(JoinVectorTest, EmptyVector) {
  std::vector<int> empty;
  EXPECT_EQ("[]", codec::join_vector(empty));
}

TEST(JoinVectorTest, SingleElement) {
  std::vector<std::string> single = {"hello"};
  EXPECT_EQ("[\"hello\"]", codec::join_vector(single));
}

TEST(JoinVectorTest, CustomSeparator) {
  std::vector<double> nums = {1.1, 2.2, 3.3};
  EXPECT_EQ("[1.1|2.2|3.3]", codec::join_vector(nums, "|"));
}

TEST(JoinVectorTest, StringVector) {
  std::vector<std::string> words = {"apple", "banana", "cherry"};
  EXPECT_EQ("[\"apple\", \"banana\", \"cherry\"]", codec::join_vector(words));
}

TEST(JoinVectorTest, CharVector) {
  std::vector<char> chars = {'a', 'b', 'c'};
  EXPECT_EQ("['a', 'b', 'c']", codec::join_vector(chars));
}

TEST(JoinVectorTest, BoolVector) {
  std::vector<bool> flags = {true, false, true};
  EXPECT_EQ("[1, 0, 1]", codec::join_vector(flags));
}

TEST(JoinVectorTest, LongVector) {
  std::vector<long> longs = {1000000L, 2000000L, 3000000L};
  EXPECT_EQ("[1000000, 2000000, 3000000]", codec::join_vector(longs));
}

TEST(JoinVectorTest, FloatVector) {
  std::vector<float> floats = {1.5f, 2.5f, 3.5f};
  EXPECT_EQ("[1.5, 2.5, 3.5]", codec::join_vector(floats));
}

TEST(JoinVectorTest, Int8Separator) {
  std::vector<int8_t> nums = {1, 2, 3};
  EXPECT_EQ("[1,2,3]", codec::join_vector(nums, ","));
}

TEST(JoinVectorTest, ComplexSeparator) {
  std::vector<int> nums = {1, 2, 3};
  EXPECT_EQ("[1 - 2 - 3]", codec::join_vector(nums, " - "));
}

TEST(JoinVectorTest, LargeVector) {
  std::vector<int> large(1000);
  for (int i = 0; i < 1000; i++) {
    large[i] = i;
  }
  std::string result = codec::join_vector(large);
  EXPECT_EQ('[', result.front());
  EXPECT_EQ(']', result.back());
  EXPECT_NE(result.find("500"), std::string::npos);
}

TEST(JoinVectorTest, SpecialChars) {
  std::vector<std::string> specials = {"a,b", "c,d", "e,f"};
  EXPECT_EQ("[\"a,b\", \"c,d\", \"e,f\"]", codec::join_vector(specials));
}

TEST(JoinVectorTest, NewlineSeparator) {
  std::vector<int> nums = {1, 2, 3};
  EXPECT_EQ("[1\n2\n3]", codec::join_vector(nums, "\n"));
}