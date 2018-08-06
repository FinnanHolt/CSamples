#include "bst.h"

#define STRSIZE 50

int mystrcmp(const void* a, const void* b);
char* myprintstr(const void* v);
bst* file_bst_create(FILE *fp);
void print_tree(bstnode* b);
bst* misspelt(FILE*fp, bst* b);

int main(int argc, char* argv[])
{

  bst*b, *b1;
  FILE* fp;
  if (argc != 3){
    ON_ERROR("Please Input 2 Arguments\n");
  }


  if ((fp = fopen(argv[1], "r"))==NULL){
    ON_ERROR("First File Could Not be Opened\n");
  }
  b=file_bst_create(fp);
  fclose(fp);

  if ((fp = fopen(argv[2], "r"))==NULL){
    ON_ERROR("Second File Could Not be Opened\n");
  }

  b1=misspelt(fp, b);
  fclose(fp);

  print_tree(b1->top);

  bst_free(&b1);
  bst_free(&b);
  return 0;
}

bst* file_bst_create(FILE *fp)
{
    bst* b;
    char temp[STRSIZE];

    b=bst_init(STRSIZE, mystrcmp, myprintstr);

    while (fscanf(fp, "%s", temp)!=EOF){
        bst_insert(b, temp);
    }
    return b;
}

int mystrcmp(const void* a, const void* b)
{
   return strcmp(a, b);
}

char* myprintstr(const void* v)
{
   return (char*)v;
}

void print_tree(bstnode* node)
{
  if (node == NULL){
    return;
  }
  printf("%s\n",(char*)node->data);
  if (node->left)
  {
    print_tree(node->left);
  }
  if (node->right)
  {
    print_tree(node->right);
  }
}

bst* misspelt(FILE*fp, bst* b)
{
  bst* b1;
  char temp[STRSIZE];

  b1=bst_init(STRSIZE, mystrcmp, myprintstr);

  while (fscanf(fp, "%s", temp)!=EOF){
      if (bst_isin(b, (char*)temp)){
    }
      else {
        bst_insert(b1, (char*)temp);
      }
  }
  return b1;
}
