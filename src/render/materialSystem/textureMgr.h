// textureMgr.h
#ifndef __TEXTUREMGR__
#define __TEXTUREMGR__

#include "common/handle.h"
#include "libsn/hashMap.h"
class Texture;

class TextureMgr
{
protected:
  snSHashMap<Texture*> textures;
  snVector<char*> invalidationList;

  Texture* LoadTexture(const char* textureName);

public:
  TextureMgr();
  virtual ~TextureMgr();

  int Initialize();
  Texture* GetTexture(const char* textureName);
  void BindTexture(int stage, Texture* t);
  void ReloadTexture(int texId);
  void ReleaseTextures();
  void ListTextures();
  void InvalidateTexture(const char* name);
};

#endif
