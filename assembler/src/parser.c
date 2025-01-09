#include "parser.h"
#include "scanner.h"
#include "../include/opcode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 2048
#define MAX_LABELS 512

typedef struct
{
    char label[64];
    int address; // in unità di istruzione
} LabelEntry;

static LabelEntry g_labels[MAX_LABELS];
static int g_labelCount = 0;

// Struttura che memorizza le linee parse del primo pass
typedef struct
{
    AsmLine line;
    int address; // numero istruzione (nel file .bin, ogni istr = 4 byte)
} ParsedLine;

static ParsedLine g_parsedLines[MAX_LINES];
static int g_lineUsed = 0;

static int find_label_address(const char *name)
{
    for (int i = 0; i < g_labelCount; i++)
    {
        if (strcmp(g_labels[i].label, name) == 0)
        {
            return g_labels[i].address;
        }
    }
    return -1;
}

// Aggiunge una label con address
static void add_label(const char *name, int addr)
{
    if (g_labelCount < MAX_LABELS)
    {
        strncpy(g_labels[g_labelCount].label, name, 63);
        g_labels[g_labelCount].label[63] = '\0';
        g_labels[g_labelCount].address = addr;
        g_labelCount++;
    }
    else
    {
        fprintf(stderr, "[ERRORE] Troppi label.\n");
    }
}

// Helper per parse dei registri "R3" => 3
static int parseRegister(const char *s)
{
    if ((s[0] == 'R' || s[0] == 'r'))
    {
        int n = atoi(s + 1);
        if (n >= 0 && n < NUM_REGISTERS)
        {
            return n;
        }
    }
    return -1;
}

// Crea la word finale [opcode << 24 | payload & 0xFFFFFF]
static inline uint32_t make_instr(uint8_t opcode, uint32_t payload)
{
    return ((uint32_t)opcode << 24) | (payload & 0x00FFFFFF);
}

int assemble_file(const char *input_filename, const char *output_filename)
{
    // 1) Apertura file input
    FILE *fin = fopen(input_filename, "r");
    if (!fin)
    {
        fprintf(stderr, "Impossibile aprire '%s'\n", input_filename);
        return 1;
    }

    // Leggiamo riga per riga con lo scanner, accumuliamo in g_parsedLines
    g_labelCount = 0;
    g_lineUsed = 0;
    int lineno = 0;
    int current_addr = 0; // contatore di istruzioni

    while (1)
    {
        AsmLine line;
        int ret = scanner_next_line(fin, &line, &lineno);
        if (ret == 0)
        {
            // EOF
            break;
        }
        if (ret < 0)
        {
            // errore
            continue;
        }

        if (line.lineType == LINE_TYPE_EMPTY)
        {
            // riga vuota
            continue;
        }

        // Se c'è label, registrala
        if (line.label[0] != '\0')
        {
            // label = current_addr
            add_label(line.label, current_addr);
        }

        // Se la riga è un'istruzione o .db
        if (strcasecmp(line.mnemonic, ".db") == 0)
        {
            // .db -> 1 word (4 byte)
            // contiamo come 1 "istr" => address
            ParsedLine *pl = &g_parsedLines[g_lineUsed++];
            pl->line = line;
            pl->address = current_addr;
            current_addr++;
        }
        else
        {
            // Istruzione
            ParsedLine *pl = &g_parsedLines[g_lineUsed++];
            pl->line = line;
            pl->address = current_addr;
            current_addr++;
        }
    }
    fclose(fin);

    // 2) Apertura file output
    FILE *fout = fopen(output_filename, "wb");
    if (!fout)
    {
        fprintf(stderr, "Impossibile creare '%s'\n", output_filename);
        return 1;
    }

    // 3) Second pass: costruiamo istruzioni
    for (int i = 0; i < g_lineUsed; i++)
    {
        AsmLine *L = &g_parsedLines[i].line;
        if (strcasecmp(L->mnemonic, ".db") == 0)
        {
            // .db => scrive 4 byte interpretati come hex
            // Esempio: .db 0xFF, 0x00, 0x00, 0x00
            uint8_t bytes[4] = {0, 0, 0, 0};
            for (int k = 0; k < 4 && k < L->ntok; k++)
            {
                bytes[k] = (uint8_t)strtoul(L->tokens[k], NULL, 0);
            }
            fwrite(bytes, 1, 4, fout);
            continue;
        }

        // Altrimenti è un'istr.
        // Costruiamo opcode e payload:
        uint8_t opcode = OP_UNKNOWN;
        uint32_t payload = 0;

        // Confronta mnemonico
        char upper[32];
        memset(upper, 0, sizeof(upper));
        for (int c = 0; c < (int)strlen(L->mnemonic) && c < 31; c++)
        {
            upper[c] = (char)toupper((unsigned char)L->mnemonic[c]);
        }

        // (per brevità, grande if-else, potresti farlo con una tabella)
        if (strcmp(upper, "MOV") == 0)
            opcode = OP_MOV;
        else if (strcmp(upper, "MOVI") == 0)
            opcode = OP_MOVI;
        else if (strcmp(upper, "ADD") == 0)
            opcode = OP_ADD;
        else if (strcmp(upper, "SUB") == 0)
            opcode = OP_SUB;
        else if (strcmp(upper, "AND") == 0)
            opcode = OP_AND;
        else if (strcmp(upper, "OR") == 0)
            opcode = OP_OR;
        else if (strcmp(upper, "XOR") == 0)
            opcode = OP_XOR;
        else if (strcmp(upper, "NOT") == 0)
            opcode = OP_NOT;
        else if (strcmp(upper, "SHL") == 0)
            opcode = OP_SHL;
        else if (strcmp(upper, "SHR") == 0)
            opcode = OP_SHR;
        else if (strcmp(upper, "LOAD") == 0)
            opcode = OP_LOAD;
        else if (strcmp(upper, "STORE") == 0)
            opcode = OP_STORE;
        else if (strcmp(upper, "JMP") == 0)
            opcode = OP_JMP;
        else if (strcmp(upper, "JEQ") == 0)
            opcode = OP_JEQ;
        else if (strcmp(upper, "JNE") == 0)
            opcode = OP_JNE;
        else if (strcmp(upper, "PUSH") == 0)
            opcode = OP_PUSH;
        else if (strcmp(upper, "POP") == 0)
            opcode = OP_POP;
        else if (strcmp(upper, "CALL") == 0)
            opcode = OP_CALL;
        else if (strcmp(upper, "RET") == 0)
            opcode = OP_RET;
        else if (strcmp(upper, "DRAW") == 0)
            opcode = OP_DRAW;
        else
            opcode = OP_UNKNOWN;

// Macros per bit shift
#define RD_SHIFT 20
#define RS_SHIFT 16
#define RT_SHIFT 12

        switch (opcode)
        {
        case OP_MOV:
        {
            if (L->ntok < 2)
                break;
            int rd = parseRegister(L->tokens[0]);
            int rs = parseRegister(L->tokens[1]);
            payload = ((rd & 0xF) << RD_SHIFT) | ((rs & 0xF) << RS_SHIFT);
            break;
        }
        case OP_MOVI:
        {
            if (L->ntok < 2)
                break;
            int rd = parseRegister(L->tokens[0]);
            uint32_t imm = strtoul(L->tokens[1], NULL, 0) & 0xFFFFF;
            payload = ((rd & 0xF) << RD_SHIFT) | imm;
            break;
        }
        case OP_ADD:
        case OP_SUB:
        case OP_AND:
        case OP_OR:
        case OP_XOR:
        {
            if (L->ntok < 3)
                break;
            int rd = parseRegister(L->tokens[0]);
            int rs = parseRegister(L->tokens[1]);
            int rt = parseRegister(L->tokens[2]);
            payload = ((rd & 0xF) << RD_SHIFT) | ((rs & 0xF) << RS_SHIFT) | ((rt & 0xF) << RT_SHIFT);
            break;
        }
        case OP_NOT:
        {
            if (L->ntok < 2)
                break;
            int rd = parseRegister(L->tokens[0]);
            int rs = parseRegister(L->tokens[1]);
            payload = ((rd & 0xF) << RD_SHIFT) | ((rs & 0xF) << RS_SHIFT);
            break;
        }
        case OP_SHL:
        case OP_SHR:
        {
            if (L->ntok < 3)
                break;
            int rd = parseRegister(L->tokens[0]);
            int rs = parseRegister(L->tokens[1]);
            uint32_t imm8 = strtoul(L->tokens[2], NULL, 0) & 0xFF;
            payload = ((rd & 0xF) << RD_SHIFT) | ((rs & 0xF) << RS_SHIFT) | ((imm8 & 0xFF) << 8);
            break;
        }
        case OP_LOAD:
        case OP_STORE:
        {
            if (L->ntok < 2)
                break;
            // LOAD rd, imm20
            // STORE rs, imm20
            int rA = parseRegister(L->tokens[0]);
            uint32_t addr = strtoul(L->tokens[1], NULL, 0) & 0xFFFFF;
            payload = ((rA & 0xF) << RD_SHIFT) | addr;
            break;
        }
        case OP_JMP:
        case OP_JEQ:
        case OP_JNE:
        case OP_CALL:
        {
            if (L->ntok < 1)
                break;
            // Se tokens[0] è label, cerchiamo l'indirizzo
            int lblAddr = find_label_address(L->tokens[0]);
            if (lblAddr < 0)
            {
                // se non trovato => interpretalo come valore numerico
                lblAddr = (int)strtoul(L->tokens[0], NULL, 0);
            }
            payload = lblAddr & 0xFFFFFF;
            break;
        }
        case OP_PUSH:
        {
            if (L->ntok < 1)
                break;
            int rs = parseRegister(L->tokens[0]);
            payload = ((rs & 0xF) << RD_SHIFT);
            break;
        }
        case OP_POP:
        {
            if (L->ntok < 1)
                break;
            int rd = parseRegister(L->tokens[0]);
            payload = ((rd & 0xF) << RD_SHIFT);
            break;
        }
        case OP_RET:
        {
            // nessun arg
            break;
        }
        case OP_DRAW:
        {
            if (L->ntok < 3)
                break;
            int rx = parseRegister(L->tokens[0]);
            int ry = parseRegister(L->tokens[1]);
            int rc = parseRegister(L->tokens[2]);
            payload = ((rx & 0xF) << RD_SHIFT) | ((ry & 0xF) << RS_SHIFT) | ((rc & 0xF) << RT_SHIFT);
            break;
        }
        default:
            // OP_UNKNOWN => 0xFF => ferma la CPU
            break;
        }

        uint32_t final = make_instr(opcode, payload);
        fwrite(&final, sizeof(uint32_t), 1, fout);
    }

    fclose(fout);
    return 0;
}
