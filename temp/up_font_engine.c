#include "up_gdi.h"
#include "up_ucstring.h"
#include "up_string.h"
#include "up_file.h"
#include "up_memory.h"
#include "up_mru.h"

#define UP_FONT_ENGINE_GLYPH_SIZE ()
#define UP_FONT_ENGINE_CACHE_SIZE (100)
#define UP_FONT_ENGINE_DEFAULT_CODE ()

#define UP_FONT_ENGINE_SIZE_OF_HEAD (28)
#define UP_FONT_ENGINE_SIZE_OF_MAP_NODE (8)
#define UP_FONT_ENGINE_SIZE_OF_GLYPH_HEAD (28)

typedef unsigned short t_unicode;


typedef struct  
{
	t_mru_node m_tParent;
	t_unicode m_tCode;

	int m_iBX;
	int m_iBY;
	int m_iBW;
	int m_iBH;

	int m_iDW;

	char* m_pBitmap;
}t_glyph_node;

typedef struct map_node
{
	t_unicode m_tCode;
	int m_iGlyphId;
}t_map_node;

typedef struct  
{
	t_map_node* m_pNodes;
	int m_iCount;

	int* m_pHashTable;
	int m_iHashCount;
}t_glyph_map;

typedef struct  
{
	t_mru_list_clazz m_tMruClazz;
	t_mru_list m_tMruList;

	int m_iGlyphCount;
	int m_iGlyphSize;
	int m_height;
	t_unicode m_tDefaultCode;

	int m_hFile;
}t_font_engine;


static int up_font_engine_load_buffer(t_font_engine* pFontEngine, unsigned long ulOff, unsigned char* pucBuffer, int iSize)
{
	int iRet = 0;

	if ((pFontEngine->m_hFile != -1) && Up_FSeek(pFontEngine->m_hFile, ulOff, UP_FS_FILE_BEGIN))
	{
		iRet = Up_FRead(pFontEngine->m_hFile, pucBuffer, iSize);
	}

	return iRet;
}

static int up_font_engine_load_head(t_font_engine* pFontEngine)
{
	unsigned char pucBuffer[UP_FONT_ENGINE_SIZE_OF_MAP_NODE];
	int iIndex = 0;

	up_font_engine_load_buffer(pFontEngine, 0, pucBuffer, UP_FONT_ENGINE_SIZE_OF_MAP_NODE);

	return iIndex;
}

static int up_font_engine_load_map_node(t_font_engine* pFontEngine, int iMapIndex, t_map_node* pMapNode)
{
	unsigned char pucBuffer[UP_FONT_ENGINE_SIZE_OF_HEAD];
	int iIndex = 0;

	up_font_engine_load_buffer(pFontEngine, 0, pucBuffer, UP_FONT_ENGINE_SIZE_OF_HEAD);

	return iIndex;
}

static int up_font_engine_load_glyph_node(t_font_engine* pFontEngine, int iGlyphIndex, t_glyph_node* pGlyphNode)
{
	unsigned char pucBuffer[UP_FONT_ENGINE_SIZE_OF_GLYPH_HEAD];
	int iIndex = 0;

	up_font_engine_load_buffer(pFontEngine, 0, pucBuffer, UP_FONT_ENGINE_SIZE_OF_GLYPH_HEAD);

	return iIndex;
}

static void up_font_engine_release_glyph_node(t_font_engine* pFontEngine, t_glyph_node* pGlyphNode)
{
	if (pGlyphNode->m_pBitmap)
	{
		zero_free(pGlyphNode->m_pBitmap);
	}
}

static int up_font_engine_get_index(t_font_engine* pFontEngine, t_unicode tCode)
{
	//TODO
	return 0;
}


static void up_font_engine_load_glyph_by_unicode(t_font_engine* pFontEngine, t_unicode tCode, t_glyph_node* pGlyphNode)
{
	int iGlyphIndex = up_font_engine_get_index(pFontEngine, tCode);

	if (iGlyphIndex < 0)
	{
		iGlyphIndex = up_font_engine_get_index(pFontEngine, pFontEngine->m_tDefaultCode);
	}

	up_font_engine_load_glyph_node(pFontEngine, iGlyphIndex, pGlyphNode);
}

static t_glyph_node* up_font_engine_get_glyph(t_font_engine* pFontEngine, t_unicode tCode)
{
	t_glyph_node* pGlyphNode = NULL;

	Up_MruList_Lookup(&(pFontEngine->m_tMruList), (void*)tCode, &pGlyphNode);

	return pGlyphNode;
}

static int up_glyphnode_compare(t_mru_node* pNode, void* pKey)
{
	t_glyph_node* pGlyphNode = (t_glyph_node*)pNode;
	t_unicode tCode = (t_unicode)pKey;

	return (pGlyphNode->m_tCode) - tCode;
}

static void up_glyphnode_init(t_mru_node* pNode, void* pKey, void* pData)
{
	t_glyph_node* pGlyphNode = (t_glyph_node*)pNode;
	t_unicode tCode = (t_unicode)pKey;
	t_font_engine* pFontEngine = (t_font_engine*)pData;

	pGlyphNode->m_tCode = tCode;

	up_font_engine_load_glyph_by_unicode(pFontEngine, tCode, pGlyphNode);
}

static void up_glyphnode_reset(t_mru_node* pNode, void* pKey, void* pData)
{
	t_glyph_node* pGlyphNode = (t_glyph_node*)pNode;
	t_unicode tCode = (t_unicode)pKey;
	t_font_engine* pFontEngine = (t_font_engine*)pData;

	pGlyphNode->m_tCode = tCode;

	up_font_engine_load_glyph_by_unicode(pFontEngine, tCode, pGlyphNode);
}

static void up_glyphnode_done(t_mru_node* pNode, void* pData)
{
	t_glyph_node* pGlyphNode = (t_glyph_node*)pNode;
	t_font_engine* pFontEngine = (t_font_engine*)pData;

	up_font_engine_release_glyph_node(pFontEngine, pGlyphNode);
}

t_font_engine* up_font_engine_init()
{
	t_font_engine* pFontEngine = zero_alloc(sizeof(t_font_engine));

	//TODO

	up_font_engine_load_head(pFontEngine);

	pFontEngine->m_tMruClazz.m_fCompare = (Up_MruNode_CompareFunc)up_glyphnode_compare;
	pFontEngine->m_tMruClazz.m_fInit = (Up_MruNode_InitFunc)up_glyphnode_init;
	pFontEngine->m_tMruClazz.m_fDone = (Up_MruNode_DoneFunc)up_glyphnode_done;
	pFontEngine->m_tMruClazz.m_fReset = (Up_MruNode_ResetFunc)up_glyphnode_reset;
	pFontEngine->m_tMruClazz.m_iNodeSize = sizeof(t_glyph_node);

	Up_MruList_Init(&(pFontEngine->m_tMruList), &(pFontEngine->m_tMruClazz), UP_FONT_ENGINE_CACHE_SIZE, pFontEngine);
	
	return pFontEngine;
}

void up_font_engine_deinit(t_font_engine* pFontEngine)
{
	Up_MruList_Done(&(pFontEngine->m_tMruList));

	//TODO
}


int up_font_engine_get_char_width(t_font_engine* pFontEngine, unsigned long ulChar)
{
	t_glyph_node* pGlyph = up_font_engine_get_glyph(pFontEngine, ulChar);

	return (pGlyph) ? pGlyph->m_iDW : 0;
}

int up_font_engine_get_height(t_font_engine* pFontEngine)
{
	return pFontEngine->m_height;
}