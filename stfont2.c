/* --------------------------------- stfont2.c ------------------------------ */

/* This is part of the flight simulator 'fly8'.
 * Author: Eyal Lebedinsky (eyal@eyal.emu.id.au).
*/

/* Text in graphics window - stroke font. Fine resolution, round.
*/

#include "fly.h"


/* Each command is one byte optionaly followed by one byte operand. First byte
 * is size.
 *	\x00 draw	xy
 *	\x01 move	xy
 *	\x02 end
 * The table is strictly ASCII.
*/

char NEAR* NEAR StFont2[] = {
/*00*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*10*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*20*/
  /* */	"\x60\x02",
  /*!*/	"\x40\x01\x27\x00\x23\x01\x21\x00\x21\x02",
  /*"*/	"\x60\x01\x17\x00\x15\x01\x37\x00\x35\x02",
  /*#*/	"\x60\x01\x17\x00\x11\x01\x37\x00\x31\x01\x05\x00\x45\x01\x03\x00\x43\x02",
  /*$*/	"\x60\x01\x27\x00\x21\x01\x02\x00\x32\x00\x43\x00\x34\x00\x14\x00\x05\x00\x16\x00\x46\x02",
  /*%*/	"\x60\x01\x02\x00\x46\x01\x06\x00\x07\x00\x17\x00\x16\x00\x06\x01\x31\x00\x32\x00\x42\x00\x41\x00\x31\x02",
  /*&*/	"\x60\x01\x41\x00\x05\x00\x06\x00\x17\x00\x26\x00\x25\x00\x03\x00\x02\x00\x11\x00\x21\x00\x43\x02",
  /*'*/	"\x40\x01\x17\x00\x15\x02",
  /*(*/	"\x40\x01\x27\x00\x05\x00\x03\x00\x21\x02",
  /*)*/	"\x40\x01\x07\x00\x25\x00\x23\x00\x01\x02",
  /***/	"\x60\x01\x02\x00\x46\x01\x06\x00\x42\x01\x04\x00\x44\x01\x21\x00\x27\x02",
  /*+*/	"\x60\x01\x14\x00\x54\x01\x32\x00\x36\x02",
  /*,*/	"\x20\x01\x00\x00\x11\x00\x12\x02",
  /*-*/	"\x60\x01\x14\x00\x44\x02",
  /*.*/	"\x20\x01\x01\x00\x02\x00\x12\x00\x11\x00\x01\x02",
  /* / */	"\x60\x01\x01\x00\x56\x02",
/*30*/
  /*0*/	"\x60\x01\x31\x00\x11\x00\x02\x00\x06\x00\x17\x00\x37\x00\x46\x00\x42\x00\x31\x02",
  /*1*/	"\x60\x01\x15\x00\x37\x00\x31\x02",
  /*2*/	"\x60\x01\x06\x00\x17\x00\x37\x00\x46\x00\x45\x00\x34\x00\x24\x00\x02\x00\x01\x00\x41\x02",
  /*3*/	"\x60\x01\x07\x00\x47\x00\x46\x00\x24\x00\x34\x00\x43\x00\x42\x00\x31\x00\x11\x00\x02\x02",
  /*4*/	"\x60\x01\x31\x00\x37\x00\x04\x00\x03\x00\x43\x02",
  /*5*/	"\x60\x01\x02\x00\x11\x00\x31\x00\x42\x00\x44\x00\x35\x00\x05\x00\x07\x00\x47\x02",
  /*6*/	"\x60\x01\x47\x00\x27\x00\x05\x00\x02\x00\x11\x00\x31\x00\x42\x00\x43\x00\x34\x00\x04\x02",
  /*7*/	"\x60\x01\x07\x00\x47\x00\x46\x00\x13\x00\x11\x02",
  /*8*/	"\x60\x01\x14\x00\x03\x00\x02\x00\x11\x00\x31\x00\x42\x00\x43\x00\x34\x00\x14\x00\x05\x00\x06\x00\x17\x00\x37\x00\x46\x00\x45\x00\x34\x02",
  /*9*/	"\x60\x01\x01\x00\x21\x00\x43\x00\x46\x00\x37\x00\x17\x00\x06\x00\x05\x00\x14\x00\x44\x02",
  /*:*/	"\x20\x01\x02\x00\x03\x00\x13\x00\x12\x00\x02\x01\x05\x00\x06\x00\x16\x00\x15\x00\x05\x02",
  /*;*/	"\x40\x01\x01\x00\x23\x01\x15\x00\x16\x00\x26\x00\x25\x00\x15\x02",
  /*<*/	"\x60\x01\x31\x00\x04\x00\x37\x02",
  /*=*/	"\x60\x01\x15\x00\x45\x01\x13\x00\x43\x02",
  /*>*/	"\x60\x01\x01\x00\x34\x00\x07\x02",
  /*?*/	"\x60\x01\x06\x00\x17\x00\x37\x00\x46\x00\x24\x00\x23\x01\x21\x00\x21\x02",
/*40*/
  /*@*/	"\x60\x01\x41\x00\x11\x00\x02\x00\x06\x00\x17\x00\x37\x00\x46\x00\x43\x00\x23\x00\x25\x02",
  /*A*/	"\x60\x01\x01\x00\x05\x00\x27\x00\x45\x00\x41\x01\x03\x00\x43\x02",
  /*B*/	"\x60\x01\x34\x00\x43\x00\x42\x00\x31\x00\x01\x00\x07\x00\x37\x00\x46\x00\x45\x00\x34\x00\x04\x02",
  /*C*/	"\x60\x01\x42\x00\x31\x00\x11\x00\x02\x00\x06\x00\x17\x00\x37\x00\x46\x02",
  /*D*/	"\x60\x01\x01\x00\x07\x00\x37\x00\x46\x00\x42\x00\x31\x00\x01\x02",
  /*E*/	"\x60\x01\x41\x00\x01\x00\x07\x00\x47\x01\x34\x00\x04\x02",
  /*F*/	"\x60\x01\x01\x00\x07\x00\x47\x01\x34\x00\x04\x02",
  /*G*/	"\x60\x01\x47\x00\x17\x00\x06\x00\x02\x00\x11\x00\x41\x00\x43\x00\x33\x02",
  /*H*/	"\x60\x01\x01\x00\x07\x01\x41\x00\x47\x01\x04\x00\x44\x02",
  /*I*/	"\x60\x01\x21\x00\x27\x01\x11\x00\x31\x01\x17\x00\x37\x02",
  /*J*/	"\x60\x01\x47\x00\x42\x00\x31\x00\x11\x00\x02\x02",
  /*K*/	"\x60\x01\x07\x00\x01\x01\x47\x00\x14\x00\x41\x02",
  /*L*/	"\x60\x01\x07\x00\x01\x00\x41\x02",
  /*M*/	"\x60\x01\x01\x00\x07\x00\x25\x00\x47\x00\x41\x02",
  /*N*/	"\x60\x01\x01\x00\x07\x01\x41\x00\x47\x01\x06\x00\x42\x02",
  /*O*/	"\x60\x01\x01\x00\x41\x00\x47\x00\x07\x00\x01\x02",
/*50*/
  /*P*/	"\x60\x01\x01\x00\x07\x00\x37\x00\x46\x00\x45\x00\x34\x00\x04\x02",
  /*Q*/	"\x60\x01\x21\x00\x11\x00\x02\x00\x06\x00\x17\x00\x37\x00\x46\x00\x43\x00\x21\x01\x23\x00\x41\x02",
  /*R*/	"\x60\x01\x01\x00\x07\x00\x37\x00\x46\x00\x45\x00\x34\x00\x04\x01\x14\x00\x41\x02",
  /*S*/	"\x60\x01\x46\x00\x37\x00\x17\x00\x06\x00\x05\x00\x14\x00\x34\x00\x43\x00\x42\x00\x31\x00\x11\x00\x02\x02",
  /*T*/	"\x60\x01\x21\x00\x27\x01\x07\x00\x47\x02",
  /*U*/	"\x60\x01\x07\x00\x02\x00\x11\x00\x31\x00\x42\x00\x47\x02",
  /*V*/	"\x60\x01\x07\x00\x03\x00\x21\x00\x43\x00\x47\x02",
  /*W*/	"\x60\x01\x07\x00\x01\x00\x23\x00\x41\x00\x47\x02",
  /*X*/	"\x60\x01\x01\x00\x02\x00\x46\x00\x47\x01\x41\x00\x42\x00\x06\x00\x07\x02",
  /*Y*/	"\x60\x01\x07\x00\x06\x00\x24\x00\x21\x01\x47\x00\x46\x00\x24\x02",
  /*Z*/	"\x60\x01\x07\x00\x47\x00\x46\x00\x02\x00\x01\x00\x41\x02",
  /*[*/	"\x40\x01\x27\x00\x07\x00\x01\x00\x21\x02",
  /*\\*/"\x60\x01\x06\x00\x51\x02",
  /*]*/	"\x40\x01\x07\x00\x27\x00\x21\x00\x01\x02",
  /*^*/	"\x60\x01\x05\x00\x27\x00\x45\x02",
  /*_*/	"\x60\x01\x00\x00\x40\x02",
/*60*/
  /*`*/	"\x40\x01\x07\x00\x25\x02",
  /*a*/	"\x60\x01\x45\x00\x41\x01\x43\x00\x25\x00\x15\x00\x04\x00\x02\x00\x11\x00\x21\x00\x43\x02",
  /*b*/	"\x60\x01\x07\x00\x01\x01\x03\x00\x25\x00\x35\x00\x44\x00\x42\x00\x31\x00\x21\x00\x03\x02",
  /*c*/	"\x60\x01\x41\x00\x11\x00\x02\x00\x04\x00\x15\x00\x45\x02",
  /*d*/	"\x60\x01\x47\x00\x41\x01\x43\x00\x25\x00\x15\x00\x04\x00\x02\x00\x11\x00\x21\x00\x43\x02",
  /*e*/	"\x60\x01\x31\x00\x11\x00\x02\x00\x04\x00\x15\x00\x35\x00\x44\x00\x43\x00\x03\x02",
  /*f*/	"\x60\x01\x21\x00\x26\x00\x37\x01\x14\x00\x34\x02",
  /*g*/	"\x60\x01\x10\x00\x30\x00\x41\x00\x46\x01\x44\x00\x26\x00\x16\x00\x05\x00\x03\x00\x12\x00\x22\x00\x44\x02",
  /*h*/	"\x60\x01\x01\x00\x07\x01\05\x00\x35\x00\x44\x00\x41\x02",
  /*i*/	"\x60\x01\x11\x00\x31\x01\x21\x00\x24\x00\x14\x01\x26\x00\x26\x02",
  /*j*/	"\x60\x01\x10\x00\x20\x00\x31\x00\x34\x01\x36\x00\x36\x02",
  /*k*/	"\x60\x01\x07\x00\x01\x00\x45\x01\x23\x00\x41\x02",
  /*l*/	"\x60\x01\x17\x00\x27\x00\x21\x01\x11\x00\x31\x02",
  /*m*/	"\x60\x01\x01\x00\x05\x00\x15\x00\x24\x00\x21\x01\x24\x00\x35\x00\x44\x00\x41\x02",
  /*n*/	"\x60\x01\x01\x00\x05\x00\x35\x00\x44\x00\x41\x02",
  /*o*/	"\x60\x01\x11\x00\x31\x00\x42\x00\x44\x00\x35\x00\x15\x00\x04\x00\x02\x00\x11\x02",
/*70*/
  /*p*/	"\x60\x01\x06\x00\x00\x01\x04\x00\x26\x00\x36\x00\x45\x00\x43\x00\x32\x00\x22\x00\x04\x02",
  /*q*/	"\x60\x01\x40\x00\x46\x01\x44\x00\x26\x00\x16\x00\x05\x00\x03\x00\x12\x00\x22\x00\x44\x02",
  /*r*/	"\x60\x01\x01\x00\x05\x01\x03\x00\x25\x00\x45\x02",
  /*s*/	"\x60\x01\x01\x00\x31\x00\x42\x00\x33\x00\x13\x00\x04\x00\x15\x00\x45\x02",
  /*t*/	"\x60\x01\x26\x00\x21\x01\x05\x00\x45\x02",
  /*u*/	"\x60\x01\x05\x00\x02\x00\x11\x00\x41\x00\x45\x02",
  /*v*/	"\x60\x01\x05\x00\x03\x00\x21\x00\x43\x00\x45\x02",
  /*w*/	"\x60\x01\x05\x00\x02\x00\x11\x00\x22\x00\x31\x00\x42\x00\x45\x02",
  /*x*/	"\x60\x01\x01\x00\x45\x01\x41\x00\x05\x02",
  /*y*/	"\x60\x01\x15\x00\x12\x00\x42\x01\x45\x00\x41\x00\x30\x00\x10\x02",
  /*z*/	"\x60\x01\x05\x00\x45\x00\x01\x00\x41\x02",
  /*{*/	"\x60\x01\x47\x00\x37\x00\x26\x00\x25\x00\x14\x00\x23\x00\x22\x00\x31\x00\x41\x02",
  /*|*/	"\x40\x01\x17\x00\x11\x02",
  /*}*/	"\x60\x01\x07\x00\x17\x00\x26\x00\x25\x00\x34\x00\x23\x00\x22\x00\x11\x00\x01\x02",
  /*~*/	"\x60\x01\x04\x00\x15\x00\x33\x00\x44\x02",
  /* */	0,
/*80*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*90*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*a0*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*b0*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*c0*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*d0*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*e0*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*f0*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*xx*/	"\x60\x01\x00\x00\x06\x00\x46\x00\x40\x00\x00\x01\x01\x00\x45\x01\x05\x00\x41\x02"
};
