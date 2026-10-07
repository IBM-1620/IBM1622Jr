//======================================================================================================================
//
//  select.c - select card deck to read
//
//  IBM 1620 Jr Project, Computer History Museum, 2017-2026
//
//  To recreate the experience (visual, auditory, tactile, visceral) of running historic software on a 1960s-era
//  computer.
//
//    Dave Babcock  - project lead, software, website
//   David Brock    - CHM sponsor
//   Steve Casner   - hardware, software
//     Joe Fredrick - hardware, firmware
//     Len Shustek  - CHM advisor
//     Dag Spicer   - CHM advisor
//   David Wise     - IBM 1620 expert
//
//======================================================================================================================

#include "defines.h"
#include "data.h"
#include "display.h"
#include "select.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// Function declarations
int ProcessDirectory(char *dirname);
bool IsCardFile(char *filename);
int CompareEntries(const void *entry1, const void *entry2);
void EllipsisFilename(char *new, char *old, int max);


// Types
struct entry_struct {
    char type;
    char extension[4];
    char filename[NAME_MAX + 1];
};


// Data


// SelectFile
bool SelectFile(void) {
    int ret;

    ret = ProcessDirectory("");
    display = DISPLAY_READER_PUNCH;
    if (ret == SELECTED_ITEMS) {
        return TRUE;
    }

    return FALSE;
}

// ProcessDirectory
int ProcessDirectory(char *dirname) {
    DIR *dir;
    struct dirent *entry;
    char pathname[PATH_MAX + 1];
    char filename[PATH_MAX + 1];
    int num;
    int ret;
    bool topDirectory = FALSE;
    char str[PATH_MAX + 1];
    struct entry_struct entries[SELECT_MAX_ENTRIES];

    num = 0;
    topDirectory = (dirname[0] == (char)0);
    (void)strcpy(pathname, readerMount);
    if (!topDirectory) {
        (void)strcat(pathname, "/");
        (void)strcat(pathname, dirname);
    }
    if ((dir = opendir(pathname)) != NULL) {
        while (((entry = readdir(dir)) != NULL) && (num < SELECT_MAX_ENTRIES)) {
            if (entry->d_type == DT_DIR) {
                if ((entry->d_name[0] != (char)0) && (entry->d_name[0] != '.') &&
                    (strcmp(entry->d_name, "System Volume Information") != 0)) {
                    entries[num].type = DIRECTORY_TYPE;
                    (void)strcpy(entries[num].extension, "");
                    (void)strcpy(entries[num].filename, entry->d_name);
                    ++num;
                }
            } else if (entry->d_type == DT_REG) {
                if ((entry->d_name[0] != (char)0) && (entry->d_name[0] != '.')) {
                    (void)strcpy(filename, pathname);
                    (void)strcat(filename, "/");
                    (void)strcat(filename, entry->d_name);
                    if (IsCardFile(filename)) {
                        entries[num].type = FILE_TYPE;
                        char *ext = strrchr(entry->d_name, '.');
                        if (ext != NULL) {
                            (void)strncpy(entries[num].extension, ++ext, 3);
                            entries[num].extension + 3 = (char)0;
                        } else {
                            (void)strcpy(entries[num].extension, "");
                        }
                        (void)strcpy(entries[num].filename, entry->d_name);
                        ++num;
                    }
                }
            }
        }
        (void)closedir(dir);
    } else {
        printf("ProcessDirectory opendir error = %s\n", strerror(errno));
        return SELECTED_NONE;
    }

    if (topDirectory) {
        if ((num == 1) && (entries[0].type == FILE_TYPE)) {
            (void)strcpy(readerFilename, pathname);
            (void)strcat(readerFilename, "/");
            (void)strcat(readerFilename, entries[0].filename);
            return SELECTED_ITEMS;
        }
    }

    if (num > 1) {
        qsort(entries, num, sizeof(struct entry_struct), CompareEntries);
    }

    while (TRUE) {

        (void)pthread_mutex_lock(&displayLock);

        // Copy directory name to display title
        (void)strcpy(str, "//");
        (void)strcat(str, dirname);
        EllipsisFilename(title, str, TITLE_LINE_LENGTH);

        // Copy directory contents to display table
        numEntries = num;
        firstEntry = 0;
        for (int i = 0; i < num; ++i) {
            items[i].type = entries[i].type;
            (void)strcpy(items[i].extension, entries[i].extension);
            EllipsisFilename(items[i].filename, entries[i].filename, ITEM_LINE_LENGTH);
        }

        (void)pthread_mutex_unlock(&displayLock);

        // Wait for directory or file to be selected
        display = DISPLAY_FILE_SELECT;
        selectedItem = SELECTED_NONE;
        selectedFile = SELECTED_NONE;
        while ((selectedFile == SELECTED_NONE) && (access(readerDev, F_OK) == 0) && running) {
            (void)usleep(100000);
        }

        // Process directory or file selection
        if (selectedFile == SELECTED_BACKUP) {
            return SELECTED_BACKUP;
        } else if (selectedFile >= SELECTED_ITEMS) {
            if (entries[selectedFile - SELECTED_ITEMS].type == DIRECTORY_TYPE) {
                if (!topDirectory) {
                    (void)strcpy(str, dirname);
                    (void)strcat(str, "/");
                    (void)strcat(str, entries[selectedFile - SELECTED_ITEMS].filename);
                } else {
                    (void)strcpy(str, entries[selectedFile - SELECTED_ITEMS].filename);
                }
                ret = ProcessDirectory(str);
                if (ret == SELECTED_BACKUP) {
                    continue;
                } else {
                    return ret;
                }
            } else /* entries[selectedFile - SELECTED_ITEMS].type == FILE_TYPE */ {
                (void)strcpy(readerFilename, pathname);
                (void)strcat(readerFilename, "/");
                (void)strcat(readerFilename, entries[selectedFile - SELECTED_ITEMS].filename);
                return SELECTED_ITEMS;
            }
        } else {
            return SELECTED_NONE;
        }
    }

    return SELECTED_NONE;
}

// IsCardFile
bool IsCardFile(char *filename) {
    int file;
    int cnt;
    int found;
    char chr;
    char buf[85];

    if ((file = open(filename, O_RDONLY)) != -1) {
        cnt = read(file, buf, 85);
        (void)close(file);
        found = FALSE;
        for (int i = 0; i < cnt; ++i) {
            chr = buf[i] & 0x7F;
            if (chr == '\n') {
                found = TRUE;
                continue;
            }
            if (alphamericReaderActions[(int)chr][0] == ACTION_ERROR) return FALSE;
        }
        if ((cnt > 80) && !found) return FALSE;

    } else {
        printf("IsCardFile open error = %s\n", strerror(errno));
        return FALSE;
    }

    return TRUE;
}

// CompareEntries
int CompareEntries(const void *entry1, const void *entry2) {
    if ((*(struct entry_struct *)entry1).type < (*(struct entry_struct *)entry2).type) return -1;
    if ((*(struct entry_struct *)entry1).type > (*(struct entry_struct *)entry2).type) return 1;
    return strcasecmp((*(struct entry_struct *)entry1).filename, (*(struct entry_struct *)entry2).filename);
}

// Ellipsis filename
void EllipsisFilename(char *new, char *old, int max) {
    int len;

    len = strlen(old);
    if (len <= max) {
        (void)strcpy(new, old);
    } else {
        (void)strncpy(new, old, (max / 2) - 2);
        (void)strcpy(new + (max / 2) - 2, "...");
        (void)strcpy(new + (max / 2) + 1, old + len - (max / 2) + 1);
    }
}
