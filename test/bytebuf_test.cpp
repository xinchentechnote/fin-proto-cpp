// Copyright 2025 xinchentechnote
#include "include/bytebuf.hpp"

#include <gtest/gtest.h>

TEST(ByteBufTest, BasicWriteReadU8) {
  ByteBuf buf;
  buf.write_u8(123);
  EXPECT_EQ(buf.read_u8(), 123);
}

TEST(ByteBufTest, BasicWriteReadU16) {
  ByteBuf buf;
  buf.write_u16(0xABCD);
  EXPECT_EQ(buf.read_u16(), 0xABCD);
}

TEST(ByteBufTest, BasicWriteReadU32) {
  ByteBuf buf;
  buf.write_u32(0xDEADBEEF);
  EXPECT_EQ(buf.read_u32(), 0xDEADBEEF);
}

TEST(ByteBufTest, BasicWriteReadU64) {
  ByteBuf buf;
  buf.write_u64(0x1122334455667788);
  EXPECT_EQ(buf.read_u64(), 0x1122334455667788);
}

TEST(ByteBufTest, BasicWriteReadI8) {
  ByteBuf buf;
  buf.write_i8(123);
  EXPECT_EQ(buf.read_i8(), 123);
}

TEST(ByteBufTest, BasicWriteReadI16) {
  ByteBuf buf;
  buf.write_i16(0xABC);
  EXPECT_EQ(buf.read_i16(), 0xABC);
}

TEST(ByteBufTest, BasicWriteReadI32) {
  ByteBuf buf;
  buf.write_i32(0xDEADBEEF);
  EXPECT_EQ(buf.read_i32(), 0xDEADBEEF);
}

TEST(ByteBufTest, BasicWriteReadI64) {
  ByteBuf buf;
  buf.write_i64(0x1122334455667788);
  EXPECT_EQ(buf.read_i64(), 0x1122334455667788);
}

TEST(ByteBufTest, BasicWriteReadF32) {
  ByteBuf buf;
  buf.write_f32(0.5);
  EXPECT_EQ(buf.read_f32(), 0.5);
}

TEST(ByteBufTest, BasicWriteReadF64) {
  ByteBuf buf;
  buf.write_f64(0.8);
  EXPECT_EQ(buf.read_f64(), 0.8);
}

TEST(ByteBufTest, MultipleWritesAndReads) {
  ByteBuf buf;
  buf.write_u8(1);
  buf.write_u16(2);
  buf.write_u32(3);
  buf.write_u64(4);

  EXPECT_EQ(buf.read_u8(), 1);
  EXPECT_EQ(buf.read_u16(), 2);
  EXPECT_EQ(buf.read_u32(), 3);
  EXPECT_EQ(buf.read_u64(), 4);
}

TEST(ByteBufTest, OutOfBoundsReadThrows) {
  ByteBuf buf;
  buf.write_u8(5);
  buf.read_u8();
  EXPECT_THROW(buf.read_u8(), std::out_of_range);
}

TEST(ByteBufTest, ReadableWritableBytes) {
  ByteBuf buf;
  EXPECT_EQ(buf.readable_bytes(), 0);
  buf.write_u32(0x01020304);
  EXPECT_EQ(buf.readable_bytes(), 4);
  buf.read_u16();
  EXPECT_EQ(buf.readable_bytes(), 2);
}
