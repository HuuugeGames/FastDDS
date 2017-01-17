/*
	This code is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This code is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.
*/

#pragma once

#include <Windows.h>

namespace DXTC
{
	// DXT compressor (scalar version).
	void CompressImageDXT1(const BYTE* inBuf, BYTE* outBuf, int width, int height);
	void CompressImageDXT5(const BYTE* inBuf, BYTE* outBuf, int width, int height, unsigned int rowPitch = 0);
	WORD ColorTo565(const BYTE* color);
	void EmitByte(BYTE*& dest, BYTE b);
	void EmitWord(BYTE*& dest, WORD s);
	void EmitDoubleWord(BYTE*& dest, DWORD i);
	void ExtractBlock(const BYTE* inPtr, int width, BYTE* colorBlock);
	void GetMinMaxColors(const BYTE* colorBlock, BYTE* minColor, BYTE* maxColor);
	void GetMinMaxColorsWithAlpha(const BYTE* colorBlock, BYTE* minColor, BYTE* maxColor);
	void EmitColorIndices(const BYTE* colorBlock, BYTE*& outBuf, const BYTE* minColor, const BYTE* maxColor);
	void EmitAlphaIndices(const BYTE* colorBlock,  BYTE*& outBuf, const BYTE minAlpha, const BYTE maxAlpha);

	// DXT compressor (SSE2 version).
	void CompressImageDXT1SSE2(const BYTE* inBuf, BYTE* outBuf, int width, int height);
	void CompressImageDXT5SSE2(const BYTE* inBuf, BYTE* outBuf, int width, int height, unsigned int rowPitch = 0);
	void ExtractBlock_SSE2(const BYTE* inPtr, int width, BYTE* colorBlock);
	void GetMinMaxColors_SSE2(const BYTE* colorBlock, BYTE* minColor, BYTE* maxColor);
	void EmitColorIndices_SSE2(const BYTE* colorBlock, BYTE*& outBuf, const BYTE* minColor, const BYTE* maxColor);
	void EmitAlphaIndices_SSE2(const BYTE* colorBlock, BYTE*& outBuf, const BYTE minAlpha, const BYTE maxAlpha);
}
