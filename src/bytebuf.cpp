#include "bytebuf.h"

ByteBuf::ByteBuf(size_t capacity)
    : buffer_(capacity), readerIndex_(0), writerIndex_(0) {}

void ByteBuf::ensureWritable(size_t len) {
  if (writerIndex_ + len > buffer_.size()) {
    size_t newSize = buffer_.size() * 2;
    if (newSize < writerIndex_ + len)
      newSize = writerIndex_ + len;
    buffer_.resize(newSize);
  }
}

void ByteBuf::writeBytes(const void *data, size_t len) {
  ensureWritable(len);
  std::memcpy(&buffer_[writerIndex_], data, len);
  writerIndex_ += len;
}

void ByteBuf::readBytes(void *dst, size_t len) {
  if (readerIndex_ + len > writerIndex_) {
    throw std::out_of_range("ByteBuf: not enough readable bytes");
  }
  std::memcpy(dst, &buffer_[readerIndex_], len);
  readerIndex_ += len;
}

void ByteBuf::writeU8(uint8_t val) { writeBytes(&val, sizeof(val)); }

uint8_t ByteBuf::readU8() {
  uint8_t val;
  readBytes(&val, sizeof(val));
  return val;
}

void ByteBuf::writeU16(uint16_t val) {
  val = htole16(val);
  writeBytes(&val, sizeof(val));
}

uint16_t ByteBuf::readU16() {
  uint16_t val;
  readBytes(&val, sizeof(val));
  return le16toh(val);
}

void ByteBuf::writeU32(uint32_t val) {
  val = htole32(val);
  writeBytes(&val, sizeof(val));
}

uint32_t ByteBuf::readU32() {
  uint32_t val;
  readBytes(&val, sizeof(val));
  return le32toh(val);
}

void ByteBuf::writeU64(uint64_t val) {
  val = htole64(val);
  writeBytes(&val, sizeof(val));
}

uint64_t ByteBuf::readU64() {
  uint64_t val;
  readBytes(&val, sizeof(val));
  return le64toh(val);
}

void ByteBuf::reset() {
  readerIndex_ = 0;
  writerIndex_ = 0;
  //   buffer_.clear();
}

size_t ByteBuf::readableBytes() const { return writerIndex_ - readerIndex_; }

size_t ByteBuf::writableBytes() const { return buffer_.size() - writerIndex_; }
