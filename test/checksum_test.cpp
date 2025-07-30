#include "include/checksum.hpp"

#include <gtest/gtest.h>

#include "include/bytebuf.hpp"

TEST(Crc16ChecksumServiceTest, HandlesKnownInput) {
  ByteBuf buf;
  buf.write_bytes(std::vector<uint8_t>{'1', '2', '3', '4', '5', '6', '7', '8', '9'}.data(), 9);
  Crc16ChecksumService svc;
  uint16_t result = svc.calc(buf);
  EXPECT_EQ(result, 0x4B37u);
}

TEST(Crc32ChecksumServiceTest, HandlesKnownInput) {
  ByteBuf buf;
  buf.write_bytes(std::vector<uint8_t>{'1', '2', '3', '4', '5', '6', '7', '8', '9'}.data(), 9);
  Crc32ChecksumService svc;
  uint32_t result = svc.calc(buf);
  EXPECT_EQ(result, 0xCBF43926u);
}

TEST(Crc32ChecksumServiceTest, HandlesEmptyBuffer) {
  ByteBuf buf;
  Crc32ChecksumService svc;
  uint32_t result = svc.calc(buf);
  EXPECT_EQ(result, 0u);
}

TEST(SseBinChecksumServiceTest, HandlesKnownInpur) {
  ByteBuf buf;
  buf.write_bytes(std::vector<uint8_t>{1, 2, 3}.data(), 3);
  SseBinChecksumService svc;
  uint32_t checksum = svc.calc(buf);
  EXPECT_EQ(checksum, 6);
}

TEST(SzseBinChecksumServiceTest, HandlesKnownInpur) {
  ByteBuf buf;
  buf.write_bytes(std::vector<uint8_t>{1, 2, 3}.data(), 3);
  SzseBinChecksumService svc;
  uint32_t checksum = svc.calc(buf);
  EXPECT_EQ(checksum, 6);
}

TEST(ChecksumServiceContextTest, HandlesKnownInpur) {
  ByteBuf buf;
  buf.write_bytes(std::vector<uint8_t>{1, 2, 3}.data(), 3);
  auto svc = ChecksumServiceContext::instance().get<ByteBuf, uint32_t>("SSE_BIN");
  uint32_t checksum = svc->calc(buf);
  EXPECT_EQ(checksum, 6);
}
