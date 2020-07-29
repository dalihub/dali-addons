/**
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
 * All rights reserved.
 *
 * This software is a confidential and proprietary information of Samsung
 * Electronics, Inc. ("Confidential Information").  You shall not disclose such
 * Confidential Information and use it in accordance with the terms of your
 * employment with Samsung.
 */
 
#include <dali/devel-api/addons/addon-base.h>

#include <dali/devel-api/adaptor-framework/image-loading.h>
#include <dali/devel-api/adaptor-framework/image-loader-input.h>
#include <dali/internal/common/owner-pointer.h>

// Loader API

uint16_t GetFormatMagicNumber()
{
  printf("RAW: GetMagicNumber\n");
  return 0xC0DE;
}

const char* GetFormatExtension()
{
  printf("RAW: GetFormatExtension\n");
  return "raw";
}

struct Header
{
  uint16_t magic;
  uint16_t width;
  uint16_t height;
};

Header LoadBitmapHeaderInternal( const Dali::ImageLoader::Input& input, unsigned int& width, unsigned int& height )
{
  printf("RAW: GetBitmapHeader\n");

  Header header{};
  header.magic = 0;

  if( fread( &header, 1, sizeof(Header), input.file ) )
  {
    printf("RAW: width = %d\n", header.width);
    printf("RAW: height = %d\n", header.height);

    width = header.width;
    height = header.height;
  }
  return header;
}

bool LoadBitmapHeader( const Dali::ImageLoader::Input& input, unsigned int& width, unsigned int& height )
{
  return LoadBitmapHeaderInternal( input, width, height).magic != 0;
}

bool LoadBitmap( const Dali::ImageLoader::Input& input, Dali::Devel::PixelBuffer& pixelData )
{
  // rewind stream
  fseek( input.file, 0, SEEK_SET );
  unsigned int w, h;
  LoadBitmapHeaderInternal( input, w, h );
  pixelData = Dali::Devel::PixelBuffer::New( w, h, Dali::Pixel::Format::RGBA8888 );
  if( fread( pixelData.GetBuffer(), 1, w*h*4, input.file) )
  {
    return true;
  }
  return false;
}

Dali::Integration::Bitmap::Profile GetBitmapProfile()
{
  printf("RAW: GetBitmapProfile\n");
  return Dali::Integration::Bitmap::Profile::BITMAP_2D_PACKED_PIXELS;
}

/**
 * RawImageLoader implementation
 */
using namespace Dali::AddOns;
class RawImageLoader : public Dali::AddOns::AddOnBase
{
public:

  void GetAddOnInfo( Dali::AddOnInfo& info ) override
  {
    info.type = Dali::AddOnType::IMAGE_LOADER;
    info.name = "raw-image-loader";
    info.version = Dali::DALI_ADDON_VERSION( 1, 0, 0 );
    info.next = nullptr;
  }

  DispatchTable* GetGlobalDispatchTable() override
  {
    static DispatchTable dispatchTable;
    if( dispatchTable.Empty() )
    {
      dispatchTable["GetFormatMagicNumber"] = GetFormatMagicNumber;
      dispatchTable["GetFormatExtension"] = GetFormatExtension;
      dispatchTable["LoadBitmap"] = LoadBitmap;
      dispatchTable["LoadBitmapHeader"] = LoadBitmapHeader;
      dispatchTable["GetBitmapProfile"] = GetBitmapProfile;
    }
    return &dispatchTable;
  }

  DispatchTable* GetInstanceDispatchTable() override
  {
    return nullptr;
  }
};

REGISTER_ADDON_CLASS( RawImageLoader );
