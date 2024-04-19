#include <sway/gapi.hpp>
#include <sway/ui/ft2/font.hpp>

#include <algorithm>  // std::max

NAMESPACE_BEGIN(sway)
NAMESPACE_BEGIN(ui)
NAMESPACE_BEGIN(ft2)

Font::Font(std::shared_ptr<Face> face, math::size2i_t atlasSize, math::size2i_t atlasMarginSize)
    : face_(face)
    , atlasSize_(atlasSize)
    , atlasMarginSize_(atlasMarginSize) {}

void Font::setHeight(u32_t height) {
  FT_Size_RequestRec req;
  req.type = FT_SIZE_REQUEST_TYPE_REAL_DIM;
  req.width = 0;
  req.height = (FT_Long)std::round(height << 6);
  req.horiResolution = 0;
  req.vertResolution = 0;
  FT_Request_Size(face_->data(), &req);

  const auto &metrics = face_->data()->size->metrics;
  info_.height = height;
  info_.descender = metrics.descender >> 6;
  info_.ascender = metrics.ascender >> 6;
  info_.lineSpacing = metrics.height >> 6;
  info_.lineGap = (metrics.height - metrics.ascender + metrics.descender) >> 6;
  info_.maxAdvanceWidth = metrics.max_advance >> 6;
}

void Font::create(lpcstr_t charcodes, bool hinted, bool antialiased) {
  for (auto i = 0; i < strlen(charcodes); i++) {
    if (hasCharInfo(charcodes[i])) {
      continue;
    }

    FontGlyphId glyphId(face_->data(), charcodes[i]);
    glyphs_.push_back(glyphId);

    auto slot = FontGlyph::load(face_->data(), glyphId);
    if (!slot.has_value()) {
      continue;
    }

    auto info = getCharMetrics(slot.value());
    // info.symidx = glyphId.idx;

    FT_Glyph glyph;
    FT_Get_Glyph(slot.value(), &glyph);
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);

    FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
    FT_Bitmap *bitmap = &(bitmapGlyph)->bitmap;

    info.tl = math::vec2i_t(bitmapGlyph->left, bitmapGlyph->top);

    cache_[glyphId.code] = info;

    maxSize_ = this->computeMaxSize_(bitmap, maxSize_);
  }
}

auto Font::computeMaxSize_(FT_Bitmap *bitmap, math::size2i_t size) -> math::size2i_t {
  // clang-format off
  return {
    std::max<s32_t>(size.getW(), math::util::powerOf2(bitmap->width)),
    std::max<s32_t>(size.getH(), math::util::powerOf2(bitmap->rows))
  };  // clang-format on
}

void Font::drawBitmap(FT_Bitmap *bitmap, u8_t *image, const math::rect4i_t &rect) {
  for (auto y = 0; y < rect.getH(); y++) {
    for (auto x = 0; x < rect.getW(); x++) {
      auto idx = 2 * (y * rect.getW() + x);

      image[idx] = 255;

      if (x < 0 || y < 0 || x >= bitmap->width || y >= bitmap->rows) {
        image[idx + 1] = 0;
      } else {
        image[idx + 1] = bitmap->buffer[bitmap->pitch * y + x];
      }
    }
  }
}

auto Font::getBitmapData(FontGlyphId sym) -> BitmapInfo {
  auto slot = FontGlyph::load(face_->data(), sym);
  if (!slot.has_value()) {
    // Empty
  }

  FT_Glyph glyph;
  FT_Get_Glyph(slot.value(), &glyph);

  FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
  FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
  FT_Bitmap *bitmap = &(bitmapGlyph)->bitmap;

  math::rect4i_t texRect;
  texRect.set(0, 0, maxSize_);

  auto *texData = (u8_t *)malloc(maxSize_.area() * sizeof(u8_t) * 2);

  drawBitmap(bitmap, texData, texRect);

  BitmapInfo bi;
  bi.pitch = bitmap->pitch;
  bi.data = texData;
  bi.size = math::size2i_t(bitmap->width, bitmap->rows);
  bi.tl = math::vec2i_t(bitmapGlyph->left, bitmapGlyph->top);

  free(texData);
  return bi;
}

auto Font::getCharInfo(s8_t code) const -> std::optional<CharInfo> {
  const auto &iter = cache_.find(code);
  if (iter != cache_.end()) {
    return std::make_optional<CharInfo>((*iter).second);
  }

  return std::nullopt;
}

[[nodiscard]]
auto Font::hasCharInfo(s8_t code) const -> bool {
  return cache_.find(code) != cache_.end();
}

auto Font::getCharMetrics(FT_GlyphSlot slot) -> CharInfo {
  const auto &metrics = slot->metrics;

  CharInfo info;
  info.size = math::size2i_t(metrics.width, metrics.height) / 64;
  info.horzBearing = math::vec2i_t(metrics.horiBearingX >> 6, metrics.horiBearingY >> 6);
  info.vertBearing = math::vec2i_t(metrics.vertBearingX >> 6, metrics.vertBearingY >> 6);
  info.advance = metrics.horiAdvance >> 6;
  info.offset = math::vec2i_t(info.vertBearing.getX(), -info.vertBearing.getY() + metrics.vertAdvance >> 6);

  return info;
}

NAMESPACE_END(ft2)
NAMESPACE_END(ui)
NAMESPACE_END(sway)
