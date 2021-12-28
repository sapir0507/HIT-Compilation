#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "token.h"
// <id,type,kind>

typedef enum type { INTEGER, FLOAT, VOID, ERROR }Type;
typedef enum action { TO_USE, TO_DEFINE }Action;
enum kind { variable, function, parameter };

typedef struct symbol {
	const char* id;
	enum type type;
	int size; // size or num parameters
	enum kind kind;
}symbol;

void initializeSymbol(struct symbol* symbol, const char* id, enum type type, int size, enum kind kind);
//void freeSymbol(struct symbol *symbol);
const char* typeToString(enum type type);
const char* kindToString(enum kind kind);
void printSymbol(struct symbol* symbol);
int symbolcmp(struct symbol symbol1, struct symbol symbol2);
int symbolcmp_id(struct symbol symbol1, struct symbol symbol2);
int symbolCheckId(struct symbol symbol, const char* id);

static const char* typeString[] = { "INT", "FLOAT"};
static const char* kindString[] = { "variable", "function", "parameter" };

struct SLE {
	struct symbol symbol;
	struct SLE* nextEntry;
};

void printSymbolList(struct SLE* head);
void symbolList_freeList(struct SLE* head);
struct SLE* symbolList_findEntry(struct SLE* head, const char* id);
struct symbol* symbolList_getSymbol(struct SLE* head, const char* id);
struct SLE* symbolList_insertEntry(struct SLE* head, struct symbol symbol);
struct SLE* symbolList_deleteEntry(struct SLE* head, const char* id);

struct STListEntry {
	void* entry;
	struct STListEntry* nextEntry;
};


struct ST {
	struct SLE* symbolListHead;
	struct ST* parentSymbolTable;
	struct STListEntry* Chead;
};

struct STListEntry* STListEntry_initializeEntry();
struct STListEntry* STList_insertEntry(struct STListEntry* head);
struct STListEntry* STList_deleteEntry(struct STListEntry* head, struct ST* ST);
void STList_freeList(struct STListEntry* head);

struct ST* ST_getParent(struct ST* ST);
struct ST* ST_addChild(struct ST* ST);
struct ST* ST_deleteChild(struct ST* ST);

struct symbol* ST_getSymbol(struct ST* ST, const char* id);
int ST_insertSymbol(struct ST* ST, struct symbol symbol);
void ST_deleteSymbol(struct ST* ST, const char* id);

int insert(struct ST* ST, const char* id, enum type type, int size, enum kind kind);
struct symbol* lookup(struct ST* ST, const char* id);

struct ST* enter_scope(struct ST* ST);
struct ST* exit_scope(struct ST* ST);
