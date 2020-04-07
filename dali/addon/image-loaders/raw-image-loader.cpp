#include <dali/integration-api/addon-manager.h>
#include <dali/devel-api/adaptor-framework/image-loading.h>
#include <dali/devel-api/adaptor-framework/image-loader-input.h>

/*
        auto getFormatMagicNumber = addOnManager->GetGlobalProc<uint16_t()>( handle, "GetFormatMagicNumber" );
        auto getFormatExtension = addOnManager->GetGlobalProc<const char*()>( handle, "GetFormatExtension" );
        auto loadBitmap = Dali::ImageLoader::LoadBitmapFunction(addOnManager->GetGlobalProc( handle, "LoadBitmap" ));
        auto loadHeader = Dali::ImageLoader::LoadBitmapHeaderFunction(addOnManager->GetGlobalProc( handle, "LoadHeader" ));
        auto getBitmapProfile = addOnManager->GetGlobalProc<Bitmap::Profile()>( handle, "GetBitmapProfile" );

 */

extern "C" void GetAddOnInfo( Dali::AddOnInfo& info )
{
  info.type = Dali::AddOnType::IMAGE_LOADER;
  info.name = "raw-image-loader";
  info.version = Dali::DALI_ADDON_VERSION( 1, 0, 0 );
  info.next = nullptr;
}


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

  fread( &header, 1, sizeof(Header), input.file );

  printf("RAW: width = %d\n", header.width);
  printf("RAW: height = %d\n", header.height);

  width = header.width;
  height = header.height;

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
  auto header = LoadBitmapHeaderInternal( input, w, h );
  pixelData = Dali::Devel::PixelBuffer::New( w, h, Dali::Pixel::Format::RGBA8888 );
  fread( pixelData.GetBuffer(), 1, w*h*4, input.file);
  return true;
}

Dali::Integration::Bitmap::Profile GetBitmapProfile()
{
  printf("RAW: GetBitmapProfile\n");
  return Dali::Integration::Bitmap::Profile::BITMAP_2D_PACKED_PIXELS;
}

// Dispatch table
extern "C" void* GetGlobalProc( const char* funcname )
{
  std::string str (funcname);
  if (str == "GetFormatMagicNumber")
  {
    return reinterpret_cast<void *>(GetFormatMagicNumber);
  }
  else if (str == "GetFormatExtension")
  {
    return reinterpret_cast<void *>(GetFormatExtension);
  }
  else if (str == "LoadBitmap")
  {
    return reinterpret_cast<void *>(LoadBitmap);
  }
  else if (str == "LoadBitmapHeader")
  {
    return reinterpret_cast<void *>(LoadBitmapHeader);
  }
  else if (str == "GetBitmapProfile")
  {
    return reinterpret_cast<void *>(GetBitmapProfile);
  }
  return nullptr;
}

extern "C" void* GetInstanceProc( const char* funcname )
{
  return nullptr;
}
