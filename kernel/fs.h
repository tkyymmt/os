#ifndef FS_H
#define FS_H


#include <stdint.h>
#include <stdbool.h>


extern char work_dir[256];


extern void load_file(const char *path, uint32_t vaddr);
extern bool is_directory(const char *path);
extern void make_dir(const char *path);
extern void remove_dir(const char *path);
extern void list_df(const char *path);
extern void init_fs();


#endif // FS_H
