#include <stdlib.h>
#include <libcd.h>
#include "io.h"

char* load_file(char* filename, u_long *size)
{
    CdlFILE filePos;
    int     numsecs, i;
    char    *buff;

    buff = NULL;

    printf("looking for %s\n", filename);

    if(CdSearchFile(&filePos, filename) == NULL) {
        printf("[ERROR]: File not found %s\n", filename);
        return buff;
    }

    printf("found %s\n", filename);
    numsecs = (filePos.size + 2047) / 2048;
    buff = (char*)malloc(2048 * numsecs);
    CdControl(CdlSetloc, (u_char*)&filePos.pos, 0);
    CdRead(numsecs, (u_long*)buff, CdlModeSpeed);
    printf("numsecs %d size %d\n", numsecs, filePos.size);
    CdReadSync(0, 0);

    printf("file size%d\n", filePos.size);

    for(i = 0; i < filePos.size; i++) {
        printf("%c", buff[i]);
    }

    *size = filePos.size;

    return buff;
}
