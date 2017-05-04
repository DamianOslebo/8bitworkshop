
#include <string.h>

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short word;
typedef enum { false, true } bool;

byte __at (0x0) vidmem[152][256]; // 304x256x4bpp video memory
byte __at (0xc000) palette[16];
volatile byte __at (0xc804) input0;
volatile byte __at (0xc806) input1;
volatile byte __at (0xc80c) input2;
byte __at (0xc80c) sound_pia;
byte __at (0xc900) rom_select;
volatile byte __at (0xcb00) video_counter;
byte __at (0xcbff) watchdog0x39;
byte __at (0xcc00) nvram[0x400];

__sfr __at (0) debug;

// blitter flags
#define SRCSCREEN 0x1
#define DSTSCREEN 0x2
#define ESYNC 0x4
#define FGONLY 0x8
#define SOLID 0x10
#define RSHIFT 0x20
#define EVENONLY 0x40
#define ODDONLY 0x80

struct {
  byte flags;
  byte solid;
  word sstart;
  word dstart;
  byte width;
  byte height;
} __at (0xca00) blitter;

// switch flags
#define UP1 (input0 & 0x1)
#define DOWN1 (input0 & 0x2)
#define LEFT1 (input0 & 0x4)
#define RIGHT1 (input0 & 0x8)
#define START1 (input0 & 0x10)
#define START2 (input0 & 0x20)
#define UP2 (input0 & 0x40)
#define DOWN2 (input0 & 0x80)
#define LEFT2 (input1 & 0x1)
#define RIGHT2 (input1 & 0x2)
#define AUTOUP (input2 & 0x1)
#define ADVANCE (input2 & 0x2)
#define COIN2 (input2 & 0x4)
#define HIGHSCORERESET (input2 & 0x8)
#define COIN1 (input2 & 0x10)
#define COIN3 (input2 & 0x20)
#define TILTSWITCH (input2 & 0x40)
#define SOUNDACK (input2 & 0x80)

#define WATCHDOG watchdog0x39=0x39;

//

void main();

// start routine @ 0x0
// set stack pointer, enable interrupts
void start() {
__asm
        LD      SP,#0xc000
        DI
; copy initialized data
        LD    BC, #l__INITIALIZER
        LD    A, B
        LD    DE, #s__INITIALIZED
        LD    HL, #s__INITIALIZER
        LDIR
__endasm;
  
  main();
}

#define LOCHAR 0x20
#define HICHAR 0x7e

const char CH_32[] = { 0x21,0x51,0x00 };
const char CH_33[] = { 0x21,0x55,0xf0,0xf0,0xf0,0x00,0xf0 };
const char CH_34[] = { 0x42,0x52,0xf0,0xf0,0xf0,0xf0 };
const char CH_35[] = { 0x42,0x55,0xf0,0xf0,0xff,0xf0,0xf0,0xf0,0xff,0xf0,0xf0,0xf0 };
const char CH_36[] = { 0x42,0x55,0x0f,0xf0,0xff,0x00,0x0f,0xf0,0xff,0x00,0x0f,0x00 };
const char CH_37[] = { 0x42,0x55,0xf0,0x00,0x00,0xf0,0x0f,0x00,0xf0,0x00,0x00,0xf0 };
const char CH_38[] = { 0x42,0x55,0xff,0x00,0xff,0x00,0xff,0xf0,0xf0,0xf0,0x0f,0xf0 };
const char CH_39[] = { 0x21,0x52,0xf0,0xf0 };
const char CH_40[] = { 0x32,0x55,0x0f,0x00,0xf0,0x00,0xf0,0x00,0xf0,0x00,0x0f,0x00 };
const char CH_41[] = { 0x32,0x55,0xf0,0x00,0x0f,0x00,0x0f,0x00,0x0f,0x00,0xf0,0x00 };
const char CH_42[] = { 0x42,0x53,0xf0,0xf0,0x0f,0x00,0xf0,0xf0 };
const char CH_43[] = { 0x42,0x43,0x0f,0x00,0xff,0xf0,0x0f,0x00 };
const char CH_44[] = { 0x32,0x22,0x0f,0x00,0xf0,0x00 };
const char CH_45[] = { 0x42,0x31,0xff,0xf0 };
const char CH_46[] = { 0x21,0x11,0xf0 };
const char CH_47[] = { 0x42,0x55,0x00,0xf0,0x00,0xf0,0x0f,0x00,0xf0,0x00,0xf0,0x00 };
const char CH_48[] = { 0x42,0x55,0x0f,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xff,0x00 };
const char CH_49[] = { 0x32,0x55,0x0f,0x00,0xff,0x00,0x0f,0x00,0x0f,0x00,0x0f,0x00 };
const char CH_50[] = { 0x42,0x55,0xff,0x00,0x00,0xf0,0x0f,0x00,0xf0,0x00,0xff,0xf0 };
const char CH_51[] = { 0x42,0x55,0xff,0x00,0x00,0xf0,0x0f,0x00,0x00,0xf0,0xff,0x00 };
const char CH_52[] = { 0x42,0x55,0xf0,0xf0,0xf0,0xf0,0xff,0xf0,0x00,0xf0,0x00,0xf0 };
const char CH_53[] = { 0x42,0x55,0xff,0xf0,0xf0,0x00,0xff,0x00,0x00,0xf0,0xff,0x00 };
const char CH_54[] = { 0x42,0x55,0x0f,0xf0,0xf0,0x00,0xff,0xf0,0xf0,0xf0,0xff,0xf0 };
const char CH_55[] = { 0x42,0x55,0xff,0xf0,0x00,0xf0,0x0f,0x00,0xf0,0x00,0xf0,0x00 };
const char CH_56[] = { 0x42,0x55,0xff,0xf0,0xf0,0xf0,0xff,0xf0,0xf0,0xf0,0xff,0xf0 };
const char CH_57[] = { 0x42,0x55,0xff,0xf0,0xf0,0xf0,0xff,0xf0,0x00,0xf0,0xff,0x00 };
const char CH_58[] = { 0x21,0x43,0xf0,0x00,0xf0 };
const char CH_59[] = { 0x32,0x44,0x0f,0x00,0x00,0x00,0x0f,0x00,0xf0,0x00 };
const char CH_60[] = { 0x42,0x55,0x00,0xf0,0x0f,0x00,0xf0,0x00,0x0f,0x00,0x00,0xf0 };
const char CH_61[] = { 0x42,0x43,0xff,0xf0,0x00,0x00,0xff,0xf0 };
const char CH_62[] = { 0x42,0x55,0xf0,0x00,0x0f,0x00,0x00,0xf0,0x0f,0x00,0xf0,0x00 };
const char CH_63[] = { 0x42,0x55,0xff,0xf0,0x00,0xf0,0x0f,0x00,0x00,0x00,0x0f,0x00 };
const char CH_64[] = { 0x42,0x55,0x0f,0x00,0xf0,0xf0,0xff,0xf0,0xf0,0x00,0x0f,0xf0 };
const char CH_65[] = { 0x42,0x55,0x0f,0x00,0xf0,0xf0,0xff,0xf0,0xf0,0xf0,0xf0,0xf0 };
const char CH_66[] = { 0x42,0x55,0xff,0x00,0xf0,0xf0,0xff,0x00,0xf0,0xf0,0xff,0x00 };
const char CH_67[] = { 0x42,0x55,0x0f,0xf0,0xf0,0x00,0xf0,0x00,0xf0,0x00,0x0f,0xf0 };
const char CH_68[] = { 0x42,0x55,0xff,0x00,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xff,0x00 };
const char CH_69[] = { 0x42,0x55,0xff,0xf0,0xf0,0x00,0xff,0xf0,0xf0,0x00,0xff,0xf0 };
const char CH_70[] = { 0x42,0x55,0xff,0xf0,0xf0,0x00,0xff,0xf0,0xf0,0x00,0xf0,0x00 };
const char CH_71[] = { 0x42,0x55,0x0f,0xf0,0xf0,0x00,0xff,0xf0,0xf0,0xf0,0x0f,0xf0 };
const char CH_72[] = { 0x42,0x55,0xf0,0xf0,0xf0,0xf0,0xff,0xf0,0xf0,0xf0,0xf0,0xf0 };
const char CH_73[] = { 0x42,0x55,0xff,0xf0,0x0f,0x00,0x0f,0x00,0x0f,0x00,0xff,0xf0 };
const char CH_74[] = { 0x42,0x55,0x00,0xf0,0x00,0xf0,0x00,0xf0,0xf0,0xf0,0x0f,0x00 };
const char CH_75[] = { 0x42,0x55,0xf0,0xf0,0xf0,0xf0,0xff,0x00,0xf0,0xf0,0xf0,0xf0 };
const char CH_76[] = { 0x42,0x55,0xf0,0x00,0xf0,0x00,0xf0,0x00,0xf0,0x00,0xff,0xf0 };
const char CH_77[] = { 0x42,0x55,0xf0,0xf0,0xff,0xf0,0xff,0xf0,0xf0,0xf0,0xf0,0xf0 };
const char CH_78[] = { 0x42,0x55,0xf0,0xf0,0xff,0xf0,0xff,0xf0,0xff,0xf0,0xf0,0xf0 };
const char CH_79[] = { 0x42,0x55,0x0f,0x00,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x0f,0x00 };
const char CH_80[] = { 0x42,0x55,0xff,0x00,0xf0,0xf0,0xff,0x00,0xf0,0x00,0xf0,0x00 };
const char CH_81[] = { 0x42,0x55,0x0f,0x00,0xf0,0xf0,0xf0,0xf0,0xff,0xf0,0x0f,0xf0 };
const char CH_82[] = { 0x42,0x55,0xff,0x00,0xf0,0xf0,0xff,0xf0,0xff,0x00,0xf0,0xf0 };
const char CH_83[] = { 0x42,0x55,0x0f,0xf0,0xf0,0x00,0x0f,0x00,0x00,0xf0,0xff,0x00 };
const char CH_84[] = { 0x42,0x55,0xff,0xf0,0x0f,0x00,0x0f,0x00,0x0f,0x00,0x0f,0x00 };
const char CH_85[] = { 0x42,0x55,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x0f,0xf0 };
const char CH_86[] = { 0x42,0x55,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x0f,0x00,0x0f,0x00 };
const char CH_87[] = { 0x42,0x55,0xf0,0xf0,0xf0,0xf0,0xff,0xf0,0xff,0xf0,0xf0,0xf0 };
const char CH_88[] = { 0x42,0x55,0xf0,0xf0,0xf0,0xf0,0x0f,0x00,0xf0,0xf0,0xf0,0xf0 };
const char CH_89[] = { 0x42,0x55,0xf0,0xf0,0xf0,0xf0,0x0f,0x00,0x0f,0x00,0x0f,0x00 };
const char CH_90[] = { 0x42,0x55,0xff,0xf0,0x00,0xf0,0x0f,0x00,0xf0,0x00,0xff,0xf0 };
const char CH_91[] = { 0x42,0x55,0xff,0xf0,0xf0,0x00,0xf0,0x00,0xf0,0x00,0xff,0xf0 };
const char CH_92[] = { 0x42,0x43,0xf0,0x00,0x0f,0x00,0x00,0xf0 };
const char CH_93[] = { 0x42,0x55,0xff,0xf0,0x00,0xf0,0x00,0xf0,0x00,0xf0,0xff,0xf0 };
const char CH_94[] = { 0x42,0x52,0x0f,0x00,0xf0,0xf0 };
const char CH_95[] = { 0x42,0x11,0xff,0xf0 };
const char CH_96[] = { 0x32,0x52,0xf0,0x00,0x0f,0x00 };
const char CH_97[] = { 0x42,0x44,0xff,0x00,0x0f,0xf0,0xf0,0xf0,0xff,0xf0 };
const char CH_98[] = { 0x42,0x55,0xf0,0x00,0xff,0x00,0xf0,0xf0,0xf0,0xf0,0xff,0x00 };
const char CH_99[] = { 0x42,0x44,0x0f,0xf0,0xf0,0x00,0xf0,0x00,0x0f,0xf0 };
const char CH_100[] = { 0x42,0x55,0x00,0xf0,0x0f,0xf0,0xf0,0xf0,0xf0,0xf0,0x0f,0xf0 };
const char CH_101[] = { 0x42,0x44,0x0f,0xf0,0xf0,0xf0,0xff,0x00,0x0f,0xf0 };
const char CH_102[] = { 0x42,0x55,0x00,0xf0,0x0f,0x00,0xff,0xf0,0x0f,0x00,0x0f,0x00 };
const char CH_103[] = { 0x42,0x45,0x0f,0xf0,0xf0,0xf0,0xff,0xf0,0x00,0xf0,0x0f,0x00 };
const char CH_104[] = { 0x42,0x55,0xf0,0x00,0xff,0x00,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0 };
const char CH_105[] = { 0x21,0x55,0xf0,0x00,0xf0,0xf0,0xf0 };
const char CH_106[] = { 0x42,0x56,0x00,0xf0,0x00,0x00,0x00,0xf0,0x00,0xf0,0xf0,0xf0,0x0f,0x00 };
const char CH_107[] = { 0x42,0x55,0xf0,0x00,0xf0,0xf0,0xff,0x00,0xff,0x00,0xf0,0xf0 };
const char CH_108[] = { 0x42,0x55,0xff,0x00,0x0f,0x00,0x0f,0x00,0x0f,0x00,0xff,0xf0 };
const char CH_109[] = { 0x42,0x44,0xff,0xf0,0xff,0xf0,0xff,0xf0,0xf0,0xf0 };
const char CH_110[] = { 0x42,0x44,0xff,0x00,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0 };
const char CH_111[] = { 0x42,0x44,0x0f,0x00,0xf0,0xf0,0xf0,0xf0,0x0f,0x00 };
const char CH_112[] = { 0x42,0x45,0xff,0x00,0xf0,0xf0,0xf0,0xf0,0xff,0x00,0xf0,0x00 };
const char CH_113[] = { 0x42,0x45,0x0f,0xf0,0xf0,0xf0,0xf0,0xf0,0x0f,0xf0,0x00,0xf0 };
const char CH_114[] = { 0x42,0x44,0x0f,0xf0,0xf0,0x00,0xf0,0x00,0xf0,0x00 };
const char CH_115[] = { 0x42,0x44,0x0f,0xf0,0xff,0x00,0x0f,0xf0,0xff,0x00 };
const char CH_116[] = { 0x42,0x55,0x0f,0x00,0xff,0xf0,0x0f,0x00,0x0f,0x00,0x0f,0xf0 };
const char CH_117[] = { 0x42,0x44,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0x0f,0xf0 };
const char CH_118[] = { 0x42,0x44,0xf0,0xf0,0xf0,0xf0,0xff,0xf0,0x0f,0x00 };
const char CH_119[] = { 0x42,0x44,0xf0,0xf0,0xff,0xf0,0xff,0xf0,0xff,0xf0 };
const char CH_120[] = { 0x42,0x44,0xf0,0xf0,0x0f,0x00,0x0f,0x00,0xf0,0xf0 };
const char CH_121[] = { 0x42,0x45,0xf0,0xf0,0xf0,0xf0,0x0f,0xf0,0x00,0xf0,0x0f,0x00 };
const char CH_122[] = { 0x42,0x44,0xff,0xf0,0x0f,0xf0,0xff,0x00,0xff,0xf0 };
const char CH_123[] = { 0x42,0x55,0x0f,0xf0,0x0f,0x00,0xf0,0x00,0x0f,0x00,0x0f,0xf0 };
const char CH_124[] = { 0x21,0x55,0xf0,0xf0,0x00,0xf0,0xf0 };
const char CH_125[] = { 0x42,0x55,0xff,0x00,0x0f,0x00,0x00,0xf0,0x0f,0x00,0xff,0x00 };
const char CH_126[] = { 0x42,0x52,0x0f,0xf0,0xff,0x00 };
const char* const FONT_TABLE[95] = { CH_32, CH_33, CH_34, CH_35, CH_36, CH_37, CH_38, CH_39, CH_40, CH_41, CH_42, CH_43, CH_44, CH_45, CH_46, CH_47, CH_48, CH_49, CH_50, CH_51, CH_52, CH_53, CH_54, CH_55, CH_56, CH_57, CH_58, CH_59, CH_60, CH_61, CH_62, CH_63, CH_64, CH_65, CH_66, CH_67, CH_68, CH_69, CH_70, CH_71, CH_72, CH_73, CH_74, CH_75, CH_76, CH_77, CH_78, CH_79, CH_80, CH_81, CH_82, CH_83, CH_84, CH_85, CH_86, CH_87, CH_88, CH_89, CH_90, CH_91, CH_92, CH_93, CH_94, CH_95, CH_96, CH_97, CH_98, CH_99, CH_100, CH_101, CH_102, CH_103, CH_104, CH_105, CH_106, CH_107, CH_108, CH_109, CH_110, CH_111, CH_112, CH_113, CH_114, CH_115, CH_116, CH_117, CH_118, CH_119, CH_120, CH_121, CH_122, CH_123, CH_124, CH_125, CH_126, };

inline word swapw(word j) {
  return ((j << 8) | (j >> 8));
}

// x1: 0-151
// y1: 0-255
inline void blit_solid(byte x1, byte y1, byte w, byte h, byte color) {
  blitter.width = w^4;
  blitter.height = h^4;
  blitter.dstart = x1+y1*256; // swapped
  blitter.solid = color;
  blitter.flags = DSTSCREEN|SOLID;
}

inline void draw_solid(word x1, byte y1, byte w, byte h, byte color) {
  blitter.width = w^4;
  blitter.height = h^4;
  blitter.dstart = (x1>>1)+y1*256; // swapped
  blitter.solid = color;
  blitter.flags = (x1&1) ? DSTSCREEN|SOLID|RSHIFT : DSTSCREEN|SOLID;
}

inline void draw_vline(word x1, byte y1, byte h, byte color) {
  blitter.width = 1^4;
  blitter.height = h^4;
  blitter.dstart = (x1>>1)+y1*256; // swapped
  blitter.solid = color;
  blitter.flags = (x1&1) ? DSTSCREEN|SOLID|ODDONLY : DSTSCREEN|SOLID|EVENONLY;
}

inline void blit_copy_solid(word x, byte y, byte w, byte h, const byte* data, byte solid) {
  blitter.width = w^4;
  blitter.height = h^4;
  blitter.solid = solid;
  blitter.sstart = swapw((word)data);
  blitter.dstart = (x>>1)+y*256; // swapped
  if (solid)
    blitter.flags = (x&1) ? DSTSCREEN|FGONLY|SOLID|RSHIFT : DSTSCREEN|FGONLY|SOLID;
  else
    blitter.flags = (x&1) ? DSTSCREEN|RSHIFT : DSTSCREEN;
}

// bias sprites by +4 pixels
#define XBIAS 2

inline void draw_sprite(const byte* data, byte x, byte y) {
  blitter.width = data[0]^4;
  blitter.height = data[1]^4;
  blitter.sstart = swapw((word)(data+2));
  blitter.dstart = (x>>1)+y*256+XBIAS; // swapped
  blitter.flags = (x&1) ? DSTSCREEN|FGONLY|RSHIFT : DSTSCREEN|FGONLY;
}

inline void draw_sprite_solid(const byte* data, byte x, byte y, byte color) {
  blitter.width = data[0]^4;
  blitter.height = data[1]^4;
  blitter.sstart = swapw((word)(data+2));
  blitter.dstart = (x>>1)+y*256+XBIAS; // swapped
  blitter.solid = color;
  blitter.flags = (x&1) ? DSTSCREEN|FGONLY|RSHIFT|SOLID : DSTSCREEN|FGONLY|SOLID;
}

void draw_sprite_strided(const byte* data, byte x, byte y, byte stride) {
  byte i;
  byte width = data[0];
  byte height = data[1];
  byte yy = y-(height*(stride-1)/2);
  word dest = (x>>1)+yy*256+XBIAS;
  blitter.width = width^4;
  blitter.height = 1^4;
  for (i=0; i<height; i++) {
    if ((dest & 0xff) < 0x90) {
      blitter.sstart = swapw((word)(data+2));
      blitter.dstart = dest; // swapped
      blitter.flags = DSTSCREEN|FGONLY;
    }
    dest += stride << 8;
    data += width;
  }
}

void erase_sprite_strided(const byte* data, byte x, byte y, byte stride) {
  byte i;
  byte width = data[0];
  byte height = data[1];
  byte yy = y-(height*(stride-1)/2);
  word dest = (x>>1)+yy*256+XBIAS;
  blitter.width = width^4;
  blitter.height = 1^4;
  blitter.solid = 0;
  for (i=0; i<height; i++) {
    if ((dest & 0xff) < 0x90) {
      blitter.dstart = dest; // swapped
      blitter.flags = DSTSCREEN|SOLID;
    }
    dest += stride << 8;
  }
}

byte draw_char(char ch, word x, byte y, byte color) {
  const byte* shape;
  if (ch < LOCHAR || ch > HICHAR) return 1;
  shape = FONT_TABLE[ch - LOCHAR];
  blit_copy_solid(x, y - (shape[1]>>4),
                  shape[0] & 0xf, shape[1] & 0xf, shape+2, color);
  return shape[0] >> 4;
}

void draw_string(const char* str, word x, byte y, byte color) {
  while (*str) {
    x += draw_char(*str++, x, y, color);
  }
}

void draw_box(word x1, byte y1, word x2, byte y2, byte color) {
  draw_solid(x1, y1, (x2-x1)>>1, 1, color);
  draw_solid(x1, y2, (x2-x1)>>1, 1, color);
  draw_vline(x1, y1, y2-y1, color);
  draw_vline(x2, y1, y2-y1, color);
}

// GRAPHIC DATA

const byte palette_data[16] = {/*{pal:332,n:16}*/
  0x00, 0x03, 0x19, 0x50, 0x52, 0x07, 0x1f, 0x37, 0xe0, 0xa4, 0xfd, 0xff, 0x38, 0x70, 0x7f, 0xf8, };

const byte laser_vert[2+1*8] = {
  1,8,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
};

const byte laser_horiz[2+4*2] = {
  4,2,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
};


const byte playersprite1[] = {
7,16,/*{w:14,h:16,bpp:4,brev:1}*/
0x00,0x00,0xEF,0xEF,0xE0,0x00,0x00,
0x00,0xEE,0xEE,0xFE,0xEE,0xE0,0x00,
0x0E,0xED,0xEF,0xEF,0xED,0xEE,0x00,
0x0E,0xEE,0xDD,0xDD,0xDE,0xEE,0x00,
0x0E,0xEE,0xEC,0xDC,0xEE,0xEE,0x00,
0x00,0xEE,0xEE,0xDE,0xEE,0xE0,0x00,
0x00,0xEE,0xEE,0xDE,0xEE,0xE0,0x00,
0x00,0x00,0xED,0xDD,0xE0,0x00,0x00,
0xDD,0xDD,0xEE,0xEE,0xED,0xDD,0xD0,
0x0C,0xEE,0xEE,0xEE,0xEE,0xEC,0x00,
0x0E,0xE0,0xEE,0xEE,0xE0,0xEE,0x00,
0x0E,0xE0,0xEE,0xEE,0xE0,0xEE,0x00,
0x0E,0xE0,0xDD,0xDD,0xD0,0xDE,0x00,
0x0D,0x00,0xEE,0x0E,0xE0,0x0D,0x00,
0x00,0x00,0xEC,0x0C,0xE0,0x00,0x00,
0x00,0x0D,0xDD,0x0D,0xDD,0x00,0x00,
};

const byte sprite1[2+16*16/2] = {
8,16,/*{w:16,h:16,bpp:4,brev:1}*/
0x00,0x09,0x99,0x00,0x00,0x99,0x90,0x00,
0x00,0x94,0x94,0x90,0x09,0x49,0x49,0x00,
0x04,0x49,0x49,0x90,0x09,0x94,0x94,0x90,
0x94,0x99,0x94,0x90,0x09,0x49,0x99,0x49,
0x99,0x99,0x49,0x93,0x39,0x94,0x99,0x99,
0x04,0x49,0x99,0x94,0x49,0x99,0x94,0x90,
0x00,0x94,0x94,0x43,0x34,0x49,0x49,0x00,
0x00,0x09,0x43,0x94,0x49,0x34,0x90,0x00,
0x00,0x90,0x00,0x39,0x93,0x00,0x09,0x00,
0x00,0x09,0x83,0x33,0x33,0x33,0x90,0x00,
0x00,0x09,0x32,0x23,0x32,0x23,0x90,0x00,
0x00,0x03,0x03,0x23,0x82,0x30,0x30,0x00,
0x03,0x30,0x00,0x33,0x33,0x00,0x03,0x30,
0x00,0x30,0x03,0x00,0x00,0x30,0x03,0x00,
0x00,0x00,0x00,0x30,0x03,0x00,0x00,0x00,
0x00,0x09,0x99,0x00,0x00,0x99,0x90,0x00,
};

const byte sprite2[2+16*16/2] = {
8,16,/*{w:16,h:16,bpp:4,brev:1}*/
0x00,0x94,0x94,0x90,0x09,0x49,0x49,0x00,
0x04,0x49,0x49,0x90,0x09,0x94,0x94,0x90,
0x94,0x99,0x94,0x90,0x09,0x49,0x99,0x49,
0x99,0x99,0x49,0x93,0x39,0x94,0x99,0x99,
0x04,0x49,0x99,0x94,0x49,0x99,0x94,0x90,
0x00,0x94,0x94,0x43,0x34,0x49,0x49,0x00,
0x00,0x09,0x43,0x94,0x49,0x34,0x90,0x00,
0x00,0x90,0x00,0x39,0x93,0x00,0x09,0x00,
0x00,0x09,0x83,0x33,0x33,0x33,0x90,0x00,
0x00,0x09,0x32,0x23,0x32,0x23,0x90,0x00,
0x00,0x03,0x03,0x23,0x82,0x30,0x30,0x00,
0x03,0x30,0x00,0x33,0x33,0x00,0x03,0x30,
0x00,0x30,0x03,0x00,0x00,0x30,0x03,0x00,
0x00,0x00,0x00,0x30,0x03,0x00,0x00,0x00,
0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,
};

const byte sprite3[2+16*16/2] = {
8,16,/*{w:16,h:16,bpp:4,brev:1}*/
0x00,0x00,0x00,0x01,0x10,0x00,0x00,0x00,
0x00,0x00,0x00,0x11,0x11,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x10,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x10,0x00,0x00,0x00,
0x00,0x11,0x10,0x01,0x10,0x01,0x11,0x00,
0x11,0x00,0x10,0x11,0x11,0x01,0x00,0x11,
0x10,0x00,0x11,0x11,0x11,0x11,0x00,0x01,
0x10,0x00,0x00,0x11,0x11,0x00,0x00,0x01,
0x00,0x01,0x10,0x11,0x41,0x01,0x10,0x00,
0x00,0x10,0x11,0x11,0x11,0x11,0x01,0x00,
0x01,0x00,0x00,0x51,0x15,0x00,0x00,0x10,
0x01,0x00,0x01,0x10,0x01,0x10,0x00,0x10,
0x01,0x00,0x10,0x00,0x00,0x01,0x00,0x10,
0x00,0x10,0x01,0x00,0x00,0x10,0x01,0x00,
0x00,0x10,0x00,0x00,0x00,0x00,0x01,0x00,
0x00,0x01,0x10,0x00,0x00,0x01,0x10,0x00,
};

const byte sprite4[2+16*16/2] = {
8,16,/*{w:16,h:16,bpp:4,brev:1}*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xbb,0xbb,0x00,0x00,0x00,
0x00,0x00,0x00,0xbb,0xbb,0x00,0x00,0x00,
0x00,0x00,0xbb,0xbb,0xbb,0xbb,0x00,0x00,
0x00,0x0b,0x4b,0xbb,0xbb,0xb4,0xb0,0x00,
0x00,0x0b,0x44,0xbb,0xbb,0x49,0xb0,0x00,
0x00,0xbb,0x99,0x4b,0xb4,0x99,0xbb,0x00,
0x00,0x4b,0xb9,0x9b,0xb4,0x4b,0xb4,0x00,
0x00,0x04,0xbb,0x4b,0xb9,0xbb,0x40,0x00,
0x00,0x00,0x4b,0xbb,0xbb,0xb4,0x00,0x00,
0x11,0x00,0x0b,0xbb,0xbb,0xb0,0x00,0x11,
0x10,0x10,0x0b,0x41,0x14,0xb0,0x01,0x01,
0x10,0x00,0x05,0x11,0x11,0x50,0x00,0x01,
0x01,0x00,0x01,0x51,0x15,0x10,0x00,0x10,
0x00,0x10,0x01,0x10,0x01,0x10,0x01,0x00,
0x01,0x01,0x10,0x10,0x01,0x01,0x10,0x10,
};

const byte sprite5[2+16*16/2] = {
8,16,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x22,0x00,0x00,0x22,0x00,0x00,
0x00,0x20,0x20,0x00,0x00,0x02,0x02,0x00,
0x02,0x22,0x12,0x00,0x00,0x21,0x22,0x20,
0x00,0x02,0x11,0x00,0x00,0x11,0x20,0x00,
0x00,0x00,0x02,0x11,0x11,0x20,0x00,0x00,
0x00,0x00,0x02,0x22,0x22,0x20,0x00,0x00,
0x00,0x00,0x21,0x22,0x22,0x12,0x00,0x00,
0x00,0x00,0x22,0x12,0x21,0x22,0x00,0x00,
0x02,0x00,0x22,0x12,0x21,0x22,0x00,0x20,
0x20,0x02,0x21,0x11,0x11,0x12,0x20,0x02,
0x02,0x22,0x15,0x12,0x21,0x51,0x22,0x20,
0x02,0x11,0x15,0x52,0x25,0x51,0x11,0x20,
0x20,0x00,0x22,0x12,0x21,0x22,0x00,0x02,
0x20,0x02,0x20,0x20,0x02,0x02,0x20,0x02,
0x02,0x02,0x00,0x20,0x02,0x00,0x20,0x20,
};

const byte sprite6[2+12*16/2] = {
8,12,/*{w:16,h:12,bpp:4,brev:1}*/
0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x00,
0x00,0x00,0x00,0x00,0x44,0x44,0x44,0x40,
0x00,0x00,0x04,0x04,0x49,0x49,0x99,0x44,
0x00,0x00,0x44,0x44,0x99,0x94,0x44,0x40,
0x00,0x04,0x49,0x99,0x94,0x49,0x99,0x40,
0x00,0x44,0x99,0x94,0x49,0x99,0x44,0x00,
0x04,0x49,0x44,0x99,0x99,0x44,0x00,0x00,
0x04,0x44,0x00,0x49,0x44,0x40,0x00,0x00,
0x04,0x40,0x00,0x04,0x00,0x00,0x00,0x00,
0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

const byte sprite7[2+16*16/2] = {
8,16,/*{w:16,h:16,bpp:4,brev:1}*/
0x00,0x60,0x00,0x00,0x00,0x00,0x06,0x00,
0x60,0x60,0x00,0x00,0x00,0x00,0x06,0x06,
0x60,0x60,0x50,0x00,0x00,0x05,0x06,0x06,
0x60,0x60,0x06,0x00,0x00,0x60,0x06,0x06,
0x66,0x60,0x06,0x06,0x60,0x60,0x06,0x66,
0x66,0x60,0x66,0x66,0x66,0x66,0x06,0x66,
0x66,0x60,0x6b,0x6b,0xb6,0xb6,0x06,0x66,
0x06,0x66,0x66,0xb6,0x6b,0x66,0x66,0x60,
0x00,0x67,0x66,0x66,0x66,0x66,0x66,0x00,
0x00,0x11,0x11,0x66,0x66,0x11,0x11,0x00,
0x01,0x11,0x21,0x11,0x11,0x12,0x11,0x10,
0x00,0x10,0x01,0x01,0x10,0x10,0x01,0x00,
0x00,0x10,0x11,0x00,0x00,0x11,0x01,0x00,
0x00,0x00,0x10,0x00,0x00,0x01,0x00,0x00,
};

const byte sprite8[2+16*16/2] = {
8,16,/*{w:16,h:16,bpp:4,brev:1}*/
0x00,0x00,0x11,0x11,0x11,0x11,0x00,0x00,
0x00,0x01,0x61,0x11,0x21,0x12,0x10,0x00,
0x00,0x11,0x12,0x11,0x61,0x11,0x11,0x00,
0x00,0x11,0x71,0x21,0x61,0x17,0x11,0x00,
0x00,0x11,0x77,0x11,0x12,0x77,0x11,0x00,
0x00,0x11,0x17,0x17,0x71,0x72,0x11,0x00,
0x00,0x16,0x21,0x78,0x87,0x11,0x11,0x00,
0x00,0x01,0x81,0x88,0x88,0x18,0x10,0x00,
0x00,0x08,0x18,0x58,0x85,0x81,0x80,0x00,
0x00,0x88,0x01,0x88,0x88,0x10,0x88,0x00,
0x00,0x08,0x07,0x78,0x87,0x70,0x80,0x00,
0x08,0x80,0x07,0x80,0x08,0x70,0x08,0x80,
0x88,0x00,0x88,0x00,0x00,0x88,0x00,0x88,
0x80,0x00,0x08,0x80,0x08,0x80,0x00,0x08,
0x00,0x00,0x00,0x80,0x08,0x00,0x00,0x00,
};

const byte sprite9[2+13*16/2] = {
8,13,/*{w:16,h:13,bpp:4,brev:1}*/
0x00,0x00,0xaa,0x00,0x00,0xaa,0x00,0x00,
0x00,0x00,0xa0,0x0a,0xa0,0x0a,0x00,0x00,
0x0a,0xaa,0xa4,0xaa,0xaa,0x3a,0xaa,0xa0,
0x00,0x0a,0x3a,0x9a,0xa9,0xa3,0xa0,0x00,
0x00,0x0a,0xaa,0x99,0xa4,0xaa,0xa0,0x00,
0x00,0x0a,0xaa,0x99,0x94,0xaa,0xa0,0x00,
0x00,0xaa,0x33,0xaa,0xaa,0x43,0xaa,0x00,
0x0a,0x3a,0xaa,0x3a,0xa3,0xaa,0xa3,0xa0,
0x00,0xaa,0xaa,0x30,0x03,0xaa,0xaa,0x00,
0x00,0x0a,0xa0,0x30,0x03,0x0a,0xa0,0x00,
0x00,0xa3,0xa0,0x00,0x00,0x0a,0x4a,0x00,
0x00,0xaa,0xa0,0x00,0x00,0x0a,0xaa,0x00,
0x00,0x0a,0xa0,0x00,0x00,0x0a,0xa0,0x00,
};

const byte* const all_sprites[9] = {
  sprite1,
  sprite2,
  sprite3,
  sprite4,
  sprite5,
  sprite6,
  sprite7,
  sprite8,
  sprite9,
};

// GAME CODE

typedef struct Actor;

typedef void (*ActorUpdateFn)(struct Actor* a);
typedef void (*ActorDrawFn)(struct Actor* a);

typedef struct Actor {
  struct Actor* next;
  struct Actor** prevptr;
  byte x,y;
  byte* shape;
  ActorUpdateFn update;
  ActorDrawFn draw;
  union {
    struct { sbyte dx,dy; } laser;
    struct { byte exploding; } enemy;
  } u;
} Actor;

#define MAX_ACTORS 128

static Actor actors[MAX_ACTORS];
static Actor* player_list;
static Actor* fast_list;
static Actor* slow_lists[4];
static Actor* obstacle_list;
static Actor* free_list;
static Actor* effects_list;
static byte frame;

static word score; // bcd score

#define PLAYER 1
#define LASER 2

void add_actor(Actor** list, Actor* a) {
  if (*list) (*list)->prevptr = &a->next;
  a->next = *list;
  a->prevptr = list;
  *list = a;
}

void remove_actor(Actor* a) {
  if (a->next) a->next->prevptr = a->prevptr;
  *a->prevptr = a->next;
}

void draw_actor_normal(Actor* a) {
  draw_sprite(a->shape, a->x, a->y);
}

void draw_actor_exploding(Actor* a) {
  erase_sprite_strided(a->shape, a->x, a->y, a->u.enemy.exploding);
  if (a->u.enemy.exploding > 10) {
    a->draw = NULL;
  } else {
    draw_sprite_strided(a->shape, a->x, a->y, ++a->u.enemy.exploding);
  }
}

void update_actor(Actor* a) {
  // if NULL shape, we don't have anything
  if (a->shape) {
    // erase the sprite
    draw_sprite_solid(a->shape, a->x, a->y, 0);
    // call update callback
    if (a->update) {
      a->update(a);
    }
    // did we delete it?
    if (a->shape) {
      // draw the sprite
      if (a->draw) {
        a->draw(a);
      }
    } else {
      // shape became null, remove from list
      remove_actor(a);
    }
  }
}

//

word lfsr = 1;

word rand() {
  byte lsb = lfsr & 1;
  lfsr >>= 1;
  if (lsb) lfsr ^= 0xd400;
  return lfsr;
}

word bcd_add(word a, word b) {
  a; b; // to avoid "unused variable" warning
__asm
        ld      hl,#4
        add     hl,sp
        ld      iy,#2
        add     iy,sp
        ld      a,0 (iy)	; load a (lo)
        add     a, (hl)		; add b (lo)
        daa			; convert to BCD
        ld      c,a
        ld      a,1 (iy)	; load a (hi)
        inc     hl
        adc     a, (hl)		; add b (hi) + carry
        daa
        ld      b,a
        ld      l, c
        ld      h, b		; result in HL reg
__endasm;
}

signed char random_dir() {
  byte x = rand();
  if (x < 85) return 0;
  else if (x < 85*2) return -1;
  else return 1;
}

void update_actor_list(Actor* a) {
  while (a) {
    update_actor(a);
    a = a->next;
  }
}

static byte g_section;

void update_actors_partial(Actor* a) {
  while (a) {
    if (g_section ^ (a->y < 0x80)) {
      update_actor(a);
    }
    a = a->next;
  }
}

void update_screen_section(byte section) {
  g_section = section;
  update_actors_partial(player_list);
  update_actors_partial(fast_list);
  update_actors_partial(slow_lists[frame & 3]);
}

static Actor* test_actor;
static byte test_x, test_y;

byte minbyte(byte a, byte b) { return a<b?a:b; }
byte maxbyte(byte a, byte b) { return a>b?a:b; }

bool test_actor_pixels(Actor* a) {
  if (a->shape) {
    byte x,y;
    byte *p1, *p2;
    byte x1 = maxbyte(test_actor->x, a->x);
    byte y1 = maxbyte(test_actor->y, a->y);
    byte x2 = minbyte(test_actor->x + test_actor->shape[0]*2,
                      a->x + a->shape[0]*2);
    byte y2 = minbyte(test_actor->y + test_actor->shape[1],
                      a->y + a->shape[1]);
    if (x2 < x1) x2 = 255; // clip to 8-bit coord.
    if (y2 < y1) y2 = 255; // clip to 8-bit coord.
    if (x2 <= x1 || y2 <= y1) return false;
    //draw_box(x1,y1,x2,y2,0xff);
    p1 = &test_actor->shape[2+(y1-test_actor->y)*test_actor->shape[0]];
    p2 = &a->shape[2+(y1-a->y)*a->shape[0]];
    p1 += (x1 - test_actor->x) >> 1;
    p2 += (x1 - a->x) >> 1;
    for (y=y1; y<y2; y++) {
      for (x=x1; x<x2; x++) {
        if (p1[x] && p2[x]) return true;
      }
      p1 += test_actor->shape[0];
      p2 += a->shape[0];
    }
  }
  return false;
}

inline bool test_collision_actor(Actor* a) {
  return ((byte)(test_y - a->y + 16) < 32 &&
          (byte)(test_x - a->x + 16) < 32 &&
          test_actor_pixels(a));
}

Actor* test_collisions(Actor* a) {
  while (a) {
    if (test_collision_actor(a)) {
      return a;
    }
    a = a->next;
  }
  return NULL;
}

void setup_collision(Actor* a) {
  test_actor = a;
  test_x = a->x;
  test_y = a->y;
}

void destroy_player() {
  Actor* a = &actors[PLAYER];
  byte i;
  sound_pia = 1; // play sound
  for (i=0; i<60; i++) {
    WATCHDOG;
    while (video_counter != 0xfc) ;
    draw_sprite_solid(a->shape, a->x, a->y, i);
    while (video_counter == 0xfc) ;
  }
  for (i=1; i<60; i++) {
    WATCHDOG;
    while (video_counter != 0xfc) ;
    draw_sprite_strided(a->shape, a->x+i, a->y, i);
    draw_sprite_strided(a->shape, a->x-i, a->y, i);
    while (video_counter == 0xfc) ;
  }
}

void random_walk(Actor* a) {
  a->x += random_dir();
  a->y += random_dir();
  setup_collision(a);
  if (actors[PLAYER].shape && test_collision_actor(&actors[PLAYER])) {
    destroy_player();
    actors[PLAYER].shape = NULL;
  }
}

void laser_move(Actor* a) {
  // did we hit something?
  static Actor* collided;
  setup_collision(a);
  collided = test_collisions(fast_list);
  if (!collided) collided = test_collisions(slow_lists[0]);
  if (!collided) collided = test_collisions(slow_lists[1]);
  if (!collided) collided = test_collisions(slow_lists[2]);
  if (!collided) collided = test_collisions(slow_lists[3]);
  if (collided) {
    // get rid of laser (we can do this in our 'update' fn)
    a->shape = NULL;
    // set exploding counter for enemy, change fn pointers
    collided->draw = draw_actor_exploding;
    collided->update = NULL;
    collided->u.enemy.exploding = 1;
    // move enemy to effects list
    remove_actor(collided);
    add_actor(&effects_list, collided);
    // play sound
    sound_pia = 2;
    // add score
    score = bcd_add(score, 5);
    return;
  }
  // move laser
  // check for wall collisions
  a->x += a->u.laser.dx;
  if (a->u.laser.dx > 0 && a->x < 8) a->shape = NULL;
  if (a->u.laser.dx < 0 && a->x > 255-8) a->shape = NULL;
  a->y += a->u.laser.dy;
  if (a->y > 255-8) a->shape = NULL;
}

void shoot_laser(sbyte dx, sbyte dy, const byte* shape) {
  Actor* a = &actors[LASER];
  a->shape = (void*) shape;
  a->x = actors[PLAYER].x + 6;
  a->y = actors[PLAYER].y + 8;
  a->u.laser.dx = dx;
  a->u.laser.dy = dy;
  add_actor(&player_list, a);
}

void player_laser() {
  // is the laser being used?
  if (actors[LASER].shape == NULL) { // no, check controls
    if (UP2) shoot_laser(0,-8,laser_vert);
    else if (DOWN2) shoot_laser(0,8,laser_vert); 
    else if (LEFT2) shoot_laser(-8,0,laser_horiz);
    else if (RIGHT2) shoot_laser(8,0,laser_horiz);
  }
}

void player_move(Actor* a) {
  byte x = a->x;
  byte y = a->y;
  if (UP1) y-=1;
  if (DOWN1) y+=1;
  if (LEFT1) x-=1;
  if (RIGHT1) x+=1;
  a->x = x;
  a->y = y;
  player_laser();
}

static Actor* current_effect;

void effects_new_frame() {
  current_effect = effects_list;
}

void effects_next() {
  if (current_effect) {
    if (current_effect->draw) {
      current_effect->draw(current_effect);
    } else {
      remove_actor(current_effect);
    }
    current_effect = current_effect->next;
  }
}

Actor* new_actor() {
  Actor* a = free_list;
  remove_actor(a);
  a->draw = draw_actor_normal;
  return a;
}

Actor* new_effect(ActorDrawFn draw) {
  Actor* a = new_actor();
  a->draw = draw;
  add_actor(&effects_list, a);
  return a;
}

void init() {
  byte i;
  blit_solid(0, 0, 255, 255, 0);
  memset(actors, 0, sizeof(actors));
  memcpy(palette, palette_data, 16);
  player_list = fast_list = obstacle_list = free_list = NULL;
  memset(slow_lists, 0, sizeof(slow_lists));
  // add all actors to free list
  for (i=MAX_ACTORS-1; i>0; i--) {
    add_actor(&free_list, &actors[i]);
  }
  score = 0;
}

void make_player_actors() {
  // make player
  Actor* a = new_actor();
  a->x = 128;
  a->y = 120;
  a->shape = (void*) playersprite1;
  a->update = player_move;
  add_actor(&player_list, a);
  // make laser
  a = new_actor();
  a->update = laser_move;
  a->shape = NULL;
}

void make_enemy_actors() {
  byte i;
  const byte num_actors = 32;
  for (i=3; i<num_actors; i++) {
    Actor* a = new_actor();
    do {
      a->x = rand() + rand();
      a->y = rand() + rand();
    } while ((byte)(a->x - 96) < 64 && (byte)(a->y - 96) < 64);
    a->shape = (void*) all_sprites[i%9];
    a->update = random_walk;
    a->draw = draw_actor_normal;
    if (i < 5) add_actor(&fast_list, a);
    else add_actor(&slow_lists[i&3], a);
    WATCHDOG;
  }
}

void draw_bcd_word(word bcd, word x, byte y, byte color) {
  byte j;
  x += 4*4;
  for (j=0; j<4; j++) {
    x -= 4;
    draw_char('0'+(bcd & 0xf), x, y, color);
    bcd >>= 4;
  }
}

void draw_score(Actor* a) {
  a;
  draw_string("Plyr 1", 278, 8, 0xee);
  // draw digits in color 0 so the background is erased
  draw_bcd_word(score, 278, 20, 0);
  // last two digits of score are "00"
  draw_char('0', 278+16, 20, 0);
  draw_char('0', 278+20, 20, 0);
}

void draw_playfield(Actor* a) {
  a;
  draw_box(0,0,275,255,0x11);
}

void main() {
  init();
  make_player_actors();
  make_enemy_actors();
  // add low-priority effects here, because new effects
  // are added to the head of the list
  new_effect(draw_score);
  new_effect(draw_playfield);
  // main loop
  while (actors[PLAYER].shape) {
    WATCHDOG;
    effects_new_frame();
    while (video_counter >= 0x90) effects_next();
    update_screen_section(1);
    while (video_counter < 0x90) effects_next();
    update_screen_section(0);
    switch (frame & 7) {
      case 0:
        break;
    }
    frame++;
  }
  main();
}

// SOUND ROM

const byte __at(0x9000) SOUND_ROM[] = {
0xdb,0x00,0xb7,0x20,0x01,0x76,0x31,0x00,0x80,0xf3,0x11,0x00,0x40,0xdb,0x00,0x4f,0x0d,0x06,0x00,0x69,0x60,0x29,0x09,0x29,0x29,0x01,0x2b,0x01,0x09,0x01,0x0c,0x00,0xed,0xb0,0xcd,0xb5,0x01,0x3e,0x00,0xd3,0x00,0x76,0xc9,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0xc0,0x40,0x00,0x03,0x06,0x09,0x0c,0x10,0x13,0x16,0x19,0x1c,0x1f,0x22,0x25,0x28,0x2b,0x2e,0x31,0x33,0x36,0x39,0x3c,
0x3f,0x41,0x44,0x47,0x49,0x4c,0x4e,0x51,0x53,0x55,0x58,0x5a,0x5c,0x5e,0x60,0x62,0x64,0x66,0x68,0x6a,0x6b,0x6d,0x6f,0x70,0x71,0x73,0x74,0x75,0x76,0x78,0x79,0x7a,0x7a,0x7b,0x7c,0x7d,0x7d,0x7e,0x7e,0x7e,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7e,0x7e,0x7e,0x7d,0x7d,0x7c,0x7b,0x7a,0x7a,0x79,0x78,0x76,0x75,0x74,0x73,0x71,0x70,0x6f,0x6d,0x6b,0x6a,0x68,0x66,0x64,0x62,0x60,0x5e,0x5c,0x5a,0x58,0x55,0x53,0x51,0x4e,0x4c,0x49,0x47,0x44,0x41,0x3f,0x3c,0x39,0x36,0x33,0x31,0x2e,0x2b,0x28,0x25,0x22,0x1f,0x1c,0x19,0x16,0x13,0x10,0x0c,0x09,0x06,0x03,0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8,0x00,0x08,0x10,0x18,0x20,
0x28,0x30,0x38,0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,0x60,0x37,0x3f,0x5c,0x16,0xca,0xc2,0xa5,0xfe,0xba,0x77,0x89,0xaa,0x77,0x13,0xd8,0xae,0x82,0xfd,0x22,0x9c,0x46,0xde,0x14,0x50,0xb4,0x97,0x46,0x54,0x9d,0x60,0x2b,0xb0,0x04,0x00,0x00,0x00,0x00,0x01,0x00,0x0b,0x01,0xff,0x00,0xfa,0x00,0x00,0x00,0xe8,0x03,0xfc,0xff,0x6b,0x00,0x7f,0x00,0xf4,0x01,0x00,0x00,0xe8,0x03,0x02,0x00,0xeb,0x00,0x1f,0x00,0x58,0x02,0x00,0x00,0xb8,0x0b,0xff,0xff,0x2b,0x00,0x1f,0x00,0x58,0x02,0x00,0x00,0xb8,0x0b,0xff,0xff,0x4b,0x00,0x1f,0x00,0x58,0x02,0x00,0x00,0xb8,0x0b,0x01,0x00,0x2b,0x00,0x3f,0x00,0xb0,0x04,0x00,0x00,0x00,0x00,0x01,0x00,0x6b,0x00,0x7f,0x01,0xdd,
0xe5,0xdd,0x21,0x00,0x00,0xdd,0x39,0x3b,0xed,0x5b,0x08,0x40,0xed,0x4b,0x02,0x40,0xdd,0x70,0xff,0x3a,0x0a,0x40,0xdd,0xa6,0xff,0x6f,0x26,0x00,0x19,0x5e,0x3a,0x0c,0x40,0x93,0x38,0x03,0x7b,0xd3,0x00,0x2a,0x04,0x40,0x09,0x4d,0x44,0xed,0x43,0x02,0x40,0x33,0xdd,0xe1,0xc9,0x21,0x0c,0x40,0x36,0xff,0xed,0x4b,0x00,0x40,0x59,0x50,0x1b,0xed,0x53,0x00,0x40,0x78,0xb1,0xc8,0xcd,0x7f,0x01,0xcd,0x7f,0x01,0xcd,0x7f,0x01,0xcd,0x7f,0x01,0xcd,0x7f,0x01,0xcd,0x7f,0x01,0xcd,0x7f,0x01,0xcd,0x7f,0x01,0xed,0x4b,0x04,0x40,0x2a,0x06,0x40,0x09,0x4d,0x44,0xed,0x43,0x04,0x40,0xed,0x4b,0x06,0x40,0x3a,0x0b,0x40,0x6f,0x17,0x9f,0x67,0x09,0x4d,0x44,0xed,0x43,0x06,0x40,
0x2a,0x00,0x40,0x3e,0xff,0xbd,0x3e,0x00,0x9c,0x38,0xaf,0xfd,0x21,0x0c,0x40,0xfd,0x75,0x00,0x18,0xa6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
