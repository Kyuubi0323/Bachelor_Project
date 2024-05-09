#include "sdk.h"
#include "fs.h"

static const char* FS_MODE[2] = {"rb", "wb"};

void* fs_open(const char *path, int flags) {
    return (void*)sAPI_fopen(path, FS_MODE[flags]);
}

int fs_close(void* fd) {
    return sAPI_fclose((SCFILE*)fd);
}

int fs_size(void* fd) {
    return sAPI_fsize((SCFILE*)fd);
}

int fs_read(void* fd, void* buf, int cnt) {
    return sAPI_fread(buf, cnt, 1, (SCFILE*)fd);
}

int fs_write(void* fd, void* buf, int cnt) {
    return sAPI_fwrite(buf, cnt, 1, (SCFILE*)fd);
}

int fs_delete(const char *path) {
    return sAPI_remove(path);
}

int fs_mkDir(const char* dir) {
    return sAPI_mkdir(dir, 0);
}

int fs_rmDir(const char* dir) {
    return sAPI_remove(dir);
}

void* fs_openDir(const char* dir) {
    return (void*)sAPI_opendir(dir);
}

int fs_closeDir(void* dd) {
    return sAPI_closedir((SCDIR*)dd);
}
