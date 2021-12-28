#include "semantic.h"

void initializeSymbol(struct symbol* symbol, const char* id, enum type type, int size, enum kind kind) {

	symbol->id = id;
	memcpy(&(symbol->type), &type, sizeof(type)); //int, or float
	symbol->size = size;
	memcpy(&(symbol->kind), &kind, sizeof(kind)); //var, 
}

const char* typeToString(enum type type) {
	return typeString[type];
}

const char* kindToString(enum kind kind) {
	return kindString[kind];
}

void printSymbol(struct symbol* symbol) {
	if (symbol == NULL) {
		printf("symbol==NULL\n");
		return;
	}
	printf("<%s,%s,%s>\n", symbol->id, typeToString(symbol->type), kindToString(symbol->kind));
}

int symbolcmp(struct symbol symbol1, struct symbol symbol2) {
	if (
		symbol1.kind == symbol2.kind &&
		symbol1.type == symbol2.type &&
		strcmp(symbol1.id, symbol2.id) == 0
		) {
		return 0;
	}
	return 1;
}

int symbolcmp_id(struct symbol symbol1, struct symbol symbol2) {
	if (strcmp(symbol1.id, symbol2.id) == 0) {
		return 0;
	}
	return 1;
}

int symbolCheckId(struct symbol symbol, const char* id) {
	if (strcmp(symbol.id, id) == 0) {
		return 0;
	}
	return 1;
}

void printSymbolList(struct SLE* head) {
	printf("SymbolList:\n");
	if (head == NULL) {
		printf("\tNo symbols\n");
	}
	while (head != NULL) {
		printf("\t");
		printSymbol(&(head->symbol));
		head = head->nextEntry;
	}
}

void symbolList_freeList(struct SLE* head) {
	if (head == NULL) {
		return;
	}
	struct SLE* next;
	next = head->nextEntry;
	while (head != NULL) {
		//free(&(head->symbol));
		next = head->nextEntry;
		free(head);
		head = next;
	}
}

static struct SLE* initializeEntry(struct symbol symbol) {

	struct SLE* entry = (struct SLE*) malloc(sizeof(struct SLE));
	entry->symbol = symbol;
	entry->nextEntry = NULL;
	return entry;
}

struct SLE* symbolList_findEntry(struct SLE* head, const char* id) {
	while (head != NULL && symbolCheckId(head->symbol, id)) {
		head = head->nextEntry;
	}
	return head;
}

struct symbol* symbolList_getSymbol(struct SLE* head, const char* id) {
	struct SLE* entry = symbolList_findEntry(head, id);
	if (entry == NULL) {
		return NULL;
	}
	return &(entry->symbol);
}

struct SLE* symbolList_insertEntry(struct SLE* head, struct symbol symbol) {
	if (symbolList_findEntry(head, symbol.id) != NULL)
	{
		return head;
	}
	struct SLE* headTemp = initializeEntry(symbol);
	headTemp->nextEntry = head;
	return headTemp;
}

struct SLE* symbolList_deleteEntry(struct SLE* head, const char* id) {
	if (head == NULL) {
		return NULL;
	}
	if (!symbolCheckId(head->symbol, id)) {
		return head->nextEntry;
	}
	struct SLE* current = head;
	struct SLE* previous = NULL;
	while (current != NULL && symbolCheckId(current->symbol, id)) {
		previous = current;
		current = current->nextEntry;
	}
	if (current != NULL) {
		previous->nextEntry = current->nextEntry;
		free(current);
	}
	return head;
}

struct STListEntry* STListEntry_initializeEntry() {
	struct STListEntry* STListEntry = (struct STListEntry*) malloc(sizeof(struct STListEntry));
	STListEntry->entry = (struct ST*) malloc(sizeof(struct ST));
	STListEntry->nextEntry = NULL;
	return STListEntry;
}

struct STListEntry* STList_insertEntry(struct STListEntry* head) {
	struct STListEntry* headTemp = STListEntry_initializeEntry();
	headTemp->nextEntry = head;
	return headTemp;
}

struct STListEntry* STList_deleteEntry(struct STListEntry* head, struct ST* ST) {
	if (head == NULL) {
		return NULL;
	}
	if (head->entry == ST) {
		return head->nextEntry;
	}
	struct STListEntry* current = head;
	struct STListEntry* previous = NULL;
	while (current != NULL && current->entry != ST) {
		previous = current;
		current = current->nextEntry;
	}
	if (current != NULL) {
		previous->nextEntry = current->nextEntry;
		free(current->entry);
		free(current);
	}
	return head;
}

void STList_freeList(struct STListEntry* head) {
	if (head == NULL) {
		return;
	}
	struct STListEntry* next;
	next = head->nextEntry;
	while (head != NULL) {
		free(head->entry);
		next = head->nextEntry;
		free(head);
		head = next;
	}
}

struct ST* ST_getParent(struct ST* ST) {
	return ST->parentSymbolTable;
}

struct ST* ST_addChild(struct ST* ST) {
	struct STListEntry* CLEntry;
	CLEntry = STListEntry_initializeEntry();
	ST->Chead = STList_insertEntry(CLEntry);
	struct ST* childSymbolTable = (struct ST*)CLEntry->entry;
	childSymbolTable->parentSymbolTable = ST;
	childSymbolTable->symbolListHead = NULL;
	return childSymbolTable;
}

struct ST* ST_deleteChild(struct ST* ST) {
	//printf("ST_deleteChild()\n");
	struct ST* parentSymbolTable = ST_getParent(ST);
	if (parentSymbolTable != NULL) {
		STList_deleteEntry(parentSymbolTable->Chead, ST);
	}
	//printf("STList_freeList()\n");
	STList_freeList(ST->Chead);
	//printf("symbolList_freeList()\n");
	symbolList_freeList(ST->symbolListHead);
	return parentSymbolTable;
}

struct symbol* ST_getSymbol(struct ST* ST, const char* id) {
	return symbolList_getSymbol(ST->symbolListHead, id);
}

int ST_insertSymbol(struct ST* ST, struct symbol symbol) {
	struct SLE* head_old = ST->symbolListHead;
	ST->symbolListHead = symbolList_insertEntry(ST->symbolListHead, symbol);

	if (head_old == ST->symbolListHead)
	{
		return 0;
	}
	return 1;
}

void ST_deleteSymbol(struct ST* ST, const char* id) {
	ST->symbolListHead = symbolList_deleteEntry(ST->symbolListHead, id);
}

int insert(struct ST* ST, const char* id, enum type type, int size, enum kind kind) {
	struct symbol symbol;
	initializeSymbol(&symbol, id, type, size, kind);
	return ST_insertSymbol(ST, symbol);
}

struct symbol* lookup(struct ST* ST, const char* id) {
	struct ST* STIter = ST;
	struct symbol* symbol = ST_getSymbol(STIter, id);
	while (symbol == NULL) {
		STIter = ST_getParent(STIter);
		if (STIter == NULL) {
			break;
		}
		symbol = ST_getSymbol(STIter, id);
	}
	return symbol;
}

struct ST* enter_scope(struct ST* ST) {
	return ST_addChild(ST);
}

struct ST* exit_scope(struct ST* ST) {
	return ST_getParent(ST);
}
