#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#define FS_READ_ONLY    0
#define FS_WRITE_ONLY   1

void* fs_open(const char *path, int flags);
int fs_close(void* fd);
int fs_size(void* fd);
int fs_read(void* fd, void* buf, int cnt);
int fs_write(void* fd, void* buf, int cnt);
int fs_delete(const char *path);
int fs_isExist(const char* path);
int fs_mkDir(const char* dir);
int fs_rmDir(const char* dir);
void* fs_openDir(const char* dir);
int fs_closeDir(void* dd);

#endif /* __FILESYSTEM_H__ */