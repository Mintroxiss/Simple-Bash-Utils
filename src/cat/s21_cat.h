#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int nonBlank, number, showEnds, nonPrinting, squeezeBlank, showTabs, fileInd;
} Flags;

int outFile(char *fileName, Flags flags, int *num, char *pastSymb);
void optionsParser(int argc, char **argv, Flags *flags);
int optionsWork(Flags flags, char symb, int *backSlashN, int iter,
                char pastSymb, int *num, int *out);
void printNonPrinting(char symb, int showTabs);