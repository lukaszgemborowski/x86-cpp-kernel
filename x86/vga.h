#ifndef X86_VGA_H
#define X86_VGA_H

namespace x86
{

struct vga
{
	vga() : x_(0), y_(0) {}

	void putat(int x, int y, char chr) const
	{
		constexpr auto width = 80;
		unsigned char *vga = reinterpret_cast<unsigned char *>(0xB8000);

		*(vga + 2*(y * width + x)) = chr;
	}

	void putsat(int x, int y, const char *text) const
	{
		while (*text) {
			putat(x++, y, *text++);
		}
	}

	void put(char chr)
	{
		if (x_ == width) {
			x_ = 0;
			y_ ++;
		}

		putat(x_++, y_, chr);
	}

	void puts(const char *text)
	{
		while (*text) {
			put(*text++);
		}
	}

	void clear()
	{
		for (int x = 0; x < width; x ++) {
			for (int y = 0; y < height; y ++) {
				putat(x, y, ' ');
			}
		}
	}

private:
	static constexpr auto width = 80;
	static constexpr auto height = 25;
	int x_, y_;
};

}

#endif
