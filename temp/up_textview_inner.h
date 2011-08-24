#ifndef _UP_TEXTVIEW_INNER_H_
#define _UP_TEXTVIEW_INNER_H_

#include "up_APi.h"
#include "up_app_ext.h"

#define UP_TEXTVIEW_DEBUG_FLAG 1

#if UP_TEXTVIEW_DEBUG_FLAG
#	define UP_TEXTVIEW_DEBUG_CHECK_RET(exp) if(NULL == (exp)){return;}
#	define UP_TEXTVIEW_DEBUG_CHECK_RET_ZERO(exp) if(NULL == (exp)){return 0;}
#	define UP_TEXTVIEW_DEBUG_CHECK_RET_NULL(exp) if(NULL == (exp)){return NULL;}
#else
#	define UP_TEXTVIEW_DEBUG_CHECK_RET(exp) Up_Assert(exp)
#	define UP_TEXTVIEW_DEBUG_CHECK_ZERO(exp) Up_Assert(exp)
#	define UP_TEXTVIEW_DEBUG_CHECK_RET_NULL(exp) Up_Assert(exp)
#endif

#define UP_TEXTVIEW_DEBUG_UNUSE(exp) ((void)(exp))


typedef enum
{
	UP_TEXTVIEW_AUTO_HEIGHT_UP,
	UP_TEXTVIEW_AUTO_HEIGHT_DOWN,
	UP_TEXTVIEW_AUTO_HEIGHT_CENTER
}e_auto_height;

typedef struct textview_setup 
{
	int m_bMultiParagraph;
	int m_bAutoLineBreak;
	int m_bDialMode;
	int m_bPasswordMode;
	int m_iLimitedSize;
	e_auto_height m_eAutoHeight;

	int m_bSelectable;
	int m_bEditable;
}t_textview_setup;

typedef unsigned short t_code;

typedef struct  
{
	t_code* m_pCode;
	int m_iSize;
}t_candidate_code;

////////////////////////////////////////////////////////////////

typedef void (*SetClip_fxp)(void* pHandle, Rect tRect);
typedef void (*SetMode_fxp)(void* pHandle, int bXor);

typedef void* (*CreateLabel_fxp)(void* pHandle, void* pData);
typedef void (*DestroyLabel_fxp)(void* pHandle, void* pLabel);

typedef unsigned long (*GetCharHeight_fxp)(void* pHandle);
typedef unsigned long (*GetLabelWidth_fxp)(void* pHandle, void* pLabel);
typedef unsigned long (*GetCharWidth_fxp)(void* pHandle, t_code tCode);
typedef unsigned long (*GetUnicodesWidth_fxp)(void* pHandle, t_code* pCodes, int iSize);
typedef unsigned long (*GetUnicodesOffsize_fxp)(void* pHandle, t_code* pCodes, int iSize, unsigned long ulLength, int* pCurWidth, int* pLastWidth);

typedef void (*FillRect_fxp)(void* pHandle, Rect tRect);
typedef void (*DrawRect_fxp)(void* pHandle, Rect tRect);
typedef void (*DrawShadowRect_fxp)(void* pHandle, Rect tRect, Color tColorA, Color tColorB);
typedef void (*DrawUnicodes_fxp)(void* pHandle, t_code* pCodes, int iSize, int iX, int iY);
typedef void (*DrawLabel_fxp)(void* pHandle, void* pLabel, int iX, int iY);

typedef void (*SetColor_fxp)(void* pHandle, Color tColor);
typedef void (*SetLineWidth_fxp)(void* pHandle, int iWidth);


typedef struct graphics_adaptor
{
	void* m_pHandle;

	CreateLabel_fxp m_fCreateLabel;
	DestroyLabel_fxp m_fDestroyLabel;

	GetCharHeight_fxp m_fGetCharHeight;
	GetLabelWidth_fxp m_fGetLabelWidth;
	GetCharWidth_fxp m_fGetCharWidth;
	GetUnicodesWidth_fxp m_fGetUnicodesWidth;
	GetUnicodesOffsize_fxp m_fGetUnicodesOffsize;
}t_graphics_adaptor;

typedef struct render_adaptor
{
	void* m_pHandle;

	SetClip_fxp m_fSetClip;
	SetMode_fxp m_fSetMode;

	FillRect_fxp m_fFillRect;
	DrawRect_fxp m_fDrawRect;
	DrawShadowRect_fxp m_fDrawShadowRect;
	DrawUnicodes_fxp m_fDrawUnicodes;
	DrawLabel_fxp m_fDrawLabel;

	SetColor_fxp m_fSetColor;
	SetLineWidth_fxp m_fSetLineWidth;
}t_render_adaptor;

////////////////////////////////////////////////////////////////

typedef enum
{
	UP_TEXTVIEW_IME_KEY_DOWN,
	UP_TEXTVIEW_IME_KEY_UP
}e_ime_key_type;

typedef enum
{
	UP_TEXTVIEW_IME_TOUCH_DOWN,
	UP_TEXTVIEW_IME_TOUCH_MOVE,
	UP_TEXTVIEW_IME_TOUCH_UP
}e_ime_touch_type;

typedef int (*GetCount_fxp)(void* pHandle);
typedef t_code* (*GetByIndex_fxp)(void* pHandle, int iIndex, int* pSize);
typedef void (*Select_fxp)(void* pHandle, int iIndex);
typedef void (*Abandon_fxp)(void* pHandle);

typedef struct candidate_adaptor
{
	void* m_pHandle;

	GetCount_fxp m_fGetCount;
	GetByIndex_fxp m_fGetByIndex;
	Select_fxp m_fSelect;
	Abandon_fxp m_fAbandon;
}t_candidate_adaptor;


typedef struct ime_state_info
{
	t_code* m_pImeInfo;
	void* m_pIcon;
}t_ime_state_info;

typedef void (*InsertCode_fxp)(void* pHandle, t_code tCode);
typedef void (*CommitText_fxp)(void* pHandle, t_code* pText, int iSize);
typedef void (*ComposeText_fxp)(void* pHandle, t_code* pText, int iSize, t_candidate_adaptor* pCandidatesAdaptor);
typedef void (*DeleteNext_fxp)(void* pHandle);
typedef void (*DeletePre_fxp)(void* pHandle);
typedef void (*UpdateState_fxp)(void* pHandle, t_ime_state_info* pState);

typedef struct ime_listener
{
	void* m_pHandle;

	InsertCode_fxp m_fInsertCode;
	CommitText_fxp m_fCommitText;
	ComposeText_fxp m_fComposeText;
	DeleteNext_fxp m_fDeleteNext;
	DeletePre_fxp m_fDeletePre;
	UpdateState_fxp m_fUpdateState;
}t_ime_listener;



//////////////////////////////////////////

typedef void (*Start_fxp)(void* pHandle, t_ime_listener* pListener);
typedef void (*Stop_fxp)(void* pHandle);
typedef int (*ProcessKeyEvent_fxp)(void* pHandle, e_ime_key_type eKeyType, unsigned long ulKey);
typedef int (*ProcessTouchEvent_fxp)(void* pHandle, e_ime_touch_type eTouchType, int iX, int iY);

typedef struct input_engine
{
	void* m_pHandle;

	Start_fxp m_fStart;
	Stop_fxp m_fStop;
	ProcessKeyEvent_fxp m_fProcessKey;
	ProcessKeyEvent_fxp m_fTouchKey;
}t_input_engine;

typedef t_input_engine* (*CreateIme_fxp)(void* pSetup, t_ime_listener* pListener);
typedef void (*DestroyIme_fxp)(t_input_engine* pIme);

typedef struct input_engine_setup
{
	void* m_pSetup;

	CreateIme_fxp m_fCreateIme;
	DestroyIme_fxp m_fDestroyIme;
}t_input_engine_setup;



////////////////////////////////////////////////////////////////

typedef void (*TriggerKeyEvent_fxp)(void* pHandle, e_ime_key_type eKeyType, unsigned long ulKey);

typedef struct keyboard_listener
{
	void* m_pHandle;

	TriggerKeyEvent_fxp m_fTriggerKeyEvent;
}t_keyboard_listener;

typedef struct ime_listener_node
{
	t_ime_listener* m_pListener;
	struct ime_listener_node* m_pNext;
}t_ime_listener_node;

typedef struct input_manager_core
{
	int m_iCurIndex;
	int m_iEnginesCount;
	t_input_engine** m_pEngines;

	t_ime_listener m_tListener;

	t_ime_listener_node* m_pListeners;
	int m_iListenersCount;
}t_input_manager_core;


typedef struct input_manager
{
	Window* m_pRefWindow;
	Rect m_tRefRect;
	Rect m_tSoftKeyboardRect;

	Control* m_pStateControl;
	Control* m_pCandidateControl;
	Control* m_pSoftKeyboardControl;

	int m_bRegistMenubar;
//////////
	t_keyboard_listener m_tKeyboardListener;

	t_ime_listener m_tListener;
	t_ime_listener* m_pListener;

	t_candidate_adaptor m_tAdaptor;
	t_candidate_adaptor* m_pAdaptor;

	unsigned long m_ulSwitchKey;

	int m_bRun;
}t_input_manager;


t_input_manager* up_input_manager_create(Window* pWindow, Rect tStateRect, t_ime_listener* pListener);
void up_input_manager_destroy(t_input_manager* pManager);

void up_input_manager_set_reference(t_input_manager* pManager, int iRefX, int iRefTopY, int iRefBotY);

void up_input_manager_custom(t_input_manager* pManager, t_code* pInfo);

void up_input_manager_softkeyboard_show(t_input_manager* pManager);

void up_input_manager_show(t_input_manager* pManager);
void up_input_manager_hide(t_input_manager* pManager);

int up_input_manager_process_key_event(t_input_manager* pManager, e_ime_key_type eKeyType, unsigned long ulKey);
int up_input_manager_process_touch_event(t_input_manager* pManager, e_ime_touch_type eTouchType, int iX, int iY);


////////////////////////////////////////////////////////////////

typedef struct node
{
	t_code m_tCode;

	void* m_pLabel;

	unsigned long m_ulDepth;

	struct node* m_pFail;
	struct node* m_pBrother;
	struct node* m_pChild;
}t_node;

typedef struct waitlistnode
{
	t_node* m_pNode;
	struct waitlistnode* m_pNext;
}t_waitlistnode;

typedef struct waitlist
{
	t_waitlistnode* m_pFirst;
	t_waitlistnode* m_pLast;
}t_waitlist;

t_node* up_textview_waitlist_get_out(t_waitlist* pList);
void up_textview_waitlist_put_in(t_waitlist* pList, t_node* pNode);
t_waitlist* up_textview_waitlist_create(void);
void up_textview_waitlist_destroy(t_waitlist* pList);

typedef t_node t_pattern_ac;


////////////////////////////////////////////////////////////////

typedef t_pattern_ac t_pattern;

typedef enum
{
	UP_TEXTVIEW_LABEL_EMOJI_ROM,
	UP_TEXTVIEW_LABEL_EMOJI_DISK,
	UP_TEXTVIEW_LABEL_EMOJI_PRE,
	UP_TEXTVIEW_LABEL_EMOJI_NONE
}e_textview_custom_label;

typedef struct textview_label_setup 
{
	e_textview_custom_label m_eType;

	void* m_pHandle;
}t_textview_label_setup;

typedef struct up_textview_custom_label 
{
	e_textview_custom_label m_eType;

	Image* m_pImage;
	unsigned long m_ulImageId;
}t_textview_custom_label;

t_pattern_ac* up_textview_pattern_add(t_pattern_ac* pPattern, t_code* pCodes, unsigned long ulSize, t_graphics_adaptor* pGrc, void* pData);
void up_textview_pattern_destroy(t_pattern_ac* pPattern, t_graphics_adaptor* pGrc);
void up_textview_pattern_build_automation(t_pattern_ac* pPattern);

////////////////////////////////////////////////////////////////
typedef enum
{
	UP_TEXTVIEW_BIDI_TYPE_L,
	UP_TEXTVIEW_BIDI_TYPE_LRE,
	UP_TEXTVIEW_BIDI_TYPE_LRO,
	UP_TEXTVIEW_BIDI_TYPE_R,
	UP_TEXTVIEW_BIDI_TYPE_AL,
	UP_TEXTVIEW_BIDI_TYPE_RLE,
	UP_TEXTVIEW_BIDI_TYPE_RLO,
	UP_TEXTVIEW_BIDI_TYPE_PDF,
	UP_TEXTVIEW_BIDI_TYPE_EN,
	UP_TEXTVIEW_BIDI_TYPE_ES,
	UP_TEXTVIEW_BIDI_TYPE_ET,
	UP_TEXTVIEW_BIDI_TYPE_AN,
	UP_TEXTVIEW_BIDI_TYPE_CS,
	UP_TEXTVIEW_BIDI_TYPE_NSM,
	UP_TEXTVIEW_BIDI_TYPE_BN,
	UP_TEXTVIEW_BIDI_TYPE_B,
	UP_TEXTVIEW_BIDI_TYPE_S,
	UP_TEXTVIEW_BIDI_TYPE_WS,
	UP_TEXTVIEW_BIDI_TYPE_ON
}e_textview_bidi_type;

#define UP_TEXTVIEW_BIDI_FIRST_RTL (0x0590)

void up_textview_bidi_do_mirror(t_code* pCodes, unsigned long ulCount);

int up_textview_bidi_do(t_code* pDisplayCodes, e_textview_bidi_type* pTypes, unsigned long ulTextCount);
int up_textview_bidi_shape(t_code *pLine, t_code *pResult, int iCount);

////////////////////////////////////////////////////////////////

typedef struct replacement 
{
	unsigned long m_ulStart;
	unsigned long m_ulEnd;

	void* m_pLabel;
}t_replacement;

typedef struct paragraph 
{
	unsigned long m_ulBreakPos;

	unsigned long* m_ulUnitBreaks;
	unsigned long m_ulUnitCount;
	unsigned long m_ulUnitBreaksSize;
}t_paragraph;

typedef struct rtl_span 
{
	unsigned long m_ulStart;
	unsigned long m_ulEnd;
}t_rtl_span;

typedef struct layout 
{
	unsigned long m_ulParagraphsSizeGuess;
	unsigned long m_ulUnitBreaksSizeGuess;
	unsigned long m_ulLineBreaksSizeGuess;
	unsigned long m_ulRtlSpanSizeGuess;
	unsigned long m_ulDirectionsSizeGuess;
	unsigned long m_ulReplacementsSizeGuess;

	t_code* m_pDisplayCodes;
	unsigned long m_ulCodesCount;

	t_replacement* m_pReplacements;
	unsigned long m_ulReplacementCount;
	unsigned long m_ulReplacementsSize;	

	t_paragraph* m_pParagraphs;
	unsigned long m_ulParagraphCount;
	unsigned long m_ulParagraphsSize;

	unsigned long* m_ulLineBreaks;
	unsigned long** m_ulDirections;
	unsigned long* m_ulDirectionsCount;
	unsigned long m_ulLineBreaksCount;
	unsigned long m_ulLineBreaksSize;

	t_rtl_span* m_pRtlSpan;
	unsigned long m_ulRtlSpanCount;
	unsigned long m_ulRtlSpanSize;

	int m_bRtl;
	int m_bSystemRtl;
}t_layout;

#define UP_TEXTVIEW_SHOW_UNDERLINE_WIDTH_DEFAULT (2)

typedef enum
{
	UP_TEXTVIEW_LAYOUT_HIGHLIGHT_NONE,
	UP_TEXTVIEW_LAYOUT_HIGHLIGHT_XORFILL,
	UP_TEXTVIEW_LAYOUT_HIGHLIGHT_UNDERLINE
}e_layout_highlight;


typedef struct layout_highlight 
{
	unsigned long m_ulStart;
	unsigned long m_ulEnd;
	e_layout_highlight m_eMode;
}t_layout_highlight;


void up_textview_layout_init(t_layout* pLayout);
void up_textview_layout_clean(t_layout* pLayout);
void up_textview_layout_generate(t_layout* pLayout, t_code* pText, unsigned long ulTextCodeCount, t_pattern* pPattern, unsigned long ulLineWidth, t_graphics_adaptor* pGrc);
void up_textview_layout_draw(t_layout* pLayout, t_layout_highlight tHighlight, Rect tRect, Point tPos, unsigned long ulLineHeight, t_graphics_adaptor* pGrc, t_render_adaptor* pRender);


////////////////////////////////////////////////////////////////


typedef struct span 
{
	unsigned long m_ulStart;
	unsigned long m_ulEnd;

	unsigned long m_ulStartOffsize;
	unsigned long m_ulStartLine;
	unsigned long m_ulEndOffsize;
	unsigned long m_ulEndLine;
}t_span;

unsigned long up_textview_span_generate(t_span* pSpan, t_layout* pLayout, int bAtStart, t_graphics_adaptor* pGrc);
void up_textview_span_guess(t_span* pSpan, t_layout* pLayout, unsigned long ulGuessLine, unsigned long ulGuessOffsize, t_graphics_adaptor* pGrc);

typedef struct seletion 
{
	t_span m_tSpan;

	int m_bAtStart;

	unsigned long m_ulGuessOffsize;
}t_seletion;

typedef t_span t_compose;


////////////////////////////////////////////////////////////////


typedef void (*UpdateText_fxp)(void* pHandle, unsigned long ulLineCount, unsigned long ulCaretAdvance, unsigned long ulCaretLine);
typedef void (*UpdateCaret_fxp)(void* pHandle, unsigned long ulCaretAdvance, unsigned long ulCaretLine);
typedef void (*Bingo_fxp)(void* pHandle);




typedef struct context_observer
{
	void* m_pHandle;

	Bingo_fxp m_fBingo;
	UpdateText_fxp m_fUpdateText;
	UpdateCaret_fxp m_fUpdateCaret;
}t_context_observer;


typedef enum
{
	UP_TEXTVIEW_CONTEXT_FLAG_MULTIPARAGRAPH = 0,
	UP_TEXTVIEW_CONTEXT_FLAG_AUTOLINEBREAK,
	UP_TEXTVIEW_CONTEXT_FLAG_ENABLEPATTERN,
	UP_TEXTVIEW_CONTEXT_FLAG_LIMITSIZE
}e_context_flag;

#define up_textview_context_set_flag(c,f) ((c)->m_ulFlag = (((c)->m_ulFlag) | (1 << (f))))
#define up_textview_context_unset_flag(c,f) ((c)->m_ulFlag = (((c)->m_ulFlag) & (~(1 << (f)))))
#define up_textview_context_get_flag(c,f) (((c)->m_ulFlag) & (1 << (f)))

typedef struct context
{
	t_graphics_adaptor* m_pGrc;

	unsigned long m_ulTextSizeGuess;

	t_context_observer* m_pObserver;

	unsigned long m_ulFlag;

	unsigned long m_ulLimitSize;
	unsigned long m_ulLineWidth;
	t_pattern* m_pPattern;

	t_code* m_pText;
	unsigned long m_ulTextCodeCount;
	unsigned long m_ulTextSize;

	t_layout m_tLayout;
	t_seletion m_tSeletion;
	t_compose m_tCompse;
}t_context;

t_context* up_textview_context_create(t_graphics_adaptor* pGrc, t_context_observer* pObserver, t_textview_setup* pSetup, unsigned long ulLineWidth);
void up_textview_context_destroy(t_context* pContext);

int up_textview_context_get_dir(t_context* pContext);

void up_textview_context_move_pre_line(t_context* pContext);
void up_textview_context_move_next_line(t_context* pContext);
void up_textview_context_move_pre_code(t_context* pContext);
void up_textview_context_move_next_code(t_context* pContext);
void up_textview_context_location_position(t_context* pContext, int iX, int iLine);
void up_textview_context_move_position(t_context* pContext, int iX, int iLine);

const unsigned short* up_textview_context_text(t_context* pContext);

void up_textview_context_clear_all(t_context* pContext);
void up_textview_context_delete_pre(t_context* pContext);
void up_textview_context_delete_next(t_context* pContext);
void up_textview_context_insert_code(t_context* pContext, t_code tCode);
void up_textview_context_compose_text(t_context* pContext, t_code* pCodes, int iSize, t_candidate_adaptor* pCandidatesAdaptor);
void up_textview_context_commit_text(t_context* pContext, t_code* pCodes, int iSize);
void up_textview_context_change_text(t_context* pContext, t_code* pCodes, int iSize);

void up_textview_context_content_draw(t_context* pContext, Rect tRect, Point tPos, unsigned long ulLineHeight, t_render_adaptor* pRender);
void up_textview_context_caret_draw(t_context* pContext, Rect tRect, Point tPos, unsigned long ulLineHeight, unsigned long ulCaretWidth, t_render_adaptor* pRender);

void up_textview_context_pattern_enable(t_context* pContext);
void up_textview_context_pattern_disable(t_context* pContext);
void up_textview_context_pattern_add(t_context* pContext, const unsigned short* pUnicodeString, int iSize, void* pData);

//more for config

////////////////////////////////////////////////////////////////
typedef void (*GiveUpFocus_fxp)(void* pHandle);
typedef Rect (*GetArea_fxp)(void* pHandle);
typedef void (*SetArea_fxp)(void* pHandle, Rect tRect);
typedef void (*RedrawArea_fxp)(void* pHandle, Rect tRect);
typedef void (*Redraw_fxp)(void* pHandle);

typedef struct view_observer
{
	void* m_pHandle;

	GiveUpFocus_fxp m_fGiveUpFocus;
	GetArea_fxp m_fGetArea;
	SetArea_fxp m_fSetArea;
	RedrawArea_fxp m_fRedrawArea;
	Redraw_fxp m_fRedraw;
}t_view_observer;


typedef struct view
{
	t_graphics_adaptor* m_pGrc;

	t_view_observer* m_pObserver;

	t_context_observer m_tContextObserver;
	t_context* m_pContext;

	void* m_pBgSelectedLabel;
	void* m_pBgUnSelectedLabel;

	t_ime_listener m_tImeListener;
	t_input_manager* m_pImm;

	unsigned long m_ulDefaultHeight;
	unsigned long m_ulMaxHeight;

	unsigned long m_ulLineWidth;
	unsigned long m_ulLineHeight;

	unsigned long m_ulContentHeight;
	unsigned long m_ulCurHeight;

	unsigned long m_ulCaretAdvance;
	unsigned long m_ulCaretLine;

	unsigned long m_ulScrollXPos;
	unsigned long m_ulScrollYPos;

	unsigned long m_ulTempHeight;

	unsigned long m_ulBorderWidth;
	unsigned long m_ulCaretWidth;

	int m_eAutoHeight;
	int m_bReadyLossControl;
	int m_bHasFocus;

	int m_bDialMode;

	int m_iYOffsize;
}t_view;

#define UP_TEXTVIEW_VIEW_SHOW_BORDER (0x1)

void up_textview_view_set_maxy(t_view* pView, int iMaxY);

t_view* up_textview_view_create(t_graphics_adaptor* pGrc, t_view_observer* pObserver, Window* pWindow, Rect tStateRect, t_textview_setup* pSetup);
void up_textview_view_destroy(t_view* pView);

void up_textview_view_clear(t_view* pView);
void up_textview_view_insert(t_view* pView, const unsigned short* pUnicodeString, int iSize);
const unsigned short* up_textview_view_text(t_view* pView);

void up_textview_view_update_text(t_view* pView, unsigned long ulLineCount, unsigned long ulCaretAdvance, unsigned long ulCaretLine);
void up_textview_view_update_caret(t_view* pView, unsigned long ulCaretAdvance, unsigned long ulCaretLine);
void up_textview_view_bingo(t_view* pView);

void up_textview_view_update_state(t_view* pView, int bHasFocus);
void up_textview_view_draw(t_view* pView, unsigned long ulOpt, t_render_adaptor* pRender);

int up_textview_view_process_key_event(t_view* pView, e_ime_key_type eKeyType, unsigned long ulKey);
int up_textview_view_process_touch_event(t_view* pView, e_ime_touch_type eTouchType, int iX, int iY);

void up_textview_view_background(t_view* pView, void* pSelected, void* pUnSelected);

void up_textview_view_dial_mode(t_view* pView, int bEnable);

void up_textview_view_pattern_enable(t_view* pView);
void up_textview_view_pattern_disable(t_view* pView);
void up_textview_view_pattern_add(t_view* pView, const unsigned short* pUnicodeString, int iSize, void* pData);

#define UP_TEXTVIEW_DICT_CODE_MAX (10)

typedef struct ime_dict_result_rt
{
	t_code m_pCode[UP_TEXTVIEW_DICT_CODE_MAX];
	int m_iSize;
	unsigned short usDepth;
}t_ime_dict_result_rt;

typedef struct ime_dict_element_rt
{
	t_code* m_pCodes;
	int m_iSize;
}t_ime_dict_element_rt;

typedef struct ime_dict_node_rt
{
	t_code m_tCode;

	unsigned short usDepth;

	unsigned long m_ulChildrenCount;
	struct ime_dict_node_rt* m_pChildren;

	t_ime_dict_element_rt* m_pElements;
	int m_iElementsCount;
}t_ime_dict_node_rt;

typedef struct ime_dict_rt
{
	t_ime_dict_node_rt* m_pRoot;
}t_ime_dict_rt;

typedef struct ime_dict_result
{
	t_code m_pCode[10];
	int m_iSize;
	unsigned short usDepth;
}t_ime_dict_result;

typedef struct ime_dict_element
{
	unsigned long m_ulCodesIndex;
	int m_iSize;
}t_ime_dict_element;

typedef struct ime_dict_node
{
	t_code m_tCode;

	unsigned short usDepth;

	unsigned long m_ulChildren;
	unsigned long m_ulChildrenCount;

	unsigned long m_ulElementsIndex;
	int m_iElementsCount;
}t_ime_dict_node;

typedef struct ime_dict
{
	unsigned long m_ulMaxDepth;

	t_ime_dict_node* m_pNodes;
	int m_iNodesCount;

	t_ime_dict_element* m_pElements;
	int m_iElementsCount;

	t_code* m_pCodes;
	int m_iCodesCount;
}t_ime_dict;


typedef struct ime_dict_handle
{
	t_ime_dict m_tDict;

	void* m_pSource;
}t_ime_dict_handle;


t_ime_dict_handle* up_textview_dict_handle_create(void* pSource);

void up_textview_dict_handle_destroy(t_ime_dict_handle* pDictHandle);

int up_textview_dict_search(t_ime_dict_handle* pDictHandle, t_code* pCodes, int iCodesSize, t_ime_dict_result* pResults, int iResultsSize);




#endif