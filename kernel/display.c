#include "display.h"
#include "asm.h"
#include <stdint.h>


#define WIDTH	80
#define HEIGHT	25

#define COLUMN_MAX	(WIDTH - 1)
#define ROW_MAX		(HEIGHT - 1)
#define CS_INDEX	(WIDTH * row + column)


static int column;
static int row;
static uint16_t *vram;


static void cursor(int c, int r)
{
	if (c < 0 || c > COLUMN_MAX ||
		r < 0 || r > ROW_MAX)
		return;

	column = c;
	row = r;

	outb(0x0E, 0x03D4);
	outb(CS_INDEX >> 8, 0x03D5);
	outb(0x0F, 0x03D4);
	outb(CS_INDEX, 0x03D5);
}

static void cursor_right()
{
	cursor(column + 1, row);
}

static void cursor_left()
{
	cursor(column - 1, row);
}

static void cursor_up()
{
	cursor(column, row - 1);
}

static void clear_char(int c, int r)
{
	vram[WIDTH * r + c] &= 0xFF00;
}

static void clear_line(const int line)
{
	for (int i = 0; i < WIDTH; ++i)
		clear_char(i, line);
}

static void clear_display()
{
	for (int i = 0; i < HEIGHT; ++i)
		clear_line(i);

	cursor(0, 0);
}

static void scroll_up()
{
	for (int i = 0; i < HEIGHT; ++i)
		for (int j = 0; j < WIDTH; ++j)
			vram[WIDTH * i + j] = vram[WIDTH * (i + 1) + j];

	if (row)
		cursor_up();
}

static void backspace()
{
	if (!row && !column)
		return;

	if (column)
		cursor_left();
	else
		cursor(COLUMN_MAX, row - 1);

	if (vram[CS_INDEX] & 0xFF) {
		clear_char(column, row);
	} else {
		while (!(vram[CS_INDEX - 1] & 0xFF) && column)
			cursor_left();
	}
}

static void line_feed()
{
	if (row == ROW_MAX)
		scroll_up();

	cursor(0, row + 1);
}

static void horiz_tab()
{
	int tab_width = 8;
	int plus = tab_width - column % tab_width;

	if (column >= COLUMN_MAX - 8)
		line_feed();
	else
		cursor(column + plus, row);
}

void write_display(const int ch)
{
	static uint8_t color = 0x0F;

	switch (ch) {
	case '\b':
		backspace();
		break;
	case '\t':
		horiz_tab();
		break;
	case '\n':
		line_feed();
		break;
	case 0x20 ... 0x7e:
		vram[CS_INDEX] = (color << 8) | (uint8_t)ch;
		if (column == COLUMN_MAX)
			line_feed();
		else
			cursor_right();
		break;
	default:
		break;
	}
}

void init_display()
{
	vram = (uint16_t *)0xB8000;
	clear_display();
}
