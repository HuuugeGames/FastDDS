#include <cstdint>
#include <fstream>
#include <memory>
#include <stdio.h>

#include "contrib/Bitmap.hpp"
#include "contrib/BitmapDownsampled.hpp"

#include "contrib/cmdparser.hpp"
#include "contrib/DXTCompressorDLL.h"
#include "contrib/DdsHeader.hpp"


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "contrib/stb_image.h"
#include "contrib/stb_image_resize.h"

#undef max
#undef min

enum class Format {
    DXT1,
    DXT5
};

void FillHeader( DdsHeader& header, unsigned int w, unsigned int h, unsigned int mip, Format f )
{
    memset( &header, sizeof( DdsHeader ), 0 );
    strncpy( reinterpret_cast<char*>(&header.Magic), "DDS ", 4 );
    header.Size = 124;
    header.Flags = DDS_HEADER_FLAGS_TEXTURE;
    if ( mip > 0 )
        header.Flags |= DDSD_MIPMAPCOUNT;

    header.Width = w;
    header.Height = h;
    header.PitchOrLinearSize = header.Width; //TODO
    header.Depth = 0;
    header.MipMapCount = mip > 0 ? mip : 1;

    header.PixelFormat.Size = 32;
    header.PixelFormat.Flags = DDPF_RGB | DDPF_FOURCC;
    header.PixelFormat.RBitMask = 0x00ff0000;
    header.PixelFormat.GBitMask = 0x0000ff00;
    header.PixelFormat.BBitMask = 0x000000ff;
    header.PixelFormat.AlphaBitMask = 0xff000000;

    if ( f == Format::DXT1 )
    {
        strncpy( header.PixelFormat.FourCC, "DXT1", 4 );
        header.PixelFormat.RGBBitCount = 4;
    }
    else if ( f == Format::DXT5 )
    {
        strncpy( header.PixelFormat.FourCC, "DXT5", 4 );
        header.PixelFormat.Flags |= DDPF_ALPHAPIXELS;
        header.PixelFormat.RGBBitCount = 8;
    }

    header.Caps.Caps1 = 0x1000;
    if ( mip > 0 )
        header.Caps.Caps1 |= 0x400000;
    header.Caps.Caps2 = 0;
}

bool ConvertPNG( const std::string& image_path, const std::string& out_path, bool generate_mips = false, Format format = Format::DXT5 )
{
    auto image = std::make_shared<Bitmap>( image_path.c_str(), std::numeric_limits<uint>::max() );

    int width = image->Size().x;
    int height = image->Size().y;
    int c = 0;
    int desired_channels = 4;

    if ( !image || image->Size().x == 0)
    {
        std::cout << "No image data\n";
        return false;
    }

    std::ofstream outFile;
    outFile.open( out_path, std::ios::trunc | std::ios::binary  );
    if ( !outFile )
    {
        std::cout << "Can't open file\n";
        return false;
    }



	int targetDataSize = width*height;
    if ( format == Format::DXT1 )
        targetDataSize /= 2;

    auto buffer = std::make_unique<uint8_t[]>(targetDataSize);

    if ( reinterpret_cast<std::intptr_t>(buffer.get()) % 16 != 0 )
    {
        std::cout << "Error, misaligned pointer\n";
        return false;
    }
        

    if ( format == Format::DXT1 )
        DXTC::CompressImageDXT1SSE2( (const BYTE*)image->Data(), buffer.get(), width, height);
    else if ( format == Format::DXT5 )
        DXTC::CompressImageDXT5SSE2( (const BYTE*)image->Data(), buffer.get(), width, height);

    if ( !generate_mips )
    {
        DdsHeader header;
        FillHeader( header, width, height, 0, format );
        outFile.write(reinterpret_cast<char*>(&header), sizeof(DdsHeader));
        outFile.write(reinterpret_cast<char*>(buffer.get()), targetDataSize);
    }
    else
    {
        int mips = 1;
        int twidth = width;
        while ( (twidth >>= 1) > 8 )
            mips++;

        DdsHeader header;
        FillHeader( header, width, height, mips, format );
        outFile.write(reinterpret_cast<char*>(&header), sizeof(DdsHeader));
        outFile.write(reinterpret_cast<char*>(buffer.get()), targetDataSize);

        //auto resizedImage = std::make_unique<stbi_uc[]>(width*height*desired_channels);

        int mwidth = width;
        int mheight = height;
        for ( int i = 0; i < mips - 1; i++ )
        {
            targetDataSize /= 4;
            mwidth /= 2;
            mheight /= 2;

            auto downSampled = std::make_shared<BitmapDownsampled>( image, std::numeric_limits<uint>::max() );
            image = downSampled;
            //stbir_resize_uint8( image.get(), width, height, 0, resizedImage.get(), mwidth, mheight, 0, desired_channels );

            if ( format == Format::DXT1 )
                DXTC::CompressImageDXT1SSE2( (const BYTE*)downSampled->Data(), buffer.get(), mwidth, mheight);
            else if ( format == Format::DXT5 )
                DXTC::CompressImageDXT5SSE2( (const BYTE*)downSampled->Data(), buffer.get(), mwidth, mheight);
            outFile.write(reinterpret_cast<char*>(buffer.get()), targetDataSize);
        }
    }

    return true;
}

void configure_parser(cli::Parser& parser) 
{
    parser.set_required<std::string>("i", "input", "Input file (png, jpg)");
    parser.set_required<std::string>("o", "output", "Output file (dds)");
    parser.set_required<std::string>("f", "format", "Format - DXT1 or DXT5");
}

int main(int argc, char** argv )
{
    cli::Parser parser(argc, argv);
    configure_parser(parser);
    parser.run_and_exit_if_error();

    auto f = Format::DXT5;
    bool mips = true;

    auto format = parser.get<std::string>("f");
    if (format == "" || format == "DXT5" )
        f = Format::DXT5;
    else if (format == "DXT1" )
        f = Format::DXT5;
    else
    {
        std::cout << "Unrecognized format " << format << "\n";
        return -1;
    }

    if ( !ConvertPNG( parser.get<std::string>("i"), parser.get<std::string>("o"), mips, f ) )
        return -1;

    return 0;
}
