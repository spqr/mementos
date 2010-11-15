#include  <msp430x12x2.h>

/***
 * Flash memory is divided up into segments. They are 512 bytes long. If you're going
 * to erase, you have to do it at the segment level. (You can also do a mass erase, which
 * erases multiple segments.)
 *
 * Each segment has eight blocks, each of which is 64 bytes long. It's much more efficient
 * to do your writing at the block level, rather than by individual bytes.
 *
 * To write to flash, you need between 2.7 and 3.7v.
 ***/

// fixme - comments are all directed to reg segment flash, but i'm actually
// wrtiting to info mem - update comments


inline void init_flash() {
  // must set the flash timing generator between 257 and 476 kHz
  // for now, only use flash only when device is in "receive" mode,
  // and the mclk is running at ~ 3mHz.
  FCTL2 = FWKEY + FSSEL1 + FN2 + FN1 + FN0; // use MCLK/8 for flash timing generator
}


inline void erase_segment(char *segment_address) {

  // set segment erase bit
  FCTL1 = FWKEY + ERASE;
  FCTL3 = FWKEY;

  // perform dummy write to initiate erase
  *(segment_address) = 0;

  FCTL1 = FWKEY;
  // set LOCK bit
  FCTL3 = FWKEY + LOCK;
  return;

}

inline void write_byte_to_flash(unsigned char source, char *segment_address, int offset)
{

  FCTL3 = FWKEY;

  // set WRT bits
  FCTL1 = FWKEY + WRT;

  // write element
  *(segment_address + offset) = (char)source;
  //*( (char *)0x1080 ) = (char )0xaa;

  // set LOCK bit
  FCTL1 = FWKEY;
  FCTL3 = FWKEY + LOCK;

  return;

}

inline unsigned char *read_from_flash(char *segment_address, int offset) {

  return (unsigned char *)(segment_address  + offset);

}
