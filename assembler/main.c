#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cpu.h"

typedef struct{
	char* name;
	int mempos;
} Label;

typedef struct{
	Label* labels;
	int count;
} LabelArray;

u8* add_byte(u8* data, int* mempos, u8 byte)
{
	data = (u8*)realloc(data, ++(*mempos));
	data[*mempos-1] = byte;
	return data;
}

u8* compile(char** lines, int linecount, LabelArray* labels, int *data_len)
{
	u8* data = NULL;
	int mempos = 0;
	int line = 1;

	int i;
	for(i = 0; i < linecount; i++) {
		if(lines[i][0] == ':') {
			line++;
			continue;		
		}else if(lines[i][0] == 0) {
			line++;
			continue;
		}

		char* linedup = strdup(lines[i]);
		char* tok = strtok(linedup, " ,");
		
		if(strcmp(tok, "NOP") == 0){
			data = add_byte(data, &mempos, 0x00);
		}else if(strcmp(tok, "HLT") == 0) {
			data = add_byte(data, &mempos, 0x01);
		}else if(strcmp(tok, "LDAX") == 0) {
			data = add_byte(data, &mempos, 0x02);
		}else if(strcmp(tok, "LDAY") == 0) {
			data = add_byte(data, &mempos, 0x03);
		}else if(strcmp(tok, "LDXY") == 0) {
			data = add_byte(data, &mempos, 0x04);
		}else if(strcmp(tok, "LDXA") == 0) {
			data = add_byte(data, &mempos, 0x05);
		}else if(strcmp(tok, "LDYX") == 0) {
			data = add_byte(data, &mempos, 0x06);
		}else if(strcmp(tok, "LDYA") == 0) {
			data = add_byte(data, &mempos, 0x07);
		}else if(strcmp(tok, "LDHA") == 0) {
			data = add_byte(data, &mempos, 0x08);
			tok = strtok(NULL, " ,");
			int num = strtol(tok, NULL, 0);
			if(num > 255 || num < 0) {
				printf("ERROR line %d: Value out of bounds, needed 8-bit unsigned value\n", line);
				exit(-1);
			}
			data = add_byte(data, &mempos, (u8)num);
		}else if(strcmp(tok, "LDLA") == 0) {
			data = add_byte(data, &mempos, 0x09);
			tok = strtok(NULL, " ,");
			int num = strtol(tok, NULL, 0);
			if(num > 255 || num < 0) {
				printf("ERROR line %d: Value out of bounds, needed 8-bit unsigned value\n", line);
				exit(-1);
			}
			data = add_byte(data, &mempos, (u8)num);
		}else if(strcmp(tok, "LDXM") == 0) {
			data = add_byte(data, &mempos, 0x0A);
		}else if(strcmp(tok, "LDYM") == 0) {
			data = add_byte(data, &mempos, 0x0B);
		}else if(strcmp(tok, "LDAM") == 0) {
			data = add_byte(data, &mempos, 0x0C);
		}else if(strcmp(tok, "INCA") == 0) {
			data = add_byte(data, &mempos, 0x0D);
		}else if(strcmp(tok, "INCX") == 0) {
			data = add_byte(data, &mempos, 0x0E);
		}else if(strcmp(tok, "INCY") == 0) {
			data = add_byte(data, &mempos, 0x0F);
		}else if(strcmp(tok, "DECA") == 0) {
			data = add_byte(data, &mempos, 0x10);
		}else if(strcmp(tok, "DECX") == 0) {
			data = add_byte(data, &mempos, 0x11);
		}else if(strcmp(tok, "DECY") == 0) {
			data = add_byte(data, &mempos, 0x12);
		}else if(strcmp(tok, "ADAX") == 0) {
			data = add_byte(data, &mempos, 0x13);
		}else if(strcmp(tok, "ADAY") == 0) {
			data = add_byte(data, &mempos, 0x14);
		}else if(strcmp(tok, "SUAX") == 0) {
			data = add_byte(data, &mempos, 0x15);
		}else if(strcmp(tok, "SUAY") == 0) {
			data = add_byte(data, &mempos, 0x16);
		}else if(strcmp(tok, "STRA") == 0) {
			data = add_byte(data, &mempos, 0x17);
		}else if(strcmp(tok, "STRX") == 0) {
			data = add_byte(data, &mempos, 0x18);
		}else if(strcmp(tok, "STRY") == 0) {
			data = add_byte(data, &mempos, 0x19);
		}else if(strcmp(tok, "LDAC") == 0) {
			data = add_byte(data, &mempos, 0x1A);
			tok = strtok(NULL, " ,");
			int num = strtol(tok, NULL, 0);
			if(num > 255 || num < 0) {
				printf("ERROR line %d: Value out of bounds, needed 8-bit value\n", line);
				exit(-1);
			}
			data = add_byte(data, &mempos, (u8)num);
		}else if(strcmp(tok, "LDXC") == 0) {
			data = add_byte(data, &mempos, 0x1B);
			tok = strtok(NULL, " ,");
			int num = strtol(tok, NULL, 0);
			if(num > 255 || num < 0) {
				printf("ERROR line %d: Value out of bounds, needed 8-bit value\n", line);
				exit(-1);
			}
			data = add_byte(data, &mempos, (u8)num);
		}else if(strcmp(tok, "LDYC") == 0) {
			data = add_byte(data, &mempos, 0x1C);
			tok = strtok(NULL, " ,");
			int num = strtol(tok, NULL, 0);
			if(num > 255 || num < 0) {
				printf("ERROR line %d: Value out of bounds, needed 8-bit value\n", line);
				exit(-1);
			}
			data = add_byte(data, &mempos, (u8)num);
		}else if(strcmp(tok, "JMP") == 0) {
			data = add_byte(data, &mempos, 0x1D);
		}else if(strcmp(tok, "JMPL") == 0) {
			tok = strtok(NULL, " ,");
			int j;
			Label label;
			label.name = NULL;
			for(j = 0; j < labels->count; j++) {
				if(strcmp(tok, labels->labels[j].name) == 0){
					label = labels->labels[j];
				}
			}
			if(label.name == NULL) {
				printf("ERROR line %d: Unknown label!\n", line);
				exit(-1);
			}
			
			data = add_byte(data, &mempos, 0x08);
			data = add_byte(data, &mempos, (u8)((label.mempos & 0xFF00) >> 8));
			data = add_byte(data, &mempos, 0x09);
			data = add_byte(data, &mempos, (u8)(label.mempos & 0xFF));
			data = add_byte(data, &mempos, 0x1D);
		}else if(strcmp(tok, "INCD") == 0) {
			data = add_byte(data, &mempos, 0x1E);
		}else if(strcmp(tok, "SETD") == 0) {
			tok = strtok(NULL, " ,");
			int j;
			Label label;
			label.name = NULL;
			for(j = 0; j < labels->count; j++) {
				if(strcmp(tok, labels->labels[j].name) == 0){
					label = labels->labels[j];
				}
			}
			if(label.name == NULL) {
				printf("ERROR line %d: Unknown label!\n", line);
				exit(-1);
			}
			
			data = add_byte(data, &mempos, 0x08);
			data = add_byte(data, &mempos, (u8)((label.mempos & 0xFF00) >> 8));
			data = add_byte(data, &mempos, 0x09);
			data = add_byte(data, &mempos, (u8)(label.mempos & 0xFF));
		}else if(strcmp(tok, "CMPXY") == 0) {
			data = add_byte(data, &mempos, 0x1F);
		}else if(strcmp(tok, "CMPAX") == 0) {
			data = add_byte(data, &mempos, 0x20);
		}else if(strcmp(tok, "CMPAY") == 0) {
			data = add_byte(data, &mempos, 0x21);
		}else if(strcmp(tok, "JE") == 0) {
			data = add_byte(data, &mempos, 0x22);
		}else if(strcmp(tok, "JNE") == 0) {
			data = add_byte(data, &mempos, 0x23);
		}else if(strcmp(tok, "JL") == 0) {
			data = add_byte(data, &mempos, 0x24);
		}else if(strcmp(tok, "JG") == 0) {
			data = add_byte(data, &mempos, 0x25);
		}else if(strcmp(tok, "ADDA") == 0) {
			data = add_byte(data, &mempos, 0x26);
		}else if(strcmp(tok, "ADDX") == 0) {
			data = add_byte(data, &mempos, 0x27);
		}else if(strcmp(tok, "ADDY") == 0) {
			data = add_byte(data, &mempos, 0x28);
		}
		else {
			printf("ERROR line %d: Unknwon opcode '%s'\n", line, tok);
			exit(-1);
		}

		line++;
	}

	*data_len = mempos;	
	return data;
}

LabelArray* find_labels(char** lines, int linecount, int memoffset)
{
	LabelArray* array = (LabelArray*)malloc(sizeof(LabelArray));
	array->labels = NULL;
	array->count = 0;
	
	int i;
	int mempos = memoffset;
	for(i = 0; i < linecount; i++) {
		if(lines[i][0] == ':') {
			if(lines[i][1] == 0 || lines[i][1] == '\n') {
				printf("ERROR line %d: No label following ':'.\n", i+1);
				exit(-1);
			}
			array->count++;
			array->labels = (Label*)realloc(array->labels, sizeof(Label)*array->count);
			array->labels[array->count-1].mempos = mempos;
			array->labels[array->count-1].name = strdup(lines[i]+1);

			continue;
		}else if(lines[i][0] == 0) {
			continue;
		}

		char* line = strdup(lines[i]);
		char* tok = strtok(line, " ,");
		
		if(strcmp(tok, "NOP") == 0){
			mempos++;
		}else if(strcmp(tok, "HLT") == 0) {
			mempos++;
		}else if(strcmp(tok, "LDAX") == 0) {
			mempos++;
		}else if(strcmp(tok, "LDAY") == 0) {
			mempos++;
		}else if(strcmp(tok, "LDXY") == 0) {
			mempos++;
		}else if(strcmp(tok, "LDXA") == 0) {
			mempos++;
		}else if(strcmp(tok, "LDYX") == 0) {
			mempos++;
		}else if(strcmp(tok, "LDYA") == 0) {
			mempos++;
		}else if(strcmp(tok, "LDHA") == 0) {
			mempos += 2;
		}else if(strcmp(tok, "LDLA") == 0) {
			mempos += 2;
		}else if(strcmp(tok, "LDXM") == 0) {
			mempos++;
		}else if(strcmp(tok, "LDYM") == 0) {
			mempos++;			
		}else if(strcmp(tok, "LDAM") == 0) {
			mempos++;
		}else if(strcmp(tok, "INCA") == 0) {
			mempos++;
		}else if(strcmp(tok, "INCX") == 0) {
			mempos++;
		}else if(strcmp(tok, "INCY") == 0) {
			mempos++;
		}else if(strcmp(tok, "DECA") == 0) {
			mempos++;
		}else if(strcmp(tok, "DECX") == 0) {
			mempos++;
		}else if(strcmp(tok, "DECY") == 0) {
			mempos++;
		}else if(strcmp(tok, "ADAX") == 0) {
			mempos++;
		}else if(strcmp(tok, "ADAY") == 0) {
			mempos++;
		}else if(strcmp(tok, "SUAX") == 0) {
			mempos++;
		}else if(strcmp(tok, "SUAY") == 0) {
			mempos++;
		}else if(strcmp(tok, "STRA") == 0) {
			mempos++;
		}else if(strcmp(tok, "STRX") == 0) {
			mempos++;
		}else if(strcmp(tok, "STRY") == 0) {
			mempos++;
		}else if(strcmp(tok, "LDAC") == 0) {
			mempos += 2;
		}else if(strcmp(tok, "LDXC") == 0) {
			mempos += 2;
		}else if(strcmp(tok, "LDYC") == 0) {
			mempos += 2;
		}else if(strcmp(tok, "JMP") == 0) {
			mempos++;
		}else if(strcmp(tok, "JMPL") == 0) {
			mempos += 5;
		}else if(strcmp(tok, "INCD") == 0) {
			mempos++;
		}else if(strcmp(tok, "CMPXY") == 0) {
			mempos++;
		}else if(strcmp(tok, "CMPAX") == 0) {
			mempos++;
		}else if(strcmp(tok, "CMPAY") == 0) {
			mempos++;
		}else if(strcmp(tok, "JE") == 0) {
			mempos++;
		}else if(strcmp(tok, "JNE") == 0) {
			mempos++;
		}else if(strcmp(tok, "JL") == 0) {
			mempos++;
		}else if(strcmp(tok, "JG") == 0) {
			mempos++;
		}else if(strcmp(tok, "ADDA") == 0) {
			mempos++;
		}else if(strcmp(tok, "ADDX") == 0) {
			mempos++;
		}else if(strcmp(tok, "ADDY") == 0) {
			mempos++;
		}else if(strcmp(tok, "SETD") == 0) {
			mempos += 4;
		}
	}

	return array;
}

void remove_newlines(char** lines, int linecount)
{
	int i;
	for(i = 0; i < linecount; i++) {
		if(lines[i][strlen(lines[i])-1] == '\n') {
			lines[i][strlen(lines[i])-1] = 0;
		}
	}
}

int main(int argc, char** argv)
{
	if(argc < 4) {
		printf("Usage: ccpu-asm <input file> <output file> <memory offset> [options]\n\nOptions:\n\t-m - Output a memory map\n");
		return -1;
	}

	int mapflag = 0;

	if(argc > 4) {
		int i;
		for(i = 4; i < argc; i++) {
			if(strcmp(argv[i], "-m") == 0) {
				mapflag = 1;
			}
		}
	}

	int offset = strtol(argv[3], NULL, 0);

	FILE* f = fopen(argv[1], "r");
	if(f == NULL) {
		printf("Failed to open '%s'\n", argv[1]);
		return -1;
	}

	// Read f line by line into 'lines', total line count in 'linecount'
	char** lines = NULL;
	char* buffer;
	buffer = (char*)malloc(sizeof(char)*256);
	int linecount = 0;
	buffer = fgets(buffer, 256, f);
	while(buffer != NULL || !feof(f)) {
		linecount++;
		lines = (char**)realloc(lines, sizeof(char*)*linecount);
		lines[linecount-1] = strdup(buffer);
		
		buffer = fgets(buffer, 256, f);
	}
	fclose(f);
	
	// Remove all newlines at the end of a lines
	remove_newlines(lines, linecount);

	// Find and calculate offset for every label
	LabelArray *labels = find_labels(lines, linecount, offset);
/*	int i;
	for(i = 0; i < labels->count; i++) {
		printf("Name: %s, Offset: %d\n", labels->labels[i].name, labels->labels[i].mempos);
	}*/

	// Compile the assembly
	int data_len = 0;
	u8* data = compile(lines, linecount, labels, &data_len);
	if(data == NULL) {
		printf("No instruction in source file! Therefore no output!\n");
		return -1;
	}

	// Write to output file
	FILE* of = fopen(argv[2], "wb");
	fwrite(data, sizeof(u8), data_len, of);
	fclose(of);

	// If 'mapflag' output a .map file containing addreses for all labels
	if(mapflag) {
		char* filename = strdup(argv[2]);
		strncat(filename, ".map", 4);
		FILE* map = fopen(filename, "w");

		fprintf(map, "OFFSET 0x%04X\n\n", offset);

		int i;
		for(i = 0; i < labels->count; i++) {
			fprintf(map, "%s: 0x%04X\n", labels->labels[i].name, labels->labels[i].mempos);
		}
		
		fclose(map);
	}
	
	free(lines);
	return 0;	
}
