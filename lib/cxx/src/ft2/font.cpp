#include <sway/gapi.hpp>
#include <sway/ui/ft2/font.hpp>

#include <algorithm>  // std::max

namespace sway::ui {

Font::Font(std::shared_ptr<Face> face, math::size2i_t atlasSize, math::size2i_t atlasMarginSize)
    : face_(face)
    , atlasSize_(atlasSize)
    , atlasMarginSize_(atlasMarginSize) {}

void Font::setHeight(u32_t height) {
  if (!face_) {
    return;
  }

  const auto faceData = face_->data();
  if (!faceData) {
    return;
  }

  FT_Size_RequestRec req;
  req.type = FT_SIZE_REQUEST_TYPE_REAL_DIM;
  req.width = 0;
  req.height = (FT_Long)std::round(height << 6);
  req.horiResolution = 0;
  req.vertResolution = 0;
  FT_Request_Size(faceData, &req);

  const auto faceSize = faceData->size;
  if (!faceSize) {
    return;
  }

  const auto &metrics = faceSize->metrics;
  info_.height = height;
  info_.descender = metrics.descender >> 6;
  info_.ascender = metrics.ascender >> 6;
  info_.lineSpacing = metrics.height >> 6;
  info_.lineGap = (metrics.height - metrics.ascender + metrics.descender) >> 6;
  info_.maxAdvanceWidth = metrics.max_advance >> 6;
}

void Font::create(lpcstr_t charcodes, [[maybe_unused]] bool hinted, [[maybe_unused]] bool antialiased) {
  if (!face_) {
    return;
  }

  const auto faceData = face_->data();
  std::fprintf(stderr, "[Font::create] ENTER, faceData=%p\n", (void *)faceData);
  std::fflush(stderr);
  if (!faceData) {
    return;
  }

  const auto charcodeLen = strlen(charcodes);
  std::fprintf(stderr, "[Font::create] charcodeLen=%zu\n", charcodeLen);
  std::fflush(stderr);

  for (auto i = 0; i < charcodeLen; i++) {
    std::fprintf(stderr, "[Font::create] i=%d, char='%c' (0x%02x)\n", i, charcodes[i], (unsigned char)charcodes[i]);
    std::fflush(stderr);

    if (hasCharInfo(charcodes[i])) {
      continue;
    }

    FontGlyphId glyphId(faceData, charcodes[i]);
    glyphs_.push_back(glyphId);

    auto slot = FontGlyph::load(faceData, glyphId);
    if (!slot.has_value()) {
      continue;
    }

    auto info = getCharMetrics(slot.value());

    FT_Glyph glyph;
    FT_Get_Glyph(slot.value(), &glyph);
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);

    FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
    FT_Bitmap *bitmap = &(bitmapGlyph)->bitmap;

    info.bitmapGlyphOffset = math::vec2i_t(bitmapGlyph->left, bitmapGlyph->top);
    cache_[glyphId.code] = info;

    maxSize_ = this->computeMaxSize_(bitmap, maxSize_);

    FT_Done_Glyph(glyph);  // FT_Get_Glyph создаёт новый объект, который нужно освобождать.

    std::fprintf(stderr, "[Font::create] maxSize_={%d,%d}\n", maxSize_.getW(), maxSize_.getH());
    std::fflush(stderr);
  }

  std::fprintf(stderr, "[Font::create] EXIT\n");
  std::fflush(stderr);
}

auto Font::computeMaxSize_(FT_Bitmap *bitmap, math::size2i_t size) -> math::size2i_t {
  // const auto w = bitmap->width;
  // const auto h = bitmap->rows;
  // const auto pw = math::powerOf2(w);
  // const auto ph = math::powerOf2(h);

  // std::fprintf(stderr, "[computeMaxSize_] bitmap={%u,%u}, powerOf2={%d,%d}, size={%d,%d}\n", w, h, pw, ph,
  // size.getW(),
  //     size.getH());
  // std::fflush(stderr);

  // return {std::max<i32_t>(size.getW(), pw), std::max<i32_t>(size.getH(), ph)};

  constexpr i32_t MAX_ATLAS_SIZE = 2048;

  const auto pw = bitmap->width > 0 ? math::powerOf2(bitmap->width) : 1;
  const auto ph = bitmap->rows > 0 ? math::powerOf2(bitmap->rows) : 1;

  return {std::min(std::max<i32_t>(size.getW(), pw), MAX_ATLAS_SIZE),
      std::min(std::max<i32_t>(size.getH(), ph), MAX_ATLAS_SIZE)};

  // // clang-format off
  // return {
  //   std::max<i32_t>(size.getW(), math::powerOf2(bitmap->width)),
  //   std::max<i32_t>(size.getH(), math::powerOf2(bitmap->rows))
  // };  // clang-format on
}

void Font::drawBitmap(FT_Bitmap *bitmap, Greymap &data) {
  for (auto y = 0; y < maxSize_.getH(); y++) {
    for (auto x = 0; x < maxSize_.getW(); x++) {
      if (x < 0 || y < 0 || x >= bitmap->width || y >= bitmap->rows) {
        data.at(x, y, 1) = 0;
      } else {
        data.at(x, y, 1) = bitmap->buffer[bitmap->pitch * y + x];
      }
    }
  }
}

auto Font::getBitmapData(FontGlyphId sym) -> BitmapInfo {
  auto slot = FontGlyph::load(face_->data(), sym);
  if (!slot.has_value()) {
    return BitmapInfo(maxSize_);
  }

  FT_Glyph glyph;
  FT_Get_Glyph(slot.value(), &glyph);

  FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
  FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
  FT_Bitmap *bitmap = &(bitmapGlyph)->bitmap;

  BitmapInfo bi(maxSize_);
  bi.bitmapSize = math::size2i_t(bitmap->width, bitmap->rows);
  drawBitmap(bitmap, bi.data);

  FT_Done_Glyph(glyph);  // FT_Get_Glyph создаёт новый объект, который нужно освобождать.

  return bi;
}

auto Font::getCharInfo(u32_t code) const -> std::optional<CharInfo> {
  const auto &iter = cache_.find(code);
  if (iter != cache_.end()) {
    return std::make_optional<CharInfo>((*iter).second);
  }

  return std::nullopt;
}

[[nodiscard]]
auto Font::hasCharInfo(u32_t code) const -> bool {
  return cache_.find(code) != cache_.end();
}

auto Font::getCharMetrics(FT_GlyphSlot slot) -> CharInfo {
  const auto &metrics = slot->metrics;

  CharInfo info;
  info.size = math::size2i_t(metrics.width, metrics.height) / 64;
  info.advance = metrics.horiAdvance >> 6;

  return info;
}

}  // namespace sway::ui
