#include "bst.h"

#define STRSIZE 50
#define BRACKETS 3
#define ROOT 1

void bst_nodeinsert(bst*b, bstnode* node, void* v);
bstnode* create_node(bst* b, void*v);
int bst_nodesize(bstnode*node);
int bst_nodemaxdepth(bstnode* node);
bool bst_isinnode(bst*b, bstnode* node, void* v);
void bst_nodefree(bstnode* node);
char* bst_nodeprint(bst*b, bstnode*node);
int * bst_nodegetordered(bst * b, bstnode*node, char*ptr);
void bst_noderebalance(bst*b, char* arr, int arr_start, int arr_size);


bst* bst_init(int sz, int(*comp)(const void* a, const void* b), char*(*prnt)(const void* a))
{
    bst* b;
    b = (bst*)calloc(1,sizeof(bst));
    if(b == NULL){
      ON_ERROR("Creation of Tree Failed\n");
    }
    b->top=NULL;
    b->elsz=sz;
    b->compare=comp;
    b->prntnode=prnt;
    return b;
}

void bst_insert(bst* b, void* v)
{
  if (b->top==NULL){
    b->top=create_node(b,v);
  }
  else {
    bst_nodeinsert(b, b->top, v);
  }
}

void bst_nodeinsert(bst*b, bstnode* node, void* v)
{
  bstnode* node1;

  if(b == NULL) {
    return;
  }
  if (node==NULL){
    node1 = create_node(b, v);
    node=node1;
    return;
  }
  if (b->compare(v, node->data)==0){
    return;
    }

  if (b->compare(node->data,v)>0){
    if (node->left==NULL){
      node1=create_node(b, v);
      node->left=node1;
    }
    else{
      bst_nodeinsert(b,node->left,v);
    }
  }
  else{
      if (node->right==NULL){
        node1=create_node(b, v);
        node->right=node1;
      }
      else{
        bst_nodeinsert(b,node->right,v);
      }
  }
  return;
}

bstnode* create_node(bst* b, void*v)
{
  bstnode* node;
  node=(bstnode*)calloc(1, sizeof(bstnode)+1);
  if(node == NULL){
    ON_ERROR("Creation of Node Failed\n");
  }
  node->data=(void*)calloc(1, b->elsz);
  if(node->data == NULL){
    ON_ERROR("Data Initilisation Failed\n");
  }
  memcpy(node->data, v, b->elsz);
  node->left=NULL;
  node->right=NULL;
  return node;
}

int bst_size(bst* b)
{
  return bst_nodesize(b->top);
}

int bst_nodesize(bstnode*node)
{
  if (node==NULL){
  return 0;
  }
  else{
  return(bst_nodesize(node->left) + bst_nodesize(node->right) + ROOT);
  }
}

int bst_maxdepth(bst* b)
{
  return bst_nodemaxdepth(b->top);
}

int bst_nodemaxdepth(bstnode* node)
{
  int depth_left, depth_right;
  if(node==NULL){
     return 0;
  }
  else{
     depth_left=bst_nodemaxdepth(node->left);
     depth_right=bst_nodemaxdepth(node->right);
     if(depth_left>depth_right){
        return (depth_left+ROOT);
      }
     else{
        return (depth_right+ROOT);
      }
  }
}

bool bst_isin(bst*b, void*v)
{
  return bst_isinnode(b,b->top,v);
}

bool bst_isinnode(bst*b, bstnode* node, void* v)
{
  if (node==NULL){
    return false;
  }
  if (b->compare(node->data,v)==0){
    return true;
  }

  if (b->compare(node->data,v)>0){
    return bst_isinnode(b,node->left,v);

  }
  if (b->compare(node->data, v)<0){
    return bst_isinnode(b,node->right,v);
  }
  return false;
}
void bst_insertarray(bst* b, void* v, int n)
{
  int i;
  char*ptr;
  ptr=(char*)v;
  for (i=0; i<n; i++){
    bst_insert(b, ptr);
    ptr+=b->elsz;
  }
}

void bst_free(bst **p)
{
if (*p==NULL){
  return;
}
bst_nodefree((bstnode *)(*p)->top);
free(*p);
*p=NULL;
}

void bst_nodefree(bstnode* node)
{
  if (node == NULL){
    return;
  }
  if (node->left){
    free(node->data);
    node->data = NULL;
    bst_nodefree(node->left);
  }
  if (node->right){
    if(node->data != NULL)
      free(node->data);
      node->data=NULL;
    bst_nodefree(node->right);
  }
  else{
    free(node->data);
    node->data=NULL;
  }
  free(node);
}

char* bst_print(bst* b)
{
  if (b==NULL){
    return 0;
  }
  return bst_nodeprint(b,b->top);
}

char* bst_nodeprint(bst*b, bstnode*node)
{
  int n1, n2, n3;
  char*str=NULL;
  char*str_left=NULL, *str_right=NULL;

  n1=0;
  n2=0;
  n3=0;

  if (node==NULL){
    str=(char*)calloc(STRSIZE, sizeof(char));
    return str;
  }

  str_left=bst_nodeprint(b,node->left);
  str_right=bst_nodeprint(b,node->right);

  n1=strlen(str_left);
  n2=strlen(str_right);
  n3=sizeof(b->prntnode(node->data));
  str=(char*)calloc(1,n1+n2+n3+BRACKETS);

  sprintf(str, "(%s%s%s)", b->prntnode(node->data), str_left, str_right);

  free(str_left);
  free(str_right);

  return str;

}

void bst_getordered(bst* b, void* v)
{
  int *i;
  char*ptr;
  ptr=(char*)v;

  i=bst_nodegetordered(b, b->top, ptr);
  *i=0;
}

int * bst_nodegetordered(bst * b, bstnode*node, char*ptr)
{
  static int i = 0;
  if (node==NULL){
    return &i;
  }

  bst_nodegetordered(b,node->left,ptr);
  memcpy(ptr+((i)*(b->elsz)), node->data, b->elsz);
  i++;
  bst_nodegetordered(b,node->right,ptr);
  return &i;
}

bst * bst_rebalance(bst*b)
{
  int arr_start, arr_size;
  char* arr;
  bst* newtree;
  arr_start=0;

  if (b==NULL){
    return NULL;
  }

  arr_size=bst_size(b)-1;
  arr=(char*)calloc(arr_size,b->elsz+1);

  if (arr==NULL){
    ON_ERROR("Array initialisation Failed.");
  }

  bst_getordered(b, arr);
  newtree=bst_init(b->elsz, b->compare, b->prntnode);
  bst_noderebalance(newtree, arr, arr_start, arr_size);
  free(arr);
  return newtree;

}

void bst_noderebalance(bst*b, char* arr, int arr_start, int arr_size)
{

  int median;
  median=(arr_start+arr_size)/2;
  if(arr_start>arr_size){
    return;
  }

  bst_insert(b, arr+median*(b->elsz));
  bst_noderebalance(b, arr, arr_start, median-1);
  bst_noderebalance(b, arr, median+1, arr_size);

  return;
}
