#include "up_textview_inner.h"


static void up_textview_pattern_branch_add(t_node* pNode, t_code* pCodes, unsigned long ulSize, void* pLabel, unsigned long ulDepth)
{
	t_node* pCurNode = pNode->m_pChild;

	while(pCurNode)
	{
		if(pCurNode->m_tCode == pCodes[0])
		{			
			break;
		}
		pCurNode = pCurNode->m_pBrother;
	}

	if(NULL == pCurNode)
	{
		pCurNode = (t_node*)zero_alloc(sizeof(t_node));

		pCurNode->m_tCode = pCodes[0];
		pCurNode->m_ulDepth = ulDepth;
		pCurNode->m_pBrother = pNode->m_pChild;

		pNode->m_pChild = pCurNode;
	}

	if(ulSize <= 1)
	{
		pCurNode->m_pLabel = pLabel;
	}
	else
	{
		up_textview_pattern_branch_add(pCurNode, pCodes + 1, ulSize - 1, pLabel, ulDepth + 1);
	}
}

t_pattern_ac* up_textview_pattern_add(t_pattern_ac* pPattern, t_code* pCodes, unsigned long ulSize, t_graphics_adaptor* pGrc, void* pData)
{
	t_node* pRoot = (t_node*)pPattern;

	void* pLabel = (pGrc) ? (pGrc->m_fCreateLabel(pGrc->m_pHandle, pData)) : NULL;

	if(NULL == pRoot)
	{
		pRoot = (t_node*)zero_alloc(sizeof(t_node));
	}

	if (pRoot)
	{
		up_textview_pattern_branch_add(pRoot, pCodes, ulSize, (void*)pLabel, 1);
	}

	return pRoot;
}

void up_textview_pattern_destroy(t_pattern_ac* pPattern, t_graphics_adaptor* pGrc)
{
	t_node* pNode = (t_node*)pPattern;

	if ((pNode->m_pLabel) && pGrc)
	{
		pGrc->m_fDestroyLabel(pGrc->m_pHandle, pNode->m_pLabel);
	}

	if (pNode->m_pBrother)
	{
		up_textview_pattern_destroy(pNode->m_pBrother, pGrc);
	}
	if (pNode->m_pChild)
	{
		up_textview_pattern_destroy(pNode->m_pChild, pGrc);
	}

	zero_free(pPattern);
}


void up_textview_pattern_build_automation(t_pattern_ac* pPattern)
{
	t_node* pNode = (t_node*)pPattern;

	t_waitlist* pWaitList = up_textview_waitlist_create();

	while(pNode)
	{
		t_node* pCurNode = pNode->m_pChild;

		while(pCurNode)
		{
			if(NULL == pCurNode->m_pLabel)
			{
				t_node* pFailNode = pNode->m_pFail;

				while(pFailNode)
				{
					t_node* pMatchNode = pFailNode->m_pChild;

					while(pMatchNode)
					{
						if((pMatchNode->m_tCode) == (pCurNode->m_tCode))
						{
							pCurNode->m_pFail = pMatchNode;
							break;
						}
						pMatchNode = pMatchNode->m_pBrother;
					}

					if(pMatchNode)
					{
						break;
					}

					pFailNode = pFailNode->m_pFail;
				}

				if(NULL == pFailNode)
				{
					pCurNode->m_pFail = (t_node*)pPattern;
				}

				up_textview_waitlist_put_in(pWaitList, pCurNode);
			}

			pCurNode = pCurNode->m_pBrother;
		}

		pNode = up_textview_waitlist_get_out(pWaitList);
	}

	up_textview_waitlist_destroy(pWaitList);
}