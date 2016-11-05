#include<iostream>
#include <setjmp.h>
#include<turbojpeg.h>
#include<jpeglib.h>
#include<png.h>
#include"texture.h"

using namespace core;

namespace {
	struct my_error_mgr {
		struct jpeg_error_mgr pub;
		jmp_buf setjmp_buffer;
	};
	typedef struct my_error_mgr * my_error_ptr;
	METHODDEF(void) my_error_exit(j_common_ptr cinfo){
		my_error_ptr myerr = (my_error_ptr) cinfo->err;
		(*cinfo->err->output_message) (cinfo);
		longjmp(myerr->setjmp_buffer, 1);
	}
}

bool jpg::load(const char * file) {
	jpeg_decompress_struct cinfo;
	my_error_mgr jerr;
	FILE * infile;
	int row_stride;
	if((infile = fopen(file, "rb")) == NULL) {
		return false;
	}
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return false;
	}
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	row_stride = cinfo.output_width * cinfo.output_components;
	std::vector<JSAMPROW> sp;
	sp.resize(cinfo.output_height);
	raw_image::data.resize(row_stride * cinfo.output_height);
	u32 pos = 0;
	for (int i = 0, j = 0; i < sp.size(); ++i) {
		sp[i] = &raw_image::data[j];
		j += row_stride;
	}
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, &sp[pos], 1);
		pos += 1;
	}
	raw_image::w = cinfo.output_width;
	raw_image::h = cinfo.output_height;
	raw_image::mode = cinfo.output_components == 3 ? rgb : grayscale;
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	return true;
}

bool png::load(const char * file) {
	char header[8];
	FILE *fp = fopen(file, "rb");
	if( ! fp) {
		return false;
	}
	fread(header, 1, 8, fp);
	if(png_sig_cmp((png_const_bytep)header, 0, 8)) {
		fclose(fp);
		return false;
	}
	auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if( ! png_ptr) {
		fclose(fp);
		return false;
	}
	auto info_ptr = png_create_info_struct(png_ptr);
	if( ! info_ptr) {
	    png_destroy_read_struct(&png_ptr,NULL, NULL);
		fclose(fp);
		return false;
	}	
	if(setjmp(png_jmpbuf(png_ptr))) {
	    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return false;
	}	
	int bit_depth, color_type, interlace_type;
	u32 w = 0, h = 0;
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, &interlace_type, NULL, NULL);
	raw_image::w = w;
	raw_image::h = h;
	if(color_type == PNG_COLOR_TYPE_RGBA) {
		raw_image::mode = rgba;
	}
	if (color_type == PNG_COLOR_TYPE_RGB) {
		raw_image::mode = rgb;
	}
	if (color_type == PNG_COLOR_TYPE_GRAY) {
		raw_image::mode = grayscale;
	}
	if (color_type == PNG_COLOR_TYPE_GA) {
		raw_image::mode = alpha_grayscale;
	}	
	png_read_update_info(png_ptr, info_ptr);
	if (setjmp(png_jmpbuf(png_ptr))){
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return false;
	}	
	std::vector<png_bytep> pt;
	pt.resize(raw_image::h);
	raw_image::data.resize(png_get_rowbytes(png_ptr, info_ptr) * raw_image::h);
	u32 pos = 0;
	for(int i = 0; i < raw_image::h; ++i) {
		pt[i] = &raw_image::data[pos];
		pos += (u32)png_get_rowbytes(png_ptr, info_ptr);
	}
	png_read_image(png_ptr, (png_bytepp)&pt[0]);
	fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	return true;
}
