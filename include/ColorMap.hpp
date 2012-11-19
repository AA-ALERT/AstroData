/* Color map from http://www.sandia.gov/~kmorel/documents/ColorMaps/ */

#ifndef COLOR_MAP_HPP
#define COLOR_MAP_HPP

namespace AstroData {


class Color {
public:
	Color();
	Color(unsigned char r);
	Color(unsigned char r, unsigned char g);
	Color(unsigned char r, unsigned char g, unsigned char b);

	inline unsigned char getR();
	inline unsigned char getG();
	inline unsigned char getB();

private:
	unsigned char R;
	unsigned char G;
	unsigned char B;
};

// Returns an array of colors
Color *getColorMap();


// Implementation

Color *getColorMap() {
	Color *colorMap = new Color [257];

	colorMap[0] = Color(59, 76, 192);
	colorMap[1] = Color(60, 78, 194);
	colorMap[2] = Color(61, 80, 195);
	colorMap[3] = Color(62, 81, 197);
	colorMap[4] = Color(63, 83, 198);
	colorMap[5] = Color(64, 85, 200);
	colorMap[6] = Color(66, 87, 201);
	colorMap[7] = Color(67, 88, 203);
	colorMap[8] = Color(68, 90, 204);
	colorMap[9] = Color(69, 92, 206);
	colorMap[10] = Color(70, 93, 207);
	colorMap[11] = Color(71, 95, 209);
	colorMap[12] = Color(73, 97, 210);
	colorMap[13] = Color(74, 99, 211);
	colorMap[14] = Color(75, 100, 213);
	colorMap[15] = Color(76, 102, 214);
	colorMap[16] = Color(77, 104, 215);
	colorMap[17] = Color(79, 105, 217);
	colorMap[18] = Color(80, 107, 218);
	colorMap[19] = Color(81, 109, 219);
	colorMap[20] = Color(82, 110, 221);
	colorMap[21] = Color(84, 112, 222);
	colorMap[22] = Color(85, 114, 223);
	colorMap[23] = Color(86, 115, 224);
	colorMap[24] = Color(87, 117, 225);
	colorMap[25] = Color(89, 119, 226);
	colorMap[26] = Color(90, 120, 228);
	colorMap[27] = Color(91, 122, 229);
	colorMap[28] = Color(93, 123, 230);
	colorMap[29] = Color(94, 125, 231);
	colorMap[30] = Color(95, 127, 232);
	colorMap[31] = Color(96, 128, 233);
	colorMap[32] = Color(98, 130, 234);
	colorMap[33] = Color(99, 131, 235);
	colorMap[34] = Color(100, 133, 236);
	colorMap[35] = Color(102, 135, 237);
	colorMap[36] = Color(103, 136, 238);
	colorMap[37] = Color(104, 138, 239);
	colorMap[38] = Color(106, 139, 239);
	colorMap[39] = Color(107, 141, 240);
	colorMap[40] = Color(108, 142, 241);
	colorMap[41] = Color(110, 144, 242);
	colorMap[42] = Color(111, 145, 243);
	colorMap[43] = Color(112, 147, 243);
	colorMap[44] = Color(114, 148, 244);
	colorMap[45] = Color(115, 150, 245);
	colorMap[46] = Color(116, 151, 246);
	colorMap[47] = Color(118, 153, 246);
	colorMap[48] = Color(119, 154, 247);
	colorMap[49] = Color(120, 156, 247);
	colorMap[50] = Color(122, 157, 248);
	colorMap[51] = Color(123, 158, 249);
	colorMap[52] = Color(124, 160, 249);
	colorMap[53] = Color(126, 161, 250);
	colorMap[54] = Color(127, 163, 250);
	colorMap[55] = Color(129, 164, 251);
	colorMap[56] = Color(130, 165, 251);
	colorMap[57] = Color(131, 167, 252);
	colorMap[58] = Color(133, 168, 252);
	colorMap[59] = Color(134, 169, 252);
	colorMap[60] = Color(135, 171, 253);
	colorMap[61] = Color(137, 172, 253);
	colorMap[62] = Color(138, 173, 253);
	colorMap[63] = Color(140, 174, 254);
	colorMap[64] = Color(141, 176, 254);
	colorMap[65] = Color(142, 177, 254);
	colorMap[66] = Color(144, 178, 254);
	colorMap[67] = Color(145, 179, 254);
	colorMap[68] = Color(147, 181, 255);
	colorMap[69] = Color(148, 182, 255);
	colorMap[70] = Color(149, 183, 255);
	colorMap[71] = Color(151, 184, 255);
	colorMap[72] = Color(152, 185, 255);
	colorMap[73] = Color(153, 186, 255);
	colorMap[74] = Color(155, 187, 255);
	colorMap[75] = Color(156, 188, 255);
	colorMap[76] = Color(158, 190, 255);
	colorMap[77] = Color(159, 191, 255);
	colorMap[78] = Color(160, 192, 255);
	colorMap[79] = Color(162, 193, 255);
	colorMap[80] = Color(163, 194, 255);
	colorMap[81] = Color(164, 195, 254);
	colorMap[82] = Color(166, 196, 254);
	colorMap[83] = Color(167, 197, 254);
	colorMap[84] = Color(168, 198, 254);
	colorMap[85] = Color(170, 199, 253);
	colorMap[86] = Color(171, 199, 253);
	colorMap[87] = Color(172, 200, 253);
	colorMap[88] = Color(174, 201, 253);
	colorMap[89] = Color(175, 202, 252);
	colorMap[90] = Color(176, 203, 252);
	colorMap[91] = Color(178, 204, 251);
	colorMap[92] = Color(179, 205, 251);
	colorMap[93] = Color(180, 205, 251);
	colorMap[94] = Color(182, 206, 250);
	colorMap[95] = Color(183, 207, 250);
	colorMap[96] = Color(184, 208, 249);
	colorMap[97] = Color(185, 208, 248);
	colorMap[98] = Color(187, 209, 248);
	colorMap[99] = Color(188, 210, 247);
	colorMap[100] = Color(189, 210, 247);
	colorMap[101] = Color(190, 211, 246);
	colorMap[102] = Color(192, 212, 245);
	colorMap[103] = Color(193, 212, 245);
	colorMap[104] = Color(194, 213, 244);
	colorMap[105] = Color(195, 213, 243);
	colorMap[106] = Color(197, 214, 243);
	colorMap[107] = Color(198, 214, 242);
	colorMap[108] = Color(199, 215, 241);
	colorMap[109] = Color(200, 215, 240);
	colorMap[110] = Color(201, 216, 239);
	colorMap[111] = Color(203, 216, 238);
	colorMap[112] = Color(204, 217, 238);
	colorMap[113] = Color(205, 217, 237);
	colorMap[114] = Color(206, 217, 236);
	colorMap[115] = Color(207, 218, 235);
	colorMap[116] = Color(208, 218, 234);
	colorMap[117] = Color(209, 219, 233);
	colorMap[118] = Color(210, 219, 232);
	colorMap[119] = Color(211, 219, 231);
	colorMap[120] = Color(213, 219, 230);
	colorMap[121] = Color(214, 220, 229);
	colorMap[122] = Color(215, 220, 228);
	colorMap[123] = Color(216, 220, 227);
	colorMap[124] = Color(217, 220, 225);
	colorMap[125] = Color(218, 220, 224);
	colorMap[126] = Color(219, 220, 223);
	colorMap[127] = Color(220, 221, 222);
	colorMap[128] = Color(221, 221, 221);
	colorMap[129] = Color(222, 220, 219);
	colorMap[130] = Color(223, 220, 218);
	colorMap[131] = Color(224, 219, 216);
	colorMap[132] = Color(225, 219, 215);
	colorMap[133] = Color(226, 218, 214);
	colorMap[134] = Color(227, 218, 212);
	colorMap[135] = Color(228, 217, 211);
	colorMap[136] = Color(229, 216, 209);
	colorMap[137] = Color(230, 216, 208);
	colorMap[138] = Color(231, 215, 206);
	colorMap[139] = Color(232, 215, 205);
	colorMap[140] = Color(232, 214, 203);
	colorMap[141] = Color(233, 213, 202);
	colorMap[142] = Color(234, 212, 200);
	colorMap[143] = Color(235, 212, 199);
	colorMap[144] = Color(236, 211, 197);
	colorMap[145] = Color(236, 210, 196);
	colorMap[146] = Color(237, 209, 194);
	colorMap[147] = Color(238, 209, 193);
	colorMap[148] = Color(238, 208, 191);
	colorMap[149] = Color(239, 207, 190);
	colorMap[150] = Color(240, 206, 188);
	colorMap[151] = Color(240, 205, 187);
	colorMap[152] = Color(241, 204, 185);
	colorMap[153] = Color(241, 203, 184);
	colorMap[154] = Color(242, 202, 182);
	colorMap[155] = Color(242, 201, 181);
	colorMap[156] = Color(243, 200, 179);
	colorMap[157] = Color(243, 199, 178);
	colorMap[158] = Color(244, 198, 176);
	colorMap[159] = Color(244, 197, 174);
	colorMap[160] = Color(245, 196, 173);
	colorMap[161] = Color(245, 195, 171);
	colorMap[162] = Color(245, 194, 170);
	colorMap[163] = Color(245, 193, 168);
	colorMap[164] = Color(246, 192, 167);
	colorMap[165] = Color(246, 191, 165);
	colorMap[166] = Color(246, 190, 163);
	colorMap[167] = Color(246, 188, 162);
	colorMap[168] = Color(247, 187, 160);
	colorMap[169] = Color(247, 186, 159);
	colorMap[170] = Color(247, 185, 157);
	colorMap[171] = Color(247, 184, 156);
	colorMap[172] = Color(247, 182, 154);
	colorMap[173] = Color(247, 181, 152);
	colorMap[174] = Color(247, 180, 151);
	colorMap[175] = Color(247, 178, 149);
	colorMap[176] = Color(247, 177, 148);
	colorMap[177] = Color(247, 176, 146);
	colorMap[178] = Color(247, 174, 145);
	colorMap[179] = Color(247, 173, 143);
	colorMap[180] = Color(247, 172, 141);
	colorMap[181] = Color(247, 170, 140);
	colorMap[182] = Color(247, 169, 138);
	colorMap[183] = Color(247, 167, 137);
	colorMap[184] = Color(247, 166, 135);
	colorMap[185] = Color(246, 164, 134);
	colorMap[186] = Color(246, 163, 132);
	colorMap[187] = Color(246, 161, 131);
	colorMap[188] = Color(246, 160, 129);
	colorMap[189] = Color(245, 158, 127);
	colorMap[190] = Color(245, 157, 126);
	colorMap[191] = Color(245, 155, 124);
	colorMap[192] = Color(244, 154, 123);
	colorMap[193] = Color(244, 152, 121);
	colorMap[194] = Color(244, 151, 120);
	colorMap[195] = Color(243, 149, 118);
	colorMap[196] = Color(243, 147, 117);
	colorMap[197] = Color(242, 146, 115);
	colorMap[198] = Color(242, 144, 114);
	colorMap[199] = Color(241, 142, 112);
	colorMap[200] = Color(241, 141, 111);
	colorMap[201] = Color(240, 139, 109);
	colorMap[202] = Color(240, 137, 108);
	colorMap[203] = Color(239, 136, 106);
	colorMap[204] = Color(238, 134, 105);
	colorMap[205] = Color(238, 132, 103);
	colorMap[206] = Color(237, 130, 102);
	colorMap[207] = Color(236, 129, 100);
	colorMap[208] = Color(236, 127, 99);
	colorMap[209] = Color(235, 125, 97);
	colorMap[210] = Color(234, 123, 96);
	colorMap[211] = Color(233, 121, 95);
	colorMap[212] = Color(233, 120, 93);
	colorMap[213] = Color(232, 118, 92);
	colorMap[214] = Color(231, 116, 90);
	colorMap[215] = Color(230, 114, 89);
	colorMap[216] = Color(229, 112, 88);
	colorMap[217] = Color(228, 110, 86);
	colorMap[218] = Color(227, 108, 85);
	colorMap[219] = Color(227, 106, 83);
	colorMap[220] = Color(226, 104, 82);
	colorMap[221] = Color(225, 102, 81);
	colorMap[222] = Color(224, 100, 79);
	colorMap[223] = Color(223, 98, 78);
	colorMap[224] = Color(222, 96, 77);
	colorMap[225] = Color(221, 94, 75);
	colorMap[226] = Color(220, 92, 74);
	colorMap[227] = Color(218, 90, 73);
	colorMap[228] = Color(217, 88, 71);
	colorMap[229] = Color(216, 86, 70);
	colorMap[230] = Color(215, 84, 69);
	colorMap[231] = Color(214, 82, 67);
	colorMap[232] = Color(213, 80, 66);
	colorMap[233] = Color(212, 78, 65);
	colorMap[234] = Color(210, 75, 64);
	colorMap[235] = Color(209, 73, 62);
	colorMap[236] = Color(208, 71, 61);
	colorMap[237] = Color(207, 69, 60);
	colorMap[238] = Color(205, 66, 59);
	colorMap[239] = Color(204, 64, 57);
	colorMap[240] = Color(203, 62, 56);
	colorMap[241] = Color(202, 59, 55);
	colorMap[242] = Color(200, 57, 54);
	colorMap[243] = Color(199, 54, 53);
	colorMap[244] = Color(198, 51, 52);
	colorMap[245] = Color(196, 49, 50);
	colorMap[246] = Color(195, 46, 49);
	colorMap[247] = Color(193, 43, 48);
	colorMap[248] = Color(192, 40, 47);
	colorMap[249] = Color(190, 37, 46);
	colorMap[250] = Color(189, 34, 45);
	colorMap[251] = Color(188, 30, 44);
	colorMap[252] = Color(186, 26, 43);
	colorMap[253] = Color(185, 22, 41);
	colorMap[254] = Color(183, 17, 40);
	colorMap[255] = Color(181, 11, 39);
	colorMap[256] = Color(180, 4, 38);

	return colorMap;
}

Color::Color() : R(0), G(0), B(0) {}

Color::Color(unsigned char r) : R(r), G(0), B(0) {}

Color::Color(unsigned char r, unsigned char g) : R(r), G(g), B(0) {}

Color::Color(unsigned char r, unsigned char g, unsigned char b) : R(r), G(g), B(b) {}

inline unsigned char Color::getR() {
	return R;
}

inline unsigned char Color::getG() {
	return G;
}

inline unsigned char Color::getB() {
	return B;
}

} // AstroData

#endif // COLOR_MAP_HPP
