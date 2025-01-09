#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

#define MAX_TOKENS 16
#define MAX_LABEL_LEN 64
#define MAX_MNEM_LEN 16
#define MAX_LINE_LEN 256

// Lo scanner si occupa di :

//     Leggere linea per linea un file.asm.Rimuovere i
//     commenti(;...)
//         .Estrarre la eventuale etichetta(label:) e la parte di istruzione
//     / direttiva.Spezzare la parte rimanente in mnemonico e argomenti.

//       // Tipo enumerato per rappresentare i tipi di linee lette
//       typedef enum {
//           LINE_TYPE_INST,   // istruzione (o direttiva come .db)
//           LINE_TYPE_EMPTY,  // riga vuota o solo commenti
//           LINE_TYPE_INVALID // qualunque altra cosa o errore
//       } LineType;

/**
 * Enumerazione che identifica il tipo di riga scansionata.
 */
typedef enum
{
    LINE_TYPE_INST,   // riga con istruzione o direttiva
    LINE_TYPE_EMPTY,  // riga vuota o solo commenti
    LINE_TYPE_INVALID // riga non valida o con errore
} LineType;

/**
 * Struttura che lo scanner popola con le informazioni di una riga.
 */
typedef struct
{
    char label[MAX_LABEL_LEN];   ///< eventuale label trovata (es. "start:")
    char mnemonic[MAX_MNEM_LEN]; ///< es. "MOV", ".db", ...
    char tokens[MAX_TOKENS][32]; ///< argomenti (es. "R0", "1234", "label2")
    int ntok;                    ///< quanti token
    LineType lineType;           ///< tipo di riga
} AsmLine;

/**
 * Legge una riga dal file `fin` e la tokenizza.
 * - Ritorna 1 se è stata letta una riga valida
 * - 0 se EOF
 * - -1 se errore di lettura
 */
int scanner_next_line(FILE *fin, AsmLine *outLine, int *lineno);

#endif // SCANNER_H
