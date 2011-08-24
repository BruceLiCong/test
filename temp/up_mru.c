#include "up_mru.h"
#include "up_gdi.h"
#include "up_ucstring.h"
#include "up_string.h"
#include "up_memory.h"


static void Up_Mru_Prepend(t_mru_node** ppList, t_mru_node* pNode)
{
	t_mru_node* pFirst = *ppList;


	if(pFirst)
	{
		t_mru_node* pLast = pFirst->m_pPre;

		pFirst->m_pPre = pNode;
		pLast->m_pNext  = pNode;
		pNode->m_pNext  = pFirst;
		pNode->m_pPre  = pLast;
	}
	else
	{
		pNode->m_pNext = pNode;
		pNode->m_pPre = pNode;
	}

	*ppList = pNode;
}

static void Up_Mru_Up(t_mru_node** ppList, t_mru_node* pNode)
{
	t_mru_node* pFirst = *ppList;

	//check first

	if (pFirst != pNode)
	{
		t_mru_node* pPrev;
		t_mru_node* pNext;
		t_mru_node* pLast;

		pPrev = pNode->m_pPre;
		pNext = pNode->m_pNext;

		pPrev->m_pNext = pNext;
		pNext->m_pPre = pPrev;

		pLast = pFirst->m_pPre;

		pLast->m_pNext  = pNode;
		pFirst->m_pPre = pNode;

		pNode->m_pNext = pFirst;
		pNode->m_pPre = pLast;

		*ppList = pNode;
	}
}

static void Up_Mru_Remove(t_mru_node** ppList, t_mru_node* pNode)
{
	t_mru_node* pFirst = *ppList;
	t_mru_node* pPrev;
	t_mru_node* pNext;

	//check first

	pPrev = pNode->m_pPre;
	pNext = pNode->m_pNext;

	pPrev->m_pNext = pNext;
	pNext->m_pPre = pPrev;

	if(pNode == pNext)
	{
		*ppList = NULL;
	}
	else if(pNode == pFirst)
	{
		*ppList = pNext;
	}
}


void Up_MruList_Init(t_mru_list* pList, t_mru_list_clazz* pClazz, int iMaxNodes, void* pData)
{
	//check
	pList->m_iNodesCount = 0;
	pList->m_iMaxCount = iMaxNodes;
	pList->m_pNodes = NULL;
	pList->m_tClazz = *pClazz;
	pList->m_pData = pData;
}

void Up_MruList_Reset(t_mru_list* pList)
{
	while (pList->m_pNodes)
	{
		Up_MruList_Remove(pList, pList->m_pNodes);
	}
}


void Up_MruList_Done(t_mru_list* pList)
{
	Up_MruList_Reset(pList);
}


void Up_MruList_New(t_mru_list* pList, void* pKey, t_mru_node** ppNode)
{
	t_mru_node* pNode = NULL;

	if (pList->m_iNodesCount >= pList->m_iMaxCount && pList->m_iMaxCount > 0)
	{
		pNode = pList->m_pNodes->m_pPre;

		//check node

		if (pList->m_tClazz.m_fReset)
		{
			Up_Mru_Up(&pList->m_pNodes, pNode);

			pList->m_tClazz.m_fReset(pNode, pKey, pList->m_pData);

			goto Exit;
		}

		Up_Mru_Remove(&pList->m_pNodes, pNode);
		pList->m_iNodesCount--;

		if (pList->m_tClazz.m_fDone)
		{
			pList->m_tClazz.m_fDone(pNode, pList->m_pData);
		}
	}
	else
	{
		pNode = (t_mru_node*)up_zero_alloc(pList->m_tClazz.m_iNodeSize);

		if (NULL == pNode)
		{
			goto Exit;
		}
	}

	pList->m_tClazz.m_fInit(pNode, pKey, pList->m_pData);

	Up_Mru_Prepend(&pList->m_pNodes, pNode);
	pList->m_iNodesCount++;

Exit:
	*ppNode = pNode;
}

void Up_MruList_Remove(t_mru_list* pList,t_mru_node* pNode)
{
	Up_Mru_Remove(&pList->m_pNodes, pNode);
	pList->m_iNodesCount--;

	if (pList->m_tClazz.m_fDone)
	{
		pList->m_tClazz.m_fDone(pNode, pList->m_pData);
	}
	
	up_free(pNode);
}

void Up_MruList_RemoveSelection(t_mru_list* pList, Up_MruNode_CompareFunc fSelection, void* pKey)
{
	t_mru_node* pFirst;
	t_mru_node* pNode;
	t_mru_node* pNext;

	pFirst = pList->m_pNodes;

	while (pFirst && ( NULL == fSelection || fSelection(pFirst, pKey)))
	{
		Up_MruList_Remove(pList, pFirst);
		pFirst = pList->m_pNodes;
	}

	if (pFirst)
	{
		pNode = pFirst->m_pNext;

		while (pNode != pFirst)
		{
			pNext = pNode->m_pNext;

			if (fSelection(pNode, pKey))
			{
				Up_MruList_Remove(pList, pNode);
			}

			pNode = pNext;
		}
	}
}

void Up_MruList_Lookup(t_mru_list* pList, void* pKey, t_mru_node** ppNode)
{
	t_mru_node* pNode;

	pNode = Up_MruList_Find(pList, pKey);
	if (NULL == pNode)
	{
		return Up_MruList_New(pList, pKey, ppNode);
	}
	*ppNode = pNode;
}

t_mru_node* Up_MruList_Find(t_mru_list* pList, void* pKey)
{
	Up_MruNode_CompareFunc  fCompare = pList->m_tClazz.m_fCompare;
	t_mru_node* pFirst;
	t_mru_node* pNode;

	pFirst = pList->m_pNodes;
	pNode  = NULL;

	if (pFirst)
	{
		pNode = pFirst;
		do
		{
			if(fCompare(pNode, pKey))
			{
				if (pNode != pFirst)
				{
					Up_Mru_Up(&pList->m_pNodes, pNode);
				}
				return pNode;
			}

			pNode = pNode->m_pNext;

		} while(pNode != pFirst);
	}

	return NULL;
}