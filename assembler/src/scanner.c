#include "scanner.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static void trim(char *s)
{
    // rimuove spazi iniziali e finali
    char *p = s;
    while (isspace((unsigned char)*p))
        p++;
    if (p != s)
        memmove(s, p, strlen(p) + 1);

    // elimina trailing space
    int len = (int)strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1]))
    {
        s[len - 1] = '\0';
        len--;
    }
}

int scanner_next_line(FILE *fin, AsmLine *outLine, int *lineno)
{
    char linebuf[MAX_LINE_LEN];
    if (!fgets(linebuf, sizeof(linebuf), fin))
    {
        return 0; // EOF
    }

    (*lineno)++;

    // Rimuovi \n
    char *c = strchr(linebuf, '\n');
    if (c)
        *c = '\0';

    // Rimuovi commenti
    char *comment = strchr(linebuf, ';');
    if (comment)
        *comment = '\0';

    // Trim spazi
    trim(linebuf);
    if (linebuf[0] == '\0')
    {
        // riga vuota
        outLine->lineType = LINE_TYPE_EMPTY;
        outLine->mnemonic[0] = '\0';
        outLine->label[0] = '\0';
        outLine->ntok = 0;
        return 1; // riga ok ma vuota
    }

    // inizializza campi
    memset(outLine, 0, sizeof(*outLine));
    outLine->lineType = LINE_TYPE_INST; // di base ipotizziamo sia una istruzione

    // Cerchiamo se c'è un label all'inizio
    // label e' del tipo "foo:" all'inizio
    char *first_space = strpbrk(linebuf, " \t");
    char *maybe_label_end = strchr(linebuf, ':');
    if (maybe_label_end && (!first_space || maybe_label_end < first_space))
    {
        // label presente
        *maybe_label_end = '\0';
        strncpy(outLine->label, linebuf, MAX_LABEL_LEN - 1);

        // il resto della riga e' dopo i due punti
        char *rest = maybe_label_end + 1;
        trim(rest);
        // se vuoto, riga=solo label
        if (*rest == '\0')
        {
            // riga con solo label
            // segniamo comunque lineType=INST (potrebbe non avere opcode)
            outLine->mnemonic[0] = '\0';
            outLine->ntok = 0;
            return 1;
        }
        // altrimenti, in rest c'e' la parte di istruzione
        strncpy(linebuf, rest, sizeof(linebuf) - 1);
        linebuf[sizeof(linebuf) - 1] = '\0';
    }

    // Ora tokenizziamo linebuf come "mnemonic tokens..."
    // splitted by space, comma
    int ntok = 0;
    char tempbuf[MAX_LINE_LEN];
    strncpy(tempbuf, linebuf, sizeof(tempbuf) - 1);
    tempbuf[sizeof(tempbuf) - 1] = '\0';

    char *tk = strtok(tempbuf, " \t,");
    if (!tk)
    {
        // non ci sono token
        outLine->mnemonic[0] = '\0';
        outLine->lineType = LINE_TYPE_EMPTY;
        return 1;
    }
    strncpy(outLine->mnemonic, tk, MAX_MNEM_LEN - 1);

    // Leggi i restanti parametri in outLine->tokens
    while ((tk = strtok(NULL, " \t,")) != NULL)
    {
        if (ntok < MAX_TOKENS)
        {
            strncpy(outLine->tokens[ntok], tk, 31);
            outLine->tokens[ntok][31] = '\0';
            ntok++;
        }
        else
        {
            // troppi token => ignora e/o errore
        }
    }
    outLine->ntok = ntok;

    return 1;
}
