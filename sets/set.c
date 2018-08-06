#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

/* Create empty set */
set* set_init(void)
{
   set* l;
   l = (set*) calloc(1,sizeof(set));
   if(l == NULL){
      ON_ERROR("Creation of Array Failed\n");
   }
   l->ua = arr_init();
   l->sz = 0;

   return l;
}

/* Create new set, copied from another */
set* set_copy(set* s)
{
   int i;
   set*s1;
   s1=set_init();
   for (i=0; i<s->sz; i++)
   {
      set_insert(s1, arr_get(s->ua, i));
   }
   return s1;
}

/* Create new set, copied from an array of length n*/
set* set_fromarray(arrtype* a, int n)
{
   int i;
   set*s1;
   s1=set_init();
   for (i=0; i<n; i++)
   {
      set_insert(s1, a[i]);
   }
   return s1;
}

/* Basic Operations */
/* Add one element into the set */
void set_insert(set* s, arrtype l)
{
   if (s==NULL)
   {
      return;
   }
   if (set_contains(s,l)==0)
   {
      arr_set(s->ua, s->sz, l);
      s->sz++;
   }
}

/* Return size of the set */
int set_size(set* s)
{
   if (s == NULL)
   {
      return 0;
   }
   return s->sz;
}

/* Returns true if l is in the array, false elsewise */
int set_contains(set* s, arrtype l)
{
   int i;
   arrtype temp;
   if (s==NULL)
   {
      return 0;
   }
   for (i=0; i<s->sz; i++)
   {
      temp=arr_get(s->ua, i);
      if (memcmp(&temp, &l, sizeof(arrtype))==0)
      {
         return 1;
      }
   }
   return 0;
}

/* Remove l from the set (if it's in) */
void set_remove(set* s, arrtype l)
{
   int i, j;
   arrtype temp;
   if (s==NULL)
   {
   return;
   }
   if (set_contains(s,l)==1)
   {
      for (i=0; i<s->sz; i++)
      {
         if (memcmp(&temp, &l, sizeof(arrtype))==0)
         {
            for (j=i; j<s->sz-1; j++)
            {
               arr_set(s->ua,j, arr_get(s->ua, j+1));
            }
         }
      }
      s->sz--;
   }
}

/* Remove one element from the set - there's no
   particular order for the elements, so any will do */
arrtype set_removeone(set* s)
{
   arrtype rmv;
   rmv = arr_get(s->ua, s->sz-1);
   s->sz--;
   return rmv;
}

/* Operations on 2 sets */
/* Create a new set, containing all elements from s1 & s2 */
set* set_union(set* s1, set* s2)
{
   int i;
   set* s3;
   s3=set_init();
   if (s1==NULL&&s2==NULL)
   {
      return s3;
   }
   if (s1==NULL)
   {
      s3=set_copy(s2);
      return s3;
   }
   if (s2==NULL)
   {
      s3=set_copy(s1);
      return s3;
   }
   for (i=0; i<s1->sz; i++)
   {
      set_insert(s3, arr_get(s1->ua, i));
   }
   for (i=0; i<s1->sz; i++)
   {
      set_insert(s3, arr_get(s2->ua, i));
   }
   return s3;
}

/* Create a new set, containing all elements in both s1 & s2 */
set* set_intersection(set* s1, set* s2)
{
   int i,j;
   arrtype tempi, tempj;
   set*s3;
   s3=set_init();
   if (s1==NULL||s2==NULL)
   {
      return s3;
   }
   for (i=0; i<s1->sz; i++)
   {
      tempi = arr_get(s1->ua, i);
      for (j=0; j<s2->sz; j++)
      {
         tempj=arr_get(s2->ua, j);
         if (memcmp(&tempi, &tempj, sizeof(arrtype))==0)
         {
            set_insert(s3, arr_get(s1->ua, i));
         }
      }
   }
   return s3;
}

/* Finish up */
/* Clears all space used, and sets pointer to NULL */
void set_free(set **s)
{
   set* s1;

   if (s==NULL)
   {
      return;
   }

   s1=*s;

   arr_free(&s1->ua);
   free(s1);
   *s=NULL;
}
