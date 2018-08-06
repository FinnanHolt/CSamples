#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

set * file_set_create(FILE *fp);
set* unique(set*s1, set*s2);

int main (void)
{
    FILE * fp;
    set *s1, *s2, *s3;

    if ((fp=fopen("pride-and-prej.txt", "r"))==NULL)
    {
        perror("Can't open file");
    }
    s1=file_set_create(fp);
    fclose(fp);

    if ((fp=fopen("sense-and-sense.txt", "r"))==NULL)
    {
        perror("Can't open file");
    }
    s2=file_set_create(fp);
    fclose(fp);

    s3=set_intersection(s1, s2);

    printf("There are %i unique words in sense-and-sense.txt\n"
    ,s2->sz);
    printf("There are %i unique words in pride-and-prej.txt\n"
    ,s1->sz);
    printf("There are %i common words\n", s3->sz);

    set_free(&s1);
    set_free(&s2);
    set_free(&s3);

    return 0;
}

set * file_set_create(FILE *fp)
{
    set* s1;
    arrtype temp;
    s1=set_init();
    memset(&temp, 0, sizeof(arrtype));

    while (fscanf(fp, "%s", temp.str)!=EOF)
    {
        set_insert(s1, temp);
        memset(&temp, 0, sizeof(arrtype));
    }
    return s1;
}
