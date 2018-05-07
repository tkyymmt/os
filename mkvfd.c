#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>


#define IN_BITMAP_ADDR 0x200
#define BLK_BITMAP_ADDR 0x300
#define INODES_ADDR 0x400
#define BLOCKS_ADDR 0x80400
#define BLOCK_SIZE 512


struct inode {
	union {
		int16_t in_nrs[128]; // in a case of directory. if negative value, it is inode number of directory
		uint16_t blk_nrs[128]; // in a case of file
	} nrs;
	char name[256];
} __attribute__((packed)); // 512 byte


FILE *vfd_img;


bool is_in_nr_set(int16_t in_nr)
{
	uint8_t byte;
	int offset, shift;

	offset = (abs(in_nr) - 1) / 8 + IN_BITMAP_ADDR;

	fseek(vfd_img, offset, SEEK_SET);
	fread(&byte, 1, 1, vfd_img);

	shift = (abs(in_nr) - 1) % 8;

	return byte >> shift & 1;
}

void set_in_bitmap(int16_t in_nr)
{
	uint8_t byte;
	int offset, shift;

	offset = (abs(in_nr) - 1) / 8 + IN_BITMAP_ADDR;

	fseek(vfd_img, offset, SEEK_SET);
	fread(&byte, 1, 1, vfd_img);

	shift = (abs(in_nr) - 1) % 8;
	byte |= 1 << shift;

	fseek(vfd_img, offset, SEEK_SET);
	fwrite(&byte, 1, 1, vfd_img);
}

bool is_blk_nr_set(uint16_t blk_nr)
{
	uint8_t byte;
	int offset, shift;

	offset = (blk_nr - 1) / 8 + BLK_BITMAP_ADDR;

	fseek(vfd_img, offset, SEEK_SET);
	fread(&byte, 1, 1, vfd_img);

	shift = (blk_nr - 1) % 8;

	return byte >> shift & 1;
}

void set_blk_bitmap(uint16_t blk_nr)
{
	uint8_t byte;
	int offset, shift;

	offset = (blk_nr - 1) / 8 + BLK_BITMAP_ADDR;

	fseek(vfd_img, offset, SEEK_SET);
	fread(&byte, 1, 1, vfd_img);

	shift = (blk_nr - 1) % 8 ;
	byte |= 1 << shift;

	fseek(vfd_img, offset, SEEK_SET);
	fwrite(&byte, 1, 1, vfd_img);
}

int16_t alloc_in_nr()
{
	int16_t in_nr;

	for (in_nr = 2; in_nr < 1025; ++in_nr) {
		if (!is_in_nr_set(in_nr)) {
			set_in_bitmap(in_nr);
			break;
		}
	}

	return in_nr;
}

uint16_t alloc_blk_nr()
{
	uint16_t blk_nr;

	for (blk_nr = 1; blk_nr < 1855; ++blk_nr) {
		if (!is_blk_nr_set(blk_nr)) {
			set_blk_bitmap(blk_nr);
			break;
		}
	}

	return blk_nr;
}

void __read_inode(struct inode *in, int16_t in_nr)
{
	int offset;

	offset = (abs(in_nr) - 1) * sizeof(struct inode) + INODES_ADDR;

	fseek(vfd_img, offset, SEEK_SET);
	fread(in, sizeof(struct inode), 1, vfd_img);
}

int16_t read_inode(struct inode *in, char *path)
{
	const char *name;
	struct inode tmp;
	int16_t in_nr;

	in_nr = -1;
	__read_inode(in, in_nr);

	name = strtok(path, "/");

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

void write_inode(struct inode *in, int16_t in_nr)
{
	int offset;

	offset = (abs(in_nr) - 1) * sizeof(struct inode) + INODES_ADDR;

	fseek(vfd_img, offset, SEEK_SET);
	fwrite(in, sizeof(struct inode), 1, vfd_img);
}

void write_block(void *blk, uint16_t blk_nr)
{
	int offset;

	offset = (blk_nr - 1) * BLOCK_SIZE + BLOCKS_ADDR;

	fseek(vfd_img, offset, SEEK_SET);
	fwrite(blk, BLOCK_SIZE, 1, vfd_img);
}

void mkdir(char *path)
{
	struct inode par_in, in = {0};
	char *name; 
	int16_t par_in_nr;

	name = strrchr(path, '/');
	name[0] = '\0';
	++name;

	strcpy(in.name, name);

	par_in_nr = read_inode(&par_in, path);

	for (int i = 0; i < 128; i++) {
		if (!par_in.nrs.in_nrs[i]) {
			par_in.nrs.in_nrs[i] = -alloc_in_nr();
			write_inode(&par_in, par_in_nr);
			write_inode(&in, par_in.nrs.in_nrs[i]);
			break;
		}
	}
}

void cp(char *src, char *dst)
{
	FILE *fp;
	void *buf;
	size_t size;
	int times;
	struct inode par_in, in = {0};
	char *name;
	int16_t par_in_nr;

	fp = fopen(src, "rb");

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	buf = calloc(1, size + (BLOCK_SIZE - size % BLOCK_SIZE));
	fseek(fp, 0, SEEK_SET);
	fread(buf, size, 1, fp);
	fclose(fp);

	times = size / BLOCK_SIZE;

	for (int i = 0; i < times; i++) {
		in.nrs.blk_nrs[i] = alloc_blk_nr();
		write_block(buf, in.nrs.blk_nrs[i]);
		buf += BLOCK_SIZE;
	}
	in.nrs.blk_nrs[times] = alloc_blk_nr();
	write_block(buf, in.nrs.blk_nrs[times]);

	for (; times; times--, buf -= BLOCK_SIZE);
	free(buf);

	name = strrchr(src, '/');
	name[0] = '\0';
	name++;

	strcpy(in.name, name);

	par_in_nr = read_inode(&par_in, dst);

	for (int i = 0; i < 128; i++) {
		if (!par_in.nrs.in_nrs[i]) {
			par_in.nrs.in_nrs[i] = alloc_in_nr();
			write_inode(&par_in, par_in_nr);
			write_inode(&in, par_in.nrs.in_nrs[i]);
			break;
		}
	}
}

void write_bootloader(const char *path)
{
	FILE *fp;
	void *buf;

	fp = fopen(path, "rb");
	buf = malloc(512);

	fseek(fp, 0, SEEK_SET);
	fread(buf, 512, 1, fp);

	fseek(vfd_img, 0, SEEK_SET);
	fwrite(buf, 512, 1, vfd_img);
	
	free(buf);
	fclose(fp);
}

void init_root()
{
	struct inode root_in = {0};
	
	strcpy(root_in.name, "/");
	set_in_bitmap(-1);
	write_inode(&root_in, -1);
}

void init_vfd_img()
{
	uint8_t zeroed_sects[512 * 2880] = {0};

	fwrite(&zeroed_sects, 512 * 2880, 1, vfd_img);
}

int main(int argc, char *argv[])
{
	int opt;

	vfd_img = fopen(argv[1], "w+b");

	init_vfd_img();
	init_root();

	while ((opt = getopt(argc, argv, "b:m:c:")) != -1) {
		switch (opt) {
		case 'b':
			write_bootloader(optarg);
			break;
		case 'm':
			mkdir(optarg);
			break;
		case 'c':
			cp(argv[optind - 1], argv[optind]);
			break;
		default:
			break;
		}
	}

	fclose(vfd_img);
}
