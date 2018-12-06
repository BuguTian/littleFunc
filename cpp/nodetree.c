#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int hashsize=10000;

typedef struct _fnode
{
    struct _fnode* parent;
    struct _fnode* child;
    struct _fnode* nextsib;
    struct _fnode* presib;
    struct _fnode* nexthash;
    struct _fnode* prehash;
    char* name;
    unsigned int nleng;
    unsigned long int ino;
}fnode;
static fnode* nodehash[10000];
static fnode* vfsroot = NULL;

void insert_node(fnode* parent,unsigned long int ino,const char* name, unsigned int nlength)
{
    fnode* tmp = NULL;
    fnode* new_node = NULL;
    fnode* tmp2 = NULL;
    unsigned long int hashid;

    if (NULL == parent)
    {
        return ;
    }

    //create a new node.
    new_node = (fnode*)malloc(sizeof(fnode));
    if (NULL == new_node)
    {
        printf("malloc error!");
        return ;
    }
    new_node->parent = parent;
    new_node->child = NULL;
    new_node->nextsib = NULL;
    new_node->presib = NULL;
    new_node->nexthash  = NULL;
    new_node->prehash  = NULL;
    new_node->name = (char*)malloc(nlength+1);
    if (NULL == new_node->name)
    {
        return ;
    }
    memcpy(new_node->name,name,nlength);
    new_node->name[nlength] = 0;
    new_node->nleng = nlength;
    new_node->ino = ino;

    //add sibling
    tmp = parent->child;
    if (NULL == tmp)
    {
        parent->child = new_node;
    }else{
        new_node->nextsib = tmp;
        tmp->presib = new_node;
        parent->child = new_node;
    }

    //add to hash table.
    hashid = ino%hashsize;
    tmp2 = nodehash[hashid];
    if (NULL == tmp2)
    {
        nodehash[hashid] = new_node;
    }else{
        new_node->nexthash = tmp2;
        nodehash[hashid] = new_node;
        tmp2->prehash = new_node;
    }
}

fnode* get_node(unsigned long int ino)
{
    fnode* tmp = nodehash[ino%hashsize];

    do
    {
        if (tmp->ino == ino)
        {
            return tmp;
        }
        tmp = tmp->nexthash;
    }while (NULL != tmp);
    return NULL;
}

void remove_node (fnode* node)
{
    fnode* tmp = NULL;
    unsigned long int hashid = 0;

    if (NULL == node->presib)
    {
        node->parent->child = node->nextsib;
        if (NULL != node->nextsib)
        {
            node->nextsib->presib = NULL;
        }
    }else{
        if (NULL != node->nextsib)
        {
            node->nextsib->presib = node->presib;
            node->presib->nextsib = node->nextsib;
        }else{
            node->presib->nextsib = NULL;
        }
    }

    if (node->prehash == NULL)
    {
        hashid = (node->ino) % hashsize;
        nodehash[hashid] = node->nexthash;
        if (node->nexthash != NULL)
        {
            node->nexthash->prehash = NULL;
        }
    }else{
        node->prehash->nexthash = node->nexthash;
        if (node->nexthash != NULL)
        {
            node->nexthash->prehash = node->prehash;
        }
    }
}

void init_tree()
{
    vfsroot = (fnode*)malloc(sizeof(fnode));
    if (NULL == vfsroot)
    {
        printf("malloc error!");
        return ;
    }
    vfsroot->parent = NULL;
    vfsroot->child = NULL;
    vfsroot->nextsib = NULL;
    vfsroot->presib = NULL;
    vfsroot->nexthash  = NULL;
    vfsroot->prehash  = NULL;
    vfsroot->name = (char*)malloc(2);
    if (NULL == vfsroot->name)
    {
        printf("malloc error!");
        return ;
    }
    vfsroot->name[0] = '/';
    vfsroot->name[1] = 0;
    vfsroot->nleng = 2;
    vfsroot->ino = 1;
}

int main()
{
    fnode* tmp;
    init_tree();
    insert_node(vfsroot,2,"ee",3);
    insert_node(vfsroot,3,"ff",3);
    insert_node(vfsroot,4,"gg",3);
    insert_node(vfsroot,5,"hh",3);
    tmp = vfsroot->child;
    while(NULL != tmp)
    {
        printf("%s\n",tmp->name);
        tmp=tmp->nextsib;
    }
    return 0;
}
