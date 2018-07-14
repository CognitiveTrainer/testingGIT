const byte FONT_GLYPHS[] = {
	// Special symbols inspired by CP437
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x00 NUL
	0x7e, 0x81, 0xa5, 0x81, 0xa5, 0x99, 0x81, 0x7e, // 0x01 :-)
	0x7e, 0x81, 0xa5, 0x81, 0x99, 0xa5, 0x81, 0x7e, // 0x02 :-(
	0x00, 0x36, 0x7f, 0x7f, 0x3e, 0x1c, 0x08, 0x00, // 0x03 heart
	0x08, 0x1c, 0x3e, 0x7f, 0x3e, 0x1c, 0x08, 0x00, // 0x04 diamond
	0x08, 0x1c, 0x2a, 0x7f, 0x2a, 0x08, 0x08, 0x00, // 0x05 club
	0x08, 0x1c, 0x3e, 0x7f, 0x6b, 0x08, 0x3e, 0x00, // 0x06 spade
	0x00, 0x04, 0x0e, 0x1f, 0x0e, 0x04, 0x00, 0x00, // 0x07 bullet

	0x00, 0x04, 0x0a, 0x11, 0x0a, 0x04, 0x00, 0x00, // 0x08 empty bullet
	0x00, 0x18, 0x24, 0x42, 0x42, 0x24, 0x18, 0x00, // 0x09 big bullet
	0xff, 0xe7, 0xdb, 0xbd, 0xbd, 0xdb, 0xe7, 0xff, // 0x0A big bullet inverse
	0xfe, 0x82, 0x82, 0x82, 0x82, 0x82, 0xfe, 0x00, // 0x0B checkbox empty
	0xfe, 0xc6, 0xaa, 0x92, 0xaa, 0xc6, 0xfe, 0x00, // 0x0C checkbox cross
	0xfe, 0xc2, 0xa2, 0x96, 0x8a, 0x82, 0xfe, 0x00, // 0x0D checkbox check
	0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x00, // 0x0E checkbox full
	0x49, 0x3e, 0x22, 0x63, 0x22, 0x3e, 0x49, 0x00, // 0x0F sunny

	0x01, 0x03, 0x07, 0x0f, 0x07, 0x03, 0x01, 0x00, // 0x10 triangle right
	0x08, 0x0c, 0x0e, 0x0f, 0x0e, 0x0c, 0x08, 0x00, // 0x11 triangle left
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x12 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x13
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x14
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x15
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x16
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x17

	0x04, 0x0e, 0x15, 0x04, 0x04, 0x04, 0x04, 0x00, // 0x18 arrow up
	0x04, 0x04, 0x04, 0x04, 0x15, 0x0e, 0x04, 0x00, // 0x19 arrow down
	0x00, 0x10, 0x20, 0x7f, 0x20, 0x10, 0x00, 0x00, // 0x1A arrow right
	0x00, 0x04, 0x02, 0x7f, 0x02, 0x04, 0x00, 0x00, // 0x1B arrow left
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x1C
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x1D
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x1E
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x1F

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // [space]
	0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, // !
	0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, // "
	0x0A, 0x0A, 0x1F, 0x0A, 0x1F, 0x0A, 0x0A, 0x00, // #
	0x0E, 0x15, 0x05, 0x0E, 0x14, 0x15, 0x0E, 0x00, // $
	0x13, 0x13, 0x08, 0x04, 0x02, 0x19, 0x19, 0x00, // %
	0x06, 0x09, 0x05, 0x02, 0x15, 0x09, 0x16, 0x00, // &
	0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, // '

	0x04, 0x02, 0x01, 0x01, 0x01, 0x02, 0x04, 0x00, // (
	0x01, 0x02, 0x04, 0x04, 0x04, 0x02, 0x01, 0x00, // )
	0x00, 0x0A, 0x15, 0x0E, 0x15, 0x0A, 0x00, 0x00, // *
	0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00, 0x00, // +
	0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x01, // ,
	0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, // -
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, // .
	0x10, 0x10, 0x08, 0x04, 0x02, 0x01, 0x01, 0x00, // /

	0x0E, 0x11, 0x19, 0x15, 0x13, 0x11, 0x0E, 0x00, // 0
	0x04, 0x06, 0x04, 0x04, 0x04, 0x04, 0x0E, 0x00, // 1
	0x0E, 0x11, 0x10, 0x0C, 0x02, 0x01, 0x1F, 0x00, // 2
	0x0E, 0x11, 0x10, 0x0C, 0x10, 0x11, 0x0E, 0x00, // 3
	0x08, 0x0C, 0x0A, 0x09, 0x1F, 0x08, 0x08, 0x00, // 4
	0x1F, 0x01, 0x01, 0x0E, 0x10, 0x11, 0x0E, 0x00, // 5
	0x0C, 0x02, 0x01, 0x0F, 0x11, 0x11, 0x0E, 0x00, // 6
	0x1F, 0x10, 0x08, 0x04, 0x02, 0x02, 0x02, 0x00, // 7

	0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E, 0x00, // 8
	0x0E, 0x11, 0x11, 0x1E, 0x10, 0x08, 0x06, 0x00, // 9
	0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, // :
	0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x02, 0x01, // ;
	0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x00, // <
	0x00, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x00, 0x00, // =
	0x01, 0x02, 0x04, 0x08, 0x04, 0x02, 0x01, 0x00, // >
	0x0E, 0x11, 0x10, 0x08, 0x04, 0x00, 0x04, 0x00, // ?

	0x0E, 0x11, 0x1D, 0x15, 0x0D, 0x01, 0x1E, 0x00, // @
	0x04, 0x0A, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x00, // A
	0x0F, 0x11, 0x11, 0x0F, 0x11, 0x11, 0x0F, 0x00, // B
	0x0E, 0x11, 0x01, 0x01, 0x01, 0x11, 0x0E, 0x00, // C
	0x07, 0x09, 0x11, 0x11, 0x11, 0x09, 0x07, 0x00, // D
	0x1F, 0x01, 0x01, 0x0F, 0x01, 0x01, 0x1F, 0x00, // E
	0x1F, 0x01, 0x01, 0x0F, 0x01, 0x01, 0x01, 0x00, // F
	0x0E, 0x11, 0x01, 0x0D, 0x11, 0x19, 0x16, 0x00, // G

	0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11, 0x00, // H
	0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0x07, 0x00, // I
	0x1C, 0x08, 0x08, 0x08, 0x08, 0x09, 0x06, 0x00, // J
	0x11, 0x09, 0x05, 0x03, 0x05, 0x09, 0x11, 0x00, // K
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x0F, 0x00, // L
	0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11, 0x00, // M
	0x11, 0x13, 0x13, 0x15, 0x19, 0x19, 0x11, 0x00, // N
	0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00, // O

	0x0F, 0x11, 0x11, 0x0F, 0x01, 0x01, 0x01, 0x00, // P
	0x0E, 0x11, 0x11, 0x11, 0x15, 0x09, 0x16, 0x00, // Q
	0x0F, 0x11, 0x11, 0x0F, 0x05, 0x09, 0x11, 0x00, // R
	0x0E, 0x11, 0x01, 0x0E, 0x10, 0x11, 0x0E, 0x00, // S
	0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, // T
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00, // U
	0x11, 0x11, 0x11, 0x11, 0x0A, 0x0A, 0x04, 0x00, // V
	0x41, 0x41, 0x41, 0x49, 0x2A, 0x2A, 0x14, 0x00, // W

	0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11, 0x00, // X
	0x11, 0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x00, // Y
	0x1F, 0x10, 0x08, 0x04, 0x02, 0x01, 0x1F, 0x00, // Z
	0x07, 0x01, 0x01, 0x01, 0x01, 0x01, 0x07, 0x00, // [
	0x01, 0x01, 0x02, 0x04, 0x08, 0x10, 0x10, 0x00, // BACKSLASH
	0x07, 0x04, 0x04, 0x04, 0x04, 0x04, 0x07, 0x00, // ]
	0x00, 0x04, 0x0A, 0x11, 0x00, 0x00, 0x00, 0x00, // ^
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, // _

	0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, // `
	0x00, 0x00, 0x06, 0x08, 0x0E, 0x09, 0x0E, 0x00, // a
	0x01, 0x01, 0x0D, 0x13, 0x11, 0x13, 0x0D, 0x00, // b
	0x00, 0x00, 0x06, 0x09, 0x01, 0x09, 0x06, 0x00, // c
	0x10, 0x10, 0x16, 0x19, 0x11, 0x19, 0x16, 0x00, // d
	0x00, 0x00, 0x06, 0x09, 0x07, 0x01, 0x0E, 0x00, // e
	0x04, 0x0A, 0x02, 0x07, 0x02, 0x02, 0x02, 0x00, // f
	0x00, 0x00, 0x06, 0x09, 0x09, 0x06, 0x08, 0x07, // g

	0x01, 0x01, 0x0D, 0x13, 0x11, 0x11, 0x11, 0x00, // h
	0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, // i
	0x04, 0x00, 0x06, 0x04, 0x04, 0x04, 0x04, 0x03, // j
	0x01, 0x01, 0x09, 0x05, 0x03, 0x05, 0x09, 0x00, // k
	0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, // l
	0x00, 0x00, 0x15, 0x2B, 0x29, 0x29, 0x29, 0x00, // m
	0x00, 0x00, 0x0D, 0x13, 0x11, 0x11, 0x11, 0x00, // n
	0x00, 0x00, 0x06, 0x09, 0x09, 0x09, 0x06, 0x00, // o

	0x00, 0x00, 0x0D, 0x13, 0x13, 0x0D, 0x01, 0x01, // p
	0x00, 0x00, 0x16, 0x19, 0x19, 0x16, 0x10, 0x10, // q
	0x00, 0x00, 0x0D, 0x13, 0x01, 0x01, 0x01, 0x00, // r
	0x00, 0x00, 0x0E, 0x01, 0x06, 0x08, 0x07, 0x00, // s
	0x00, 0x02, 0x07, 0x02, 0x02, 0x02, 0x04, 0x00, // t
	0x00, 0x00, 0x11, 0x11, 0x11, 0x19, 0x16, 0x00, // u
	0x00, 0x00, 0x11, 0x11, 0x11, 0x0A, 0x04, 0x00, // v
	0x00, 0x00, 0x11, 0x11, 0x15, 0x15, 0x0A, 0x00, // w

	0x00, 0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x00, // x
	0x00, 0x00, 0x09, 0x09, 0x09, 0x0E, 0x08, 0x06, // y
	0x00, 0x00, 0x0F, 0x08, 0x06, 0x01, 0x0F, 0x00, // z
	0x04, 0x02, 0x02, 0x01, 0x02, 0x02, 0x04, 0x00, // {
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, // |
	0x01, 0x02, 0x02, 0x04, 0x02, 0x02, 0x01, 0x00, // }
	0x00, 0x00, 0x11, 0x2a, 0x44, 0x00, 0x00, 0x00, // ~
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // [DEL] - used as full-width space
};

const byte FONT_LENGTHS[] = {
	0, 8, 8, 7, 7, 7, 7, 5,
	5, 8, 8, 7, 7, 7, 7, 8,
	4, 4, 0, 0, 0, 0, 0, 0,
	5, 5, 7, 7, 0, 0, 0, 0,
	2, 1, 3, 5, 5, 5, 5, 2,							//  !"#$%&'
	3, 3, 5, 5, 2, 5, 1, 5,							// ()*+,-./
	5, 4, 5, 5, 5, 5, 5, 5,							// 01234567
	5, 5, 1, 2, 4, 4, 4, 5,							// 89:;<=>?
	5, 5, 5, 5, 5, 5, 5, 5,							// @ABCDEFG
	5, 3, 5, 5, 4, 5, 5, 5,							// HIJKLMNO
	5, 5, 5, 5, 5, 5, 5, 7,							// PQRSTUVW
	5, 5, 5, 3, 5, 3, 5, 5,							// XYZ[\]^_
	2, 4, 5, 4, 5, 4, 4, 4,							// `abcdefg
	5, 2, 3, 4, 2, 6, 5, 4,							// hijklmno
	5, 5, 5, 4, 3, 5, 5, 5,							// pqrstuvw
	5, 4, 4, 3, 1, 3, 7, 7							// xyz{|}~
};

const byte BIGNUM_CHARS[] = {
	0xFF, 0x81, 0x81, 0xFF,							// 0
	0x82, 0x82, 0xFF, 0x80,							// 1
	0xE3, 0x91, 0x89, 0x87,							// 2
	0xC3, 0x89, 0x89, 0xFF,							// 3
	0x0F, 0x08, 0x08, 0xFF,							// 4
	0xCF, 0x89, 0x89, 0xF9,							// 5
	0xFF, 0x91, 0x91, 0xF3,							// 6
	0x03, 0xE1, 0x19, 0x07,							// 7
	0xFF, 0x89, 0x89, 0xFF,							// 8
	0xCF, 0x89, 0x89, 0xFF,							// 9
};

const byte SMALLNUM_CHARS[] = {
	0x7E, 0x42, 0x7E,								// 0
	0x44, 0x7E, 0x40,								// 1
	0x72, 0x52, 0x5E,								// 2
	0x42, 0x4A, 0x7E,								// 3
	0x0E, 0x08, 0x7E,								// 4
	0x4E, 0x4A, 0x7A,								// 5
	0x7E, 0x52, 0x72,								// 6
	0x06, 0x02, 0x7E,								// 7
	0x7E, 0x52, 0x7E,								// 8
	0x4E, 0x4A, 0x7E,								// 9
};

const byte DOT_CHAR = 0x40;
const byte DOUBLE_DOT_CHAR = 0x24;
const byte SPACE_CHAR = 0x00;