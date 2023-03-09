#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

void copy_file(char* source_path, char* dest_path) {
    FILE* source_file = fopen(source_path, "rb");
    FILE* dest_file = fopen(dest_path, "wb");
    if (source_file && dest_file) {
        char buffer[1024];
        size_t num_read = 0;
        while ((num_read = fread(buffer, 1, sizeof(buffer), source_file)) > 0) {
            fwrite(buffer, 1, num_read, dest_file);
        }
    }
    if (source_file) {
        fclose(source_file);
    }
    if (dest_file) {
        fclose(dest_file);
    }
}

void remove_source_file(char* source_path)
{
    if (remove(source_path) != 0) {
        fprintf(stderr, "Failed to remove file: %s\n", source_path);
    }
}

int main() {
    char* source_dir_path = "/home/yogesh/Music/source";
    char* dest_dir_path = "/home/yogesh/Music/destination";

    DIR* source_dir = opendir(source_dir_path);
    if (!source_dir) {
        fprintf(stderr, "Failed to open source directory\n");
        return 1;
    }

    DIR* dest_dir = opendir(dest_dir_path);
    if (!dest_dir) {
        fprintf(stderr, "Failed to open destination directory\n");
        return 1;
    }

    struct dirent* entry;
    while (1) {
        while ((entry = readdir(source_dir))) {
            char* filename = entry->d_name;
            if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
                continue;
            }
            char source_path[1024];
            char dest_path[1024];
            snprintf(source_path, sizeof(source_path), "%s/%s", source_dir_path, filename);
            snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir_path, filename);
            copy_file(source_path, dest_path);
            remove_source_file(source_path);
        }
        rewinddir(source_dir);
        sleep(1);
    }

    closedir(source_dir);
    closedir(dest_dir);

    return 0;
}
