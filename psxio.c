#include <stdlib.h>
#include <libcd.h>
#include "io.h"

char* load_file(char* filename, u_long *size)
{
    CdlFILE file;
    int     numsecs, i;
    char    *buff;

    buff = NULL;

    printf("[INFO]: looking for %s\n", filename);

    if(CdSearchFile(&file, filename) == NULL) {
        printf("[ERROR]: File not found %s\n", filename);
        return buff;
    }

    printf("[INFO]: found %s\n", filename);
    numsecs = (file.size + 2047) / 2048;
    buff = (char*)malloc(2048 * numsecs);
    CdControl(CdlSetloc, (u_char*)&file.pos, 0);
    CdRead(numsecs, (u_long*)buff, CdlModeSpeed);
    printf("[INFO]: numsecs: %d\n", numsecs);
    CdReadSync(0, 0);

    printf("[INFO]: read sync done: %d\n", file.size);
    *size = file.size;

    return buff;
}
