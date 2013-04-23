/**
  @file renderer_defs.h
  @author Christopher Olsen
  @date Wed Sep 29 18:25:42 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __RENDERER_DEFS__
#define __RENDERER_DEFS__

#define R_ENABLE 0x01
#define R_DISABLE 0x02

#define R_COLOR_BUFFER   0x01
#define R_DEPTH_BUFFER   0x02
#define R_STENCIL_BUFFER 0x04

#define R_CCW 0x01
#define R_CW  0x02

#define R_CULL_NONE  0x00
#define R_CULL_FRONT 0x01
#define R_CULL_BACK  0x02

#define R_ZERO                0x00
#define R_ONE                 0x01
#define R_SRC_ALPHA           0x02
#define R_DST_ALPHA           0x03
#define R_ONE_MINUS_SRC_ALPHA 0x04
#define R_ONE_MINUS_DST_ALPHA 0x05
#define R_SRC_COLOR           0x06
#define R_DST_COLOR           0x07
#define R_ONE_MINUS_SRC_COLOR 0x08
#define R_ONE_MINUS_DST_COLOR 0x09

#define R_TRIANGLES      0x01
#define R_TRIANGLE_STRIP 0x02
#define R_POINTS         0x03
#define R_LINES          0x04
#define R_LINE_STRIP     0x05

#define R_RGB            0x00
#define R_RGBA           0x01
#define R_BGR            0x02
#define R_BGRA           0x03
#define R_DXT1           0x04
#define R_DXT3           0x05
#define R_DXT5           0x06

#define R_TEXTURE0       0x00
#define R_TEXTURE1       0x01
#define R_TEXTURE2       0x02
#define R_TEXTURE3       0x03
#define R_TEXTURE4       0x04
#define R_TEXTURE5       0x05
#define R_TEXTURE6       0x06
#define R_TEXTURE7       0x07
#define R_TEXTURE8       0x08
#define R_TEXTURE9       0x09
#define R_TEXTURE10      0x0a
#define R_TEXTURE11      0x0b
#define R_TEXTURE12      0x0c
#define R_TEXTURE13      0x0d
#define R_TEXTURE14      0x0e
#define R_TEXTURE15      0x0f

#define R_VB_XYZ       0x01
#define R_VB_TC0       0x02
#define R_VB_TC1       0x04
#define R_VB_NORMAL    0x08
#define R_VB_COLOR     0x10
#define R_VB_TANGENT   0x20
#define R_VB_BITANGENT 0x40
#define R_VB_BONE      0x80
#define R_VB_WEIGHT    0x100


#define R_VB_STATIC  0x01
#define R_VB_DYNAMIC 0x02

#define R_COMBINE_REPLACE  0x01
#define R_COMBINE_MODULATE 0x02
#define R_COMBINE_ADD      0x03
#define R_COMBINE_SUBTRACT 0x04
#define R_COMBINE_DOT3     0x05
#define R_COMBINE_INTERPOLATE 0x06

#define R_FILTER_NEAREST   0x01
#define R_FILTER_LINEAR    0x02

#define R_WRAP_REPEAT      0x01
#define R_WRAP_CLAMP       0x02


#define R_VERTEX_BUFFER 0x01
#define R_INDEX_BUFFER 0x02

#define R_ADD 0x01
#define R_SUBTRACT 0x02

#define R_GT      0x01
#define R_LT      0x02
#define R_GTE     0x03
#define R_LTE     0x04
#define R_EQ      0x05
#define R_NEQ     0x06
#define R_ALWAYS  0x07
#define R_NEVER   0x08



#endif

