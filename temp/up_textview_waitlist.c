#include "up_textview_inner.h"

t_node* up_textview_waitlist_get_out(t_waitlist* pList)
{
	t_node* pNode = (t_node*)NULL;

	if(NULL != pList->m_pFirst)
	{
		t_waitlistnode* pOldWaiter = pList->m_pFirst;

		pList->m_pFirst = pOldWaiter->m_pNext;
		pNode = pOldWaiter->m_pNode;

		zero_free(pOldWaiter);
	}

	return pNode;
}

void up_textview_waitlist_put_in(t_waitlist* pList, t_node* pNode)
{
	t_waitlistnode* pNewWaiter = (t_waitlistnode*)zero_alloc(sizeof(t_waitlistnode));

	pNewWaiter->m_pNode = pNode;
	pNewWaiter->m_pNext = NULL;

	if(NULL == pList->m_pFirst)
	{
		pList->m_pFirst = pNewWaiter;
		pList->m_pLast = pNewWaiter;
	}
	else
	{
		pList->m_pLast->m_pNext = pNewWaiter;
		pList->m_pLast = pNewWaiter;
	}
}

t_waitlist* up_textview_waitlist_create()
{
	return (t_waitlist*)zero_alloc(sizeof(t_waitlist));
}

void up_textview_waitlist_destroy(t_waitlist* pList)
{
	t_node* pNode = NULL;

	do
	{
		pNode = up_textview_waitlist_get_out(pList);
	}while(pNode);

	zero_free(pList);
}
