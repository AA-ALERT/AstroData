// Copyright 2012 Alessio Sclocco <a.sclocco@vu.nl>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
Color * getColorMap();

} // AstroData

#endif // COLOR_MAP_HPP

