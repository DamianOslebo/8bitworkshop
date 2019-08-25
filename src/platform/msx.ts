
import { MSX1 } from "../machine/msx";
import { Platform, BaseZ80MachinePlatform } from "../baseplatform";
import { PLATFORMS } from "../emu";

// https://www.konamiman.com/msx/msx-e.html#msx2th
// https://www.msx.org/wiki/MSX_Cartridge_slot
// http://map.grauw.nl/resources/msx_io_ports.php
// https://openmsx.org/manual/setup.html
// https://www.msx.org/wiki/Slots
// https://www.msx.org/wiki/SDCC
// https://github.com/gseidler/The-MSX-Red-Book/blob/master/the_msx_red_book.md

var MSX_BIOS_PRESETS = [
  {id:'helloworld.asm', name:'Hello World (ASM)'},
  {id:'redbook_kbd.asm', name:'Redbook Keyboard Scanner (ASM)'},
  {id:'siegegame.c',    name:'Siege Game'},
  {id:'eliza.c',        name:'Eliza'},
];

// TODO: share with coleco, sms
var LIBCV_PRESETS = [
  { id: 'text.c', name: 'Text Mode' },
  { id: 'hello.c', name: 'Scrolling Text' },
  { id: 'text32.c', name: '32-Column Color Text' },
  { id: 'stars.c', name: 'Scrolling Starfield' },
  { id: 'cursorsmooth.c', name: 'Moving Cursor' },
  { id: 'simplemusic.c', name: 'Simple Music' },
  { id: 'musicplayer.c', name: 'Multivoice Music' },
  { id: 'mode2bitmap.c', name: 'Mode 2 Bitmap' },
  { id: 'mode2compressed.c', name: 'Mode 2 Bitmap (LZG)' },
  { id: 'lines.c', name: 'Mode 2 Lines' },
  { id: 'multicolor.c', name: 'Multicolor Mode' },
  { id: 'siegegame.c', name: 'Siege Game' },
  { id: 'shoot.c', name: 'Solarian Game' },
  { id: 'climber.c', name: 'Platform Game' },
];

class MSXPlatform extends BaseZ80MachinePlatform<MSX1> implements Platform {

  newMachine()          { return new MSX1(); }
  getPresets()          { return MSX_BIOS_PRESETS; }
  getDefaultExtension() { return ".c"; };
  readAddress(a)        { return this.machine.read(a); }
  // TODO loadBios(bios)	{ this.machine.loadBIOS(a); }

}

class MSXLibCVPlatform extends MSXPlatform implements Platform {
  getPresets() { return LIBCV_PRESETS; }
}

PLATFORMS['msx'] = MSXPlatform;
PLATFORMS['msx-libcv'] = MSXLibCVPlatform;
