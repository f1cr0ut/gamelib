#include<cstring>
#include<ft2build.h>
#include FT_FREETYPE_H
#include"../asset/asset.h"
#include"font.h"

using namespace core;

struct font::Impl {
	FT_Library library { nullptr };
	FT_Face face { nullptr };
	FT_GlyphSlot slot { nullptr };
};

font::font() {
	auto p = std::unique_ptr<Impl>(new Impl);
	std::swap(p, impl);
}

font::~font() {
	close();
}

bool font::load(const char * file, u32 dpi, u32 point) {
	if( ! file) {
		return false;
	}
	auto error = FT_Init_FreeType(&impl->library);
	if(error) {
		return false;
	}
	error = FT_New_Face(impl->library, file, 0, &impl->face);
	if(error) {
		return false;
	}
	error = FT_Set_Char_Size(impl->face, point * 64, 0, dpi, 0);
	if(error) {
		return false;
	}
	impl->slot = impl->face->glyph;
	return true;
}

void font::build(const u32 str, raw_image & res, i32 & baseline) {
	FT_Load_Char(impl->face, str, FT_LOAD_RENDER);
	res.w = impl->slot->bitmap.width;
	res.h = impl->slot->bitmap.rows;
	res.mode = raw_image::grayscale;
	res.data.resize(res.w * res.h);
	for (size_t i = 0, num = res.data.size(); i < num; ++i) {
		res.data[i] = impl->slot->bitmap.buffer[i];
	}
	baseline = impl->slot->bitmap_top;

}

void font::close() {
	if(impl->face) {
		FT_Done_Face(impl->face);
		impl->face = nullptr;
	}
	if(impl->library) {
		FT_Done_FreeType(impl->library);
		impl->library = nullptr;
	}
}
