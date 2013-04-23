// texture.h
#ifndef __TEXTURE__
#define __TEXTURE__

enum TextureType
{
  TEX_1D,
  TEX_2D,
  TEX_CUBEMAP
};

enum PixelFormat
{
  PF_NONE = 0,
  PF_A8,             // 8-bit  alpha
  PF_R5G6B5,         // 16-bit 565
  PF_B5G6R5,         // 16-bit 565 reverse
  PF_R4G4B4A4,       // 16-bit 4444
  PF_R5G5B5A1,       // 16-bit 5551
  PF_B5G5R5A1,       // 16-bit 5551 reverse
  PF_R8G8B8,         // 24-bit 888
  PF_B8G8R8,         // 24-bit 888  reverse
  PF_R8G8B8A8,       // 32-bit 8888
  PF_B8G8R8A8,       // 32-bit 8888 reverse
  PF_DXT1,           // compressed dxt1
  PF_DXT2,           // compressed dxt2
  PF_DXT3,           // compressed dxt3
  PF_DXT4,           // compressed dxt4
  PF_DXT5,           // compressed dxt5
  PF_FLOAT16,        // 16-bit float
  PF_FLOAT16_RGB,    // 16-bit float rgb
  PF_FLOAT16_RGBA,   // 16-bit float rgba
  PF_FLOAT32,        // 32-bit float
  PF_FLOAT32_RGB,    // 32-bit float rgb
  PF_FLOAT32_RGBA,   // 32-bit float rgba
  PF_DEPTH16,        // 16-bit depth
  PF_DEPTH24,        // 24-bit depth
  PF_DEPTH32,        // 32-bit depth

  PF_NUM
};



class Texture
{
protected:
  unsigned int type;
  unsigned int width;
  unsigned int height;
  unsigned int format;
  unsigned int nMips;

  virtual unsigned int _Load(unsigned char* data) = 0;

public:
  Texture() : width(0), height(0), type(0), format(0), nMips(0) { }
  virtual ~Texture() {}

  unsigned int GetType()   { return type; }
  unsigned int GetWidth()  { return width; }
  unsigned int GetHeight() { return height; }
  unsigned int GetFormat() { return format; }

  virtual void BeginRenderTarget() = 0;
  virtual void EndRenderTarget(int w, int h) = 0;
  virtual float* GetData() = 0;
  

  unsigned int Create(TextureType _type, PixelFormat _format,
                      unsigned int _width, unsigned int _height,
                      unsigned int _nMips, unsigned char* data)
  {
    type = _type;
    format = _format;
    width = _width;
    height = _height;
    nMips = _nMips;

    return _Load(data);
  }
};

#endif
