/*
static bool is_blk_nr_set(uint16_t blk_nr)
{
	int idx, shift;

	idx = (blk_nr - 1) / 8;
	shift = (blk_nr - 1) % 8;

	return bitmaps.blk_bitmap[idx] >> shift & 1;
}

static void set_blk_bitmap(uint16_t blk_nr)
{
	int idx, shift;

	idx = (blk_nr - 1) / 8;
	shift = (blk_nr - 1) % 8;

    bitmaps.blk_bitmap[idx] |= 1 << shift;
}

static uint16_t alloc_blk_nr()
{
	uint16_t blk_nr;

	for (blk_nr = 1; blk_nr < 1855; blk_nr++) {
		if (!is_blk_nr_set(blk_nr)) {
			set_blk_bitmap(blk_nr);
			break;
		}
	}

	return blk_nr;
}

static void write_block(void *blk, uint16_t blk_nr)
{
	int lba;

	lba = blk_nr - 1 + 1026;

	write_sect(lba, blk);
}
*/
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "stdio.h"
#include "string.h"
#include "fs.h"
#include "fdd.h"
#include "asm.h"


#define abs(x) (x < 0 ? -x : x)


struct inode {
	union {
		int16_t in_nrs[128]; // in a case of directory. if it is negative, inode number of directory
		uint16_t blk_nrs[128]; // in a case of file
	} nrs;
	char name[256];
} __attribute__((packed)); // 512 byte


char work_dir[256];

static struct {
	uint8_t in_bitmap[256];
	uint8_t blk_bitmap[256];
} bitmaps;


static bool is_in_nr_set(int16_t in_nr)
{
	int idx, shift;

	idx = (abs(in_nr) - 1) / 8;
	shift = (abs(in_nr) - 1) % 8;

	return bitmaps.in_bitmap[idx] >> shift & 1;
}

static void set_in_bitmap(int16_t in_nr)
{
	int idx, shift;

	idx = (abs(in_nr) - 1) / 8;
	shift = (abs(in_nr) - 1) % 8;

    bitmaps.in_bitmap[idx] |= 1 << shift;
}

static void clear_in_bitmap(int16_t in_nr)
{
	int idx, shift;

	idx = (abs(in_nr) - 1) / 8;
	shift = (abs(in_nr) - 1) % 8;

    bitmaps.in_bitmap[idx] &= ~(1 << shift);
}

static int16_t alloc_in_nr()
{
	int16_t in_nr;

	for (in_nr = 2; in_nr < 1025; in_nr++) {
		if (!is_in_nr_set(in_nr)) {
			set_in_bitmap(in_nr);
			break;
		}
	}

	return in_nr;
}

static void __read_inode(struct inode *in, int16_t in_nr)
{
	int lba;

	lba = abs(in_nr) - 1 + 2;

	read_sect(lba, in);
}

static int16_t read_inode(struct inode *in, const char *path)
{
	const char *name;
	char path2[strlen(path) + 1];
	int16_t in_nr;
	struct inode tmp;

	strcpy(path2, path);
	name = strtok(path2, "/");

	in_nr = -1;
	__read_inode(in, in_nr);

	while (name != NULL) {
		for (int i = 0; in->nrs.in_nrs[i]; i++) {
			__read_inode(&tmp, in->nrs.in_nrs[i]);

			if (strcmp(tmp.name, name) == 0) {
				in_nr = in->nrs.in_nrs[i];
				*in = tmp;
				break;
			}
		}

		name = strtok(NULL, "/");
	}

	return in_nr;
}

static void write_inode(struct inode *in, int16_t in_nr)
{
	int lba;

	lba = abs(in_nr) - 1 + 2;

	write_sect(lba, in);
}

static void read_block(void *blk, uint16_t blk_nr)
{
	int lba;

	lba = blk_nr - 1 + 1026;

	read_sect(lba, blk);
}

void load_file(const char *path, uint32_t vaddr)
{
	struct inode in;
	uint8_t blk[512];

	read_inode(&in, path);

	for (int i = 0; in.nrs.blk_nrs[i]; ++i) {
		read_block(blk, in.nrs.blk_nrs[i]);
		memcpy((void *)vaddr, blk, 512);
		vaddr += 512;
	}
}

bool is_directory(const char *path)
{
	int16_t in_nr;
	struct inode in;
	const char *name;

	name = NULL;
	in_nr = read_inode(&in, path);

	if (in_nr == -1)
		return true;

	for (int i = strlen(path) - 1; path[i]; i--) {
		if (path[i] == '/')
			name = &path[i] + 1;
	}

	if (strcmp(in.name, name) == 0 && in_nr < 0)
		return true;
	else
		return false;
}

void make_dir(const char *path)
{
	struct inode par_in, in = {0};
	int16_t par_in_nr;
	char par_path[strlen(path) + 1];

	if (!is_directory(path))
		return;

	for (int i = strlen(path) - 1; path[i]; i--) {
		if (path[i - 1] == '/') {
			strcpy(in.name, &path[i]);
			break;
		}
	}

	strcpy(par_path, path);
	for (int i = strlen(par_path) - 1; par_path[i]; i--) {
		par_path[i] = '\0';

		if (par_path[i - 1] == '/') {
			if (i - 1)
				par_path[i - 1] = '\0';
			break;
		}
	}

	par_in_nr = read_inode(&par_in, par_path);

	for (int i = 0; i < 128; i++) {
		if (!par_in.nrs.in_nrs[i]) {
			par_in.nrs.in_nrs[i] = -alloc_in_nr();
			write_inode(&in, par_in.nrs.in_nrs[i]);
			break;
		}
	}

	write_inode(&par_in, par_in_nr);
}

void remove_dir(const char *path)
{
	struct inode par_in, in;
	int16_t par_in_nr, in_nr;
	char par_path[strlen(path) + 1];

	if (!is_directory(path))
		return;

	strcpy(par_path, path);
	for (int i = strlen(par_path) - 1; par_path[i]; i--) {
		par_path[i] = '\0';

		if (par_path[i - 1] == '/') {
			if (i - 1)
				par_path[i - 1] = '\0';
			break;
		}
	}

	par_in_nr = read_inode(&par_in, par_path);
	in_nr = read_inode(&in, path);

	for (int i = 0; i < 128; i++) {
		if (par_in.nrs.in_nrs[i] == in_nr) {
			par_in.nrs.in_nrs[i] = 0;
			break;
		}
	}

	write_inode(&par_in, par_in_nr);
	memset(&in, 0, sizeof(in));
	write_inode(&in, in_nr);
	clear_in_bitmap(in_nr);
}

void list_df(const char *path)
{
	struct inode par_in, in;

	if (!is_directory(path))
		return;

	read_inode(&par_in, path);

	for (int i = 0; par_in.nrs.in_nrs[i]; i++) {
		__read_inode(&in, par_in.nrs.in_nrs[i]);
		printf("%s\n", in.name);
	}
}

void init_fs()
{
	strcpy(work_dir, "/");

	sti();
	read_sect(1, &bitmaps);
	cli();
}
