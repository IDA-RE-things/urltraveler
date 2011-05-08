/*
** 2006 Oct 10
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** Implementation of the "simple" full-text-search tokenizer.
*/

/*
** The code in this file is only compiled if:
**
**     * The FTS3 module is being built as an extension
**       (in which case SQLITE_CORE is not defined), or
**
**     * The FTS3 module is being built into the core of
**       SQLite (in which case SQLITE_ENABLE_FTS3 is defined).
*/
#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_FTS3)


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "fts3_tokenizer.h"

typedef struct simple_tokenizer {
  sqlite3_tokenizer base;
  char delim[128];             /* flag ASCII delimiters */

  // ���ķָ��� [7/8/2008 �»���]
  char acGB2312Delim[256][256];
} simple_tokenizer;

typedef struct simple_tokenizer_cursor {
  sqlite3_tokenizer_cursor base;
  const char *pInput;          /* input we are tokenizing */
  int nBytes;                  /* size of the input */
  int iOffset;                 /* current position in pInput */
  int iToken;                  /* index of next token to be returned */
  char *pToken;                /* storage for current token */
  int nTokenAllocated;         /* space allocated to zToken buffer */
} simple_tokenizer_cursor;


/* Forward declaration */
static const sqlite3_tokenizer_module simpleTokenizerModule;

static int simpleDelim(simple_tokenizer *t, unsigned char c){
  return c<0x80 && t->delim[c];
}

/**�ж����ı����ŵȷָ���
���ı������������ֽ����,��˴��������������ַ�
[7/8/2008 �»���]
*/
static int gb2312Delim(simple_tokenizer *t, unsigned char c1, unsigned char c2){
	///return c1>=0x80 && c2>=0x80 && t->acGB2312Delim[c1][c2];
	///return c1>=0xa1 && c2>=0xa1 && t->acGB2312Delim[c1][c2];
	///return isGB2312Word(t, c1, c2) && t->acGB2312Delim[c1][c2];
	return t->acGB2312Delim[c1][c2];
}

/**�ж��Ƿ���
�����������ֽ����,��˴��������������ַ�
GB2312�ı��뷶Χ��0xA1A1-0xF7FE
[7/8/2008 �»���]
*/
static int isGB2312Word(simple_tokenizer *t, unsigned char c1, unsigned char c2){
	return c1 >= 0xa1 && c1 <= 0xf7 && c2 >= 0xa1 && c2 <= 0xfe;
}

/*
** Create a new tokenizer instance.
*/
static int simpleCreate(
  int argc, const char * const *argv,
  sqlite3_tokenizer **ppTokenizer
){
  simple_tokenizer *t;

  t = (simple_tokenizer *) sqlite3_malloc(sizeof(*t));
  if( t==NULL ) return SQLITE_NOMEM;
  memset(t, 0, sizeof(*t));

  /* TODO(shess) Delimiters need to remain the same from run to run,
  ** else we need to reindex.  One solution would be a meta-table to
  ** track such information in the database, then we'd only want this
  ** information on the initial create.
  */
  if( argc>1 ){
    int i, n = strlen(argv[1]);
    for(i=0; i<n; i++){
      unsigned char ch = argv[1][i];
      /* We explicitly don't support UTF-8 delimiters for now. */
      if( ch>=0x80 ){
        sqlite3_free(t);
        return SQLITE_ERROR;
      }
      t->delim[ch] = 1;
    }
  } else {
    /* Mark non-alphanumeric ASCII characters as delimiters */
    int i;
    for(i=1; i<0x80; i++){
      t->delim[i] = !isalnum(i);
    }
  }

  /** ���ı����������ʼ�� 
  ���裺�������ŵ�GB2312����Ϊ200��200�����ֽڵ���ɣ���t->acGB2312Delim[200][200]����Ϊ1��
  [7/8/2008 �»���]
  */
  {
		unsigned char aStrGB2312Delim[][2] = {
			"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", 
			"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", 
			"��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
			"��", "����", "��", "����"
			
		};

		int iaStrGB2312DelimLen = sizeof(aStrGB2312Delim)/sizeof(aStrGB2312Delim[0]);
		int i = 0;
		for (i=0; i< iaStrGB2312DelimLen; i++)
		{
			t->acGB2312Delim[aStrGB2312Delim[i][0]][aStrGB2312Delim[i][1]] = 1;
		}
  }

  *ppTokenizer = &t->base;
  return SQLITE_OK;
}

/*
** Destroy a tokenizer
*/
static int simpleDestroy(sqlite3_tokenizer *pTokenizer){
  sqlite3_free(pTokenizer);
  return SQLITE_OK;
}

/*
** Prepare to begin tokenizing a particular string.  The input
** string to be tokenized is pInput[0..nBytes-1].  A cursor
** used to incrementally tokenize this string is returned in 
** *ppCursor.
*/
static int simpleOpen(
  sqlite3_tokenizer *pTokenizer,         /* The tokenizer */
  const char *pInput, int nBytes,        /* String to be tokenized */
  sqlite3_tokenizer_cursor **ppCursor    /* OUT: Tokenization cursor */
){
  simple_tokenizer_cursor *c;

  c = (simple_tokenizer_cursor *) sqlite3_malloc(sizeof(*c));
  if( c==NULL ) return SQLITE_NOMEM;

  c->pInput = pInput;
  if( pInput==0 ){
    c->nBytes = 0;
  }else if( nBytes<0 ){
    c->nBytes = (int)strlen(pInput);
  }else{
    c->nBytes = nBytes;
  }
  c->iOffset = 0;                 /* start tokenizing at the beginning */
  c->iToken = 0;
  c->pToken = NULL;               /* no space allocated, yet. */
  c->nTokenAllocated = 0;

  *ppCursor = &c->base;
  return SQLITE_OK;
}

/*
** Close a tokenization cursor previously opened by a call to
** simpleOpen() above.
*/
static int simpleClose(sqlite3_tokenizer_cursor *pCursor){
  simple_tokenizer_cursor *c = (simple_tokenizer_cursor *) pCursor;
  sqlite3_free(c->pToken);
  sqlite3_free(c);
  return SQLITE_OK;
}

/*
** Extract the next token from a tokenization cursor.  The cursor must
** have been opened by a prior call to simpleOpen().
*/
static int simpleNext(
  sqlite3_tokenizer_cursor *pCursor,  /* Cursor returned by simpleOpen */
  const char **ppToken,               /* OUT: *ppToken is the token text */
  int *pnBytes,                       /* OUT: Number of bytes in token */
  int *piStartOffset,                 /* OUT: Starting offset of token */
  int *piEndOffset,                   /* OUT: Ending offset of token */
  int *piPosition                     /* OUT: Position integer of token */
){
  simple_tokenizer_cursor *c = (simple_tokenizer_cursor *) pCursor;
  simple_tokenizer *t = (simple_tokenizer *) pCursor->pTokenizer;
  unsigned char *p = (unsigned char *)c->pInput;

  while( c->iOffset<c->nBytes ){
    int iStartOffset;
	
	// ��־�Ƿ�������gb2312�ķָ��� [7/9/2008 �»���]
	int iPastgb2312DelimFlag = 0;		
	// ��־�´��α���Ҫ���˶��ٸ��ֽ� [7/9/2008 �»���]
	int iBackPos = 0;
	// ��־���ηִ����Ƿ������ascii���ַ�
	int iIncludeAsciiChar = 0;
	// ��־���ηִ����Ƿ������gb2312���ַ�
	int iIncludeGB2312Char = 0;
	
	do 
	{
		iPastgb2312DelimFlag = 0;
		/* Scan past delimiter characters */
		while(    c->iOffset<c->nBytes && simpleDelim(t, p[c->iOffset]) ){
			c->iOffset++;
		}
#if 1
		/** Scan past chinese delimiter characters such as: ��������/=+ etc
		��Ϊ���ķָ������������뻹�������ַ�����
		[7/8/2008 �»���]
		*/
		while((c->iOffset + 1)< c->nBytes && gb2312Delim(t, p[c->iOffset], p[c->iOffset+1]) )
		{
			c->iOffset += 2;
			iPastgb2312DelimFlag = 1;
		}
#endif
	} while (iPastgb2312DelimFlag);

	//  [7/9/2008 �»���]
#if 1
	/* Count non-delimiter characters. */
	iStartOffset = c->iOffset;

	while( c->iOffset<c->nBytes && !simpleDelim(t, p[c->iOffset])
		&& !((c->iOffset + 1)< c->nBytes && gb2312Delim(t, p[c->iOffset], p[c->iOffset+1]))			
		)
	{
		/** �������
		���Ϊ�������������������������ҵڶ����������ı����ţ��������������һ����
		���Ϊ���ĵ�ֻ��һ������������,��һ���������һ�������Ĵ�
		[7/9/2008 �»���]
		*/
		if ((c->iOffset + 1)< c->nBytes 
			&& isGB2312Word(t, p[c->iOffset], p[c->iOffset+1])
			)
		{
			// �����ηִ��Ѿ�������ascii�룬��������һ��Ϊgb2312�ַ���Ҫ����һ��ȴ��´α���ֱ��break [7/9/2008 �»���]
			if (iIncludeAsciiChar)
			{
				break;
			}
			
			if ((c->iOffset + 3)< c->nBytes 
				&& isGB2312Word(t, p[c->iOffset+2], p[c->iOffset+3]) 
				&& !gb2312Delim(t, p[c->iOffset+2], p[c->iOffset+3])
				)
			{
				// �����������һ����,���ڴ�ʱ����4���ֽڣ��´ηִ�Ҫ������2���ֽ� [7/9/2008 �»���]
				c->iOffset += 4;
				iBackPos = 2;	
				break;
			}
			else
			{
				// ��ʱֻ��һ�������ĺ���,��Ϊ�����ִ� [7/9/2008 �»���]
				c->iOffset += 2;
				break;
			}

			iIncludeGB2312Char = 1;
		}
		else
		{
			c->iOffset++;
			iIncludeAsciiChar = 1;
		}
	}
#else
	/* Count non-delimiter characters. */
	iStartOffset = c->iOffset;
	while( c->iOffset<c->nBytes && !simpleDelim(t, p[c->iOffset]) ){
		c->iOffset++;
	}
#endif

    if( c->iOffset>iStartOffset ){
      int i, n = c->iOffset-iStartOffset;
      if( n>c->nTokenAllocated ){
        c->nTokenAllocated = n+20;
        c->pToken = sqlite3_realloc(c->pToken, c->nTokenAllocated);
        if( c->pToken==NULL ) return SQLITE_NOMEM;
      }
      for(i=0; i<n; i++){
        /* TODO(shess) This needs expansion to handle UTF-8
        ** case-insensitivity.
        */
        unsigned char ch = p[iStartOffset+i];
        c->pToken[i] = ch<0x80 ? tolower(ch) : ch;
      }
      *ppToken = c->pToken;
      *pnBytes = n;
      *piStartOffset = iStartOffset;
      *piEndOffset = c->iOffset;
      *piPosition = c->iToken++;
	
	  //Ҫ����iBackPos���ֽ� [7/9/2008 �»���]
	  c->iOffset -= iBackPos;

      return SQLITE_OK;
    }
  }
  return SQLITE_DONE;
}

/*
** The set of routines that implement the simple tokenizer
*/
static const sqlite3_tokenizer_module simpleTokenizerModule = {
  0,
  simpleCreate,
  simpleDestroy,
  simpleOpen,
  simpleClose,
  simpleNext,
};

/*
** Allocate a new simple tokenizer.  Return a pointer to the new
** tokenizer in *ppModule
*/
void sqlite3Fts3SimpleTokenizerModule(
  sqlite3_tokenizer_module const**ppModule
){
  *ppModule = &simpleTokenizerModule;
}

#endif /* !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_FTS3) */
