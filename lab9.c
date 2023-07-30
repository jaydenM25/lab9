#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

struct HashEntry{
    struct RecordType record;
    struct HashEntry* next;
};

// Fill out this structure
struct HashType
{
    struct HashEntry* head;
};

// Compute the hash function
int hash(int x, int max)
{
    return x % max;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
        struct HashEntry* temp = pHashArray[i].head;
        if(temp == NULL)
            continue;
        printf("Index %d: ", i);
        while(temp != NULL) {
            if(temp->next != NULL)
                printf("ID: %d, Name: %c, Order: %d -> ", temp->record.id, temp->record.name, temp->record.order);
            else
                printf("ID: %d, Name: %c, Order: %d ", temp->record.id, temp->record.name, temp->record.order);
            temp = temp->next;
        }
        printf("\n");
	}
}

void createEntry(struct RecordType *pRecords, struct HashType* Table, int index, int i) {
    	struct HashEntry* newEntry = malloc(sizeof(struct HashEntry));
	    if (newEntry == NULL)
	        exit(-1);
	    newEntry->next = Table[index].head;
	    Table[index].head = newEntry;
	    newEntry->record = pRecords[i];
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	
		// Your hash implementation
	
	struct HashType* Table = malloc(sizeof(struct HashType) * 15);
	for(int i = 0; i < 15; i++)
	    Table[i].head = NULL;
	    
	for(int i = 0; i < recordSz; i++) {
	    int index = hash(pRecords[i].order, 15);
        createEntry(pRecords, Table, index, i);
	}
	
	displayRecordsInHash(Table, 15);
	
	for(int i = 0; i < 15; i++) {
	    struct HashEntry * temp = Table[i].head;
	    while(temp != NULL) {
	        temp = temp->next;
	        free(temp);
	    }
	}
	free(Table);
	free(pRecords);
}
