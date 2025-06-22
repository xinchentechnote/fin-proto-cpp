#include "codec.h"
#include <gtest/gtest.h>

using namespace codec;

TEST(CodecTest, PutAndGetStringLE) {
  ByteBuf buf;
  std::string input = "hello world";
  putStringLE<uint16_t>(buf, input);

  std::string output = getStringLE<uint16_t>(buf);

  EXPECT_EQ(output, input);
}

TEST(CodecTest, PutAndGetFixedString) {
  ByteBuf buf;
  std::string input = "hi";
  putFixedString(buf, input, 8);

  std::string output = getFixedString(buf, 8);

  EXPECT_EQ(output, input);
}

TEST(CodecTest, FixedStringTruncation) {
  ByteBuf buf;
  std::string input = "1234567890";
  putFixedString(buf, input, 5);

  std::string output = getFixedString(buf, 5);

  EXPECT_EQ(output, "12345");
}

TEST(CodecTest, PutAndGetStringListLE) {
  ByteBuf buf;
  std::vector<std::string> input = {"one", "two", "three"};
  putStringListLE<uint8_t, uint8_t>(buf, input);

  auto output = getStringListLE<uint8_t, uint8_t>(buf);

  EXPECT_EQ(output.size(), input.size());
  for (size_t i = 0; i < input.size(); ++i) {
    EXPECT_EQ(output[i], input[i]);
  }
}

TEST(CodecTest, GetBasicType_U32) {
  ByteBuf buf;
  uint32_t input = 0x12345678;
  buf.writeLE<uint32_t>(input);

  uint32_t output = getBasicType<uint32_t>(buf);
  EXPECT_EQ(output, input);
}

TEST(CodecTest, GetBasicType_Float64) {
  ByteBuf buf;
  double input = 3.14159265359;
  buf.writeLE<double>(input);

  double output = getBasicType<double>(buf);
  EXPECT_DOUBLE_EQ(output, input);
}
