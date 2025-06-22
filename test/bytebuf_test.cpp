#include "bytebuf.h"
#include <gtest/gtest.h>

TEST(ByteBufTest, BasicWriteReadU8) {
  ByteBuf buf;
  buf.writeU8(123);
  EXPECT_EQ(buf.readU8(), 123);
}

TEST(ByteBufTest, BasicWriteReadU16) {
  ByteBuf buf;
  buf.writeU16(0xABCD);
  EXPECT_EQ(buf.readU16(), 0xABCD);
}

TEST(ByteBufTest, BasicWriteReadU32) {
  ByteBuf buf;
  buf.writeU32(0xDEADBEEF);
  EXPECT_EQ(buf.readU32(), 0xDEADBEEF);
}

TEST(ByteBufTest, BasicWriteReadU64) {
  ByteBuf buf;
  buf.writeU64(0x1122334455667788);
  EXPECT_EQ(buf.readU64(), 0x1122334455667788);
}

TEST(ByteBufTest, MultipleWritesAndReads) {
  ByteBuf buf;
  buf.writeU8(1);
  buf.writeU16(2);
  buf.writeU32(3);
  buf.writeU64(4);

  EXPECT_EQ(buf.readU8(), 1);
  EXPECT_EQ(buf.readU16(), 2);
  EXPECT_EQ(buf.readU32(), 3);
  EXPECT_EQ(buf.readU64(), 4);
}

TEST(ByteBufTest, OutOfBoundsReadThrows) {
  ByteBuf buf;
  buf.writeU8(5);
  buf.readU8();
  EXPECT_THROW(buf.readU8(), std::out_of_range);
}

TEST(ByteBufTest, ReadableWritableBytes) {
  ByteBuf buf;
  EXPECT_EQ(buf.readableBytes(), 0);
  buf.writeU32(0x01020304);
  EXPECT_EQ(buf.readableBytes(), 4);
  buf.readU16();
  EXPECT_EQ(buf.readableBytes(), 2);
}
