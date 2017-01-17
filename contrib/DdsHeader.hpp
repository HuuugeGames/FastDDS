#ifndef __INCLUDED_DDSHEADER_HPP__
#define __INCLUDED_DDSHEADER_HPP__


struct DdsHeader
{
    unsigned int Magic;
    unsigned int Size;
    unsigned int Flags;
    unsigned int Height;
    unsigned int Width;
    unsigned int PitchOrLinearSize;
    unsigned int Depth;
    unsigned int MipMapCount;
    unsigned int Reserved1[ 11 ];

    //  DDPIXELFORMAT
    struct
    {
        unsigned int Size;
        unsigned int Flags;
        char FourCC[4];
        unsigned int RGBBitCount;
        unsigned int RBitMask;
        unsigned int GBitMask;
        unsigned int BBitMask;
        unsigned int AlphaBitMask;
    } PixelFormat;

    //  DDCAPS2
    struct
    {
        unsigned int Caps1;
        unsigned int Caps2;
        unsigned int DDSX;
        unsigned int Reserved;
    } Caps;
    unsigned int Reserved2;
};

struct DdsHeader10
{
    unsigned int Format;
    unsigned int Dimension;
    unsigned int MiscFlag;
    unsigned int ArraySize;
    unsigned int Reserved;
};

enum {
    DDSD_CAPS = 0x00000001l,
    DDSD_HEIGHT = 0x00000002l,
    DDSD_WIDTH = 0x00000004l,
    DDSD_PITCH = 0x00000008l,
    DDSD_ALPHABITDEPTH = 0x00000080l,
    DDSD_PIXELFORMAT = 0x00001000l,
    DDSD_MIPMAPCOUNT = 0x00020000l,
    DDSD_LINEARSIZE = 0x00080000l,
    DDSD_DEPTH = 0x00800000l,
    DDS_HEADER_FLAGS_TEXTURE = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
};

enum {
	DDPF_ALPHAPIXELS = 0x00000001l,	// surface has alpha channel
	DDPF_ALPHA		 = 0x00000002l,	// alpha only
	DDPF_FOURCC		 = 0x00000004l,	// FOURCC available
	DDPF_RGB		 = 0x00000040l	// RGB(A) bitmap
};

#endif
