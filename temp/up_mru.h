#ifndef UP_MRU_H
#define	UP_MRU_H


typedef struct mru_node
{
	struct mru_node* m_pNext;
	struct mru_node* m_pPre;
}t_mru_node;


typedef int (*Up_MruNode_CompareFunc)(t_mru_node*  pNode, void* pKey);

typedef void (*Up_MruNode_InitFunc)(t_mru_node*  pNode, void* pKey, void* pData);

typedef void (*Up_MruNode_ResetFunc)(t_mru_node*  pNode, void* pKey, void* pData);

typedef void (*Up_MruNode_DoneFunc)(t_mru_node*  pNode, void* pData);



typedef struct mru_list_clazz
{
	int m_iNodeSize;

	Up_MruNode_CompareFunc m_fCompare;
	Up_MruNode_InitFunc m_fInit;
	Up_MruNode_ResetFunc m_fReset;
	Up_MruNode_DoneFunc m_fDone;
}t_mru_list_clazz;


typedef struct mru_list
{
	int m_iMaxCount;
	int m_iNodesCount;

	t_mru_node* m_pNodes;

	void* m_pData;
	t_mru_list_clazz m_tClazz;
}t_mru_list;

static void Up_Mru_Prepend(t_mru_node** ppList, t_mru_node* pNode);

static void Up_Mru_Up(t_mru_node** ppList, t_mru_node* pNode);

static void Up_Mru_Remove(t_mru_node** ppList, t_mru_node* pNode);


void Up_MruList_Init(t_mru_list* pList, t_mru_list_clazz*  pClazz, int iMaxNodes, void* pData);

void Up_MruList_Reset(t_mru_list* pList);


void Up_MruList_Done(t_mru_list* pList);


void Up_MruList_New(t_mru_list* pList, void* pKey, t_mru_node** ppNode);

void Up_MruList_Remove(t_mru_list* pList,t_mru_node* pNode);

void Up_MruList_RemoveSelection(t_mru_list* pList, Up_MruNode_CompareFunc fSelection, void* pKey);

void Up_MruList_Lookup(t_mru_list* pList, void* pKey, t_mru_node** ppNode);

t_mru_node* Up_MruList_Find(t_mru_list* pList, void* pKey);

#endif