#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "../cpu/opcode.h" // Contiene la enum con OP_ADD, OP_SUB, etc.

#define MAX_LINES 1024
#define MAX_TOKENS 16
#define MAX_LABELS 256

typedef struct
{
    char label[64];
    int address; // in "istruzioni" (non in byte), poi *4 => offset
} LabelEntry;

static LabelEntry g_labels[MAX_LABELS];
static int g_labelCount = 0;

// Struttura che memorizza una singola riga "parsata" in modo incompleto
// Nel primo pass, non risolviamo label di jump/call => usiamo placeholder
typedef enum
{
    LINE_TYPE_INST,
    LINE_TYPE_DB
} LineType;

typedef struct
{
    LineType type;
    uint32_t instr; // se type=INST, conterrà opcode e parte costante
    char mnemonic[16];
    char tokens[MAX_TOKENS][32]; // parametri
    int ntok;
    int lineNumber;
    // Se l'istruzione ha un riferimento a label, la memorizziamo
    // es. JMP PIPPO => referenceLabel="PIPPO"
    // Nel 2° pass risolviamo e mettiamo nel instr i 24 bit di address
    char referenceLabel[64];
} ParsedLine;

// array di linee
static ParsedLine g_parsedLines[MAX_LINES];
static int g_lineCount = 0;

// -------------------------------------------------------
// Funzioni di label e parsing
// -------------------------------------------------------
static void add_label(const char *labelName, int address)
{
    if (g_labelCount < MAX_LABELS)
    {
        strncpy(g_labels[g_labelCount].label, labelName, 63);
        g_labels[g_labelCount].label[63] = '\0';
        g_labels[g_labelCount].address = address;
        g_labelCount++;
    }
    else
    {
        fprintf(stderr, "[ERRORE] Troppe label!\n");
    }
}

static int find_label_address(const char *labelName)
{
    for (int i = 0; i < g_labelCount; i++)
    {
        if (strcmp(g_labels[i].label, labelName) == 0)
        {
            return g_labels[i].address;
        }
    }
    return -1; // not found
}

// Helper: crea una word 32 bit mettendo opcode nei bit alti
static uint32_t make_instr(uint8_t opcode, uint32_t payload)
{
    return ((uint32_t)opcode << 24) | (payload & 0x00FFFFFF);
}

// parseRegister("R3") => 3
static int parseRegister(const char *s)
{
    if (s[0] != 'R' && s[0] != 'r')
        return -1;
    int n = atoi(s + 1);
    if (n < 0 || n >= NUM_REGISTERS)
        return -1;
    return n;
}

// toUpper string
static void str_toupper(char *dst, const char *src)
{
    while (*src)
    {
        *dst = (char)toupper((unsigned char)*src);
        dst++;
        src++;
    }
    *dst = '\0';
}

// -------------------------------------------------------
// 1° pass: leggiamo le righe, salviamo istruzioni e label
// -------------------------------------------------------
static void first_pass(FILE *fin)
{
    char linebuf[256];
    int current_address = 0; // in "istruzioni" -> ogni istr = 4 byte
    int lineno = 1;

    while (fgets(linebuf, sizeof(linebuf), fin))
    {
        // Rimuovi commenti
        char *comment = strchr(linebuf, ';');
        if (comment)
            *comment = '\0';

        // Trim spazi iniziali/finali
        // (per brevità, facciamo un trim veloce)
        // ...

        // Se la riga è vuota, skip
        char *tmp = linebuf;
        while (isspace((unsigned char)*tmp))
            tmp++;
        if (*tmp == '\0')
        {
            lineno++;
            continue;
        }

        // Ora cerchiamo se c'è una label del tipo "mio_label:"
        char label_name[64];
        memset(label_name, 0, sizeof(label_name));

        // Copia la riga per tokenizzare
        char linecpy[256];
        strncpy(linecpy, tmp, 255);
        linecpy[255] = '\0';

        char *token = strtok(linecpy, " \t\n");
        if (!token)
        {
            lineno++;
            continue;
        }

        // Controlla se token finisce con ':'
        size_t tlen = strlen(token);
        if (token[tlen - 1] == ':')
        {
            // Rimuovi ':'
            token[tlen - 1] = '\0';
            strcpy(label_name, token);

            // Aggiungi label => current_address
            add_label(label_name, current_address);

            // Prendi prossimo token (eventuale opcode)
            token = strtok(NULL, " \t\n");
            if (!token)
            {
                // riga era solo "label:"
                lineno++;
                continue;
            }
        }

        // Adesso token potrebbe essere l'istruzione
        // Salviamo in g_parsedLines
        ParsedLine *pl = &g_parsedLines[g_lineCount];
        memset(pl, 0, sizeof(*pl));
        pl->type = LINE_TYPE_INST;
        pl->lineNumber = lineno;

        strcpy(pl->mnemonic, token);

        // Raccogli i restanti parametri in pl->tokens
        int ntok = 0;
        while (1)
        {
            char *tok2 = strtok(NULL, ", \t\n");
            if (!tok2)
                break;
            // trim spazi
            while (*tok2 && isspace((unsigned char)*tok2))
                tok2++;
            if (*tok2 == '\0')
                continue;

            strncpy(pl->tokens[ntok], tok2, 31);
            pl->tokens[ntok][31] = '\0';
            ntok++;
            if (ntok >= MAX_TOKENS)
                break;
        }
        pl->ntok = ntok;

        // Se è una direttiva .db => passiamo a LINE_TYPE_DB
        // form: ".db 0xFF, 0x00, 0x00, 0x00"
        if (strcasecmp(pl->mnemonic, ".db") == 0)
        {
            pl->type = LINE_TYPE_DB;
            g_lineCount++;
            // ogni .db inserisce 4 byte => +1 "istruzione" (se vogliamo rigido,
            // ma in realtà .db 4 byte = 1 word)
            current_address++;
            lineno++;
            continue;
        }

        // Altrimenti è un'istruzione
        // Per ora non risolviamo label => set instruction = 0xFFFFFFFF
        pl->instr = 0xFFFFFFFF;
        g_lineCount++;
        current_address++; // +1 istr => +4 byte

        lineno++;
    }
}

// -------------------------------------------------------
// 2° pass: risolviamo istruzioni, label reference, generiamo .bin
// -------------------------------------------------------
static int second_pass(const char *input_filename, const char *output_filename)
{
    FILE *fout = fopen(output_filename, "wb");
    if (!fout)
    {
        fprintf(stderr, "Impossibile creare %s\n", output_filename);
        return 1;
    }

    for (int i = 0; i < g_lineCount; i++)
    {
        ParsedLine *pl = &g_parsedLines[i];
        if (pl->type == LINE_TYPE_DB)
        {
            // si aspetta 4 parametri
            // es: .db 0xFF, 0x00, 0x00, 0x00
            uint8_t bytes[4] = {0, 0, 0, 0};
            int bcount = 0;
            for (int k = 0; k < pl->ntok && k < 4; k++)
            {
                bytes[k] = (uint8_t)strtoul(pl->tokens[k], NULL, 0);
                bcount++;
            }
            // scrivi i 4 byte
            fwrite(bytes, 1, 4, fout);
            continue;
        }

        // Altrimenti istruzione. Interpreta pl->mnemonic
        //  e parametri in pl->tokens
        char upper[16];
        str_toupper(upper, pl->mnemonic);

        uint8_t opcode = OP_UNKNOWN;
        uint32_t payload = 0;
// "Aiutino" per fill.

// Macros
#define RD_SHIFT 20
#define RS_SHIFT 16
#define RT_SHIFT 12

        // (Potresti fare un grande if-else / switch)
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

        // Ora interpretiamo i parametri
        switch (opcode)
        {
        case OP_MOV: // MOV rd, rs
        {
            if (pl->ntok < 2)
                break;
            int rd = parseRegister(pl->tokens[0]);
            int rs = parseRegister(pl->tokens[1]);
            if (rd < 0 || rs < 0)
                break;
            payload = ((rd & 0xF) << RD_SHIFT) | ((rs & 0xF) << RS_SHIFT);
            break;
        }
        case OP_MOVI: // MOVI rd, imm20
        {
            if (pl->ntok < 2)
                break;
            int rd = parseRegister(pl->tokens[0]);
            uint32_t imm = strtoul(pl->tokens[1], NULL, 0) & 0xFFFFF;
            payload = ((rd & 0xF) << RD_SHIFT) | imm;
            break;
        }
        case OP_ADD: // ADD rd, rs, rt
        case OP_SUB:
        case OP_AND:
        case OP_OR:
        case OP_XOR:
        {
            if (pl->ntok < 3)
                break;
            int rd = parseRegister(pl->tokens[0]);
            int rs = parseRegister(pl->tokens[1]);
            int rt = parseRegister(pl->tokens[2]);
            if (rd < 0 || rs < 0 || rt < 0)
                break;
            payload = ((rd & 0xF) << RD_SHIFT) | ((rs & 0xF) << RS_SHIFT) | ((rt & 0xF) << RT_SHIFT);
            break;
        }
        case OP_NOT: // NOT rd, rs
        {
            if (pl->ntok < 2)
                break;
            int rd = parseRegister(pl->tokens[0]);
            int rs = parseRegister(pl->tokens[1]);
            payload = ((rd & 0xF) << RD_SHIFT) | ((rs & 0xF) << RS_SHIFT);
            break;
        }
        case OP_SHL: // SHL rd, rs, imm8
        case OP_SHR:
        {
            if (pl->ntok < 3)
                break;
            int rd = parseRegister(pl->tokens[0]);
            int rs = parseRegister(pl->tokens[1]);
            uint32_t imm8 = strtoul(pl->tokens[2], NULL, 0) & 0xFF;
            payload = ((rd & 0xF) << RD_SHIFT) | ((rs & 0xF) << RS_SHIFT) | ((imm8 & 0xFF) << 8);
            break;
        }
        case OP_LOAD:  // LOAD rd, addr
        case OP_STORE: // STORE rs, addr
        {
            if (pl->ntok < 2)
                break;
            int rA = parseRegister(pl->tokens[0]); // rd o rs
            uint32_t imm20 = strtoul(pl->tokens[1], NULL, 0) & 0xFFFFF;
            payload = ((rA & 0xF) << RD_SHIFT) | imm20;
            break;
        }
        case OP_JMP:
        case OP_JEQ:
        case OP_JNE:
        case OP_CALL:
        {
            // potremmo avere un immediate o una label
            if (pl->ntok < 1)
                break;
            // check se token[0] è label => tentiamo find_label_address
            // se non trovato => memorizziamo successivamente => 2 pass
            // Invece, in questo codice, stiamo nel 2 pass => le label ci sono
            int addr = find_label_address(pl->tokens[0]);
            if (addr < 0)
            {
                // Non è una label => interpretalo come numero
                addr = (int)strtoul(pl->tokens[0], NULL, 0);
            }
            // max 24 bit
            payload = (addr & 0xFFFFFF);
            break;
        }
        case OP_PUSH: // PUSH rs
        {
            if (pl->ntok < 1)
                break;
            int rs = parseRegister(pl->tokens[0]);
            payload = ((rs & 0xF) << RD_SHIFT);
            break;
        }
        case OP_POP: // POP rd
        {
            if (pl->ntok < 1)
                break;
            int rd = parseRegister(pl->tokens[0]);
            payload = ((rd & 0xF) << RD_SHIFT);
            break;
        }
        case OP_RET:
        {
            // nessun parametro
            break;
        }
        case OP_DRAW: // DRAW rx, ry, rc
        {
            if (pl->ntok < 3)
                break;
            int rx = parseRegister(pl->tokens[0]);
            int ry = parseRegister(pl->tokens[1]);
            int rc = parseRegister(pl->tokens[2]);
            payload = ((rx & 0xF) << RD_SHIFT) | ((ry & 0xF) << RS_SHIFT) | ((rc & 0xF) << RT_SHIFT);
            break;
        }
        default:
            // OP_UNKNOWN
            break;
        }

        uint32_t final = make_instr(opcode, payload);
        fwrite(&final, sizeof(uint32_t), 1, fout);
    }

    fclose(fout);
    return 0;
}

int assemble_file(const char *input_filename, const char *output_filename)
{
    // Inizializza strutture
    memset(g_labels, 0, sizeof(g_labels));
    g_labelCount = 0;
    memset(g_parsedLines, 0, sizeof(g_parsedLines));
    g_lineCount = 0;

    FILE *fin = fopen(input_filename, "r");
    if (!fin)
    {
        fprintf(stderr, "Impossibile aprire %s\n", input_filename);
        return 1;
    }

    // PRIMO PASS
    first_pass(fin);

    fclose(fin);

    // SECONDO PASS => genera binario
    return second_pass(input_filename, output_filename);
}
