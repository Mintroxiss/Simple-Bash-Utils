#include "s21_cat.h"

int main(int argc, char **argv) {
  Flags flags = {0, 0, 0, 0, 0, 0, 0};
  optionsParser(argc, argv, &flags);
  int num = 1;
  char pastSymb = ' ';
  while (flags.fileInd < argc) {
    if (outFile(argv[flags.fileInd], flags, &num, &pastSymb) == 1)
      printf("Файл \"%s\" отсутствует в директории\n", argv[flags.fileInd]);
    flags.fileInd += 1;
  }
  return 0;
}

void optionsParser(int argc, char **argv, Flags *flags) {
  int opt;
  const struct option longOptions[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, "bEnsTetv", longOptions, NULL)) != -1) {
    switch (opt) {
      case 'b':
        flags->nonBlank = 1;
        flags->number = 1;
        break;
      case 'E':
        flags->showEnds = 1;
        break;
      case 'e':
        flags->showEnds = 1;
        flags->nonPrinting = 1;
        break;
      case 'n':
        flags->number = 1;
        break;
      case 's':
        flags->squeezeBlank = 1;
        break;
      case 'T':
        flags->showTabs = 1;
        break;
      case 't':
        flags->showTabs = 1;
        flags->nonPrinting = 1;
        break;
      case 'v':
        flags->nonPrinting = 1;
        break;
      case '?':
        exit(1);
        break;
    }
  }
  flags->fileInd = optind;
}

int outFile(char *fileName, Flags flags, int *num, char *pastSymb) {
  int fl = 0;
  FILE *file = fopen(fileName, "r");
  if (file != NULL) {
    char symb;
    int out = 1;
    int contCheck = 1;
    int iter = 0;
    int backSlashN = 0;
    while ((symb = fgetc(file)) != EOF) {
      iter++;
      contCheck =
          optionsWork(flags, symb, &backSlashN, iter, *pastSymb, num, &out);
      if (out && contCheck) {
        fputc(symb, stdout);
      } else {
        out = 1;
        contCheck = 1;
      }
      *pastSymb = symb;
    }
    fclose(file);
  } else
    fl = 1;
  return fl;
}

int optionsWork(Flags flags, char symb, int *backSlashN, int iter,
                char pastSymb, int *num, int *out) {
  int stopper = 0;
  if (flags.squeezeBlank) {
    if (pastSymb == '\n' && iter == 1) {
      stopper = 1;
    }
    if (symb == '\n') {
      *backSlashN += 1;
      if (*backSlashN > 2) {
        *backSlashN = 2;
        stopper = 1;
      } else if (iter == 2 && *backSlashN == 2) {
        stopper = 1;
      }
    } else {
      *backSlashN = 0;
    }
  }
  if (!stopper) {
    if (flags.nonBlank) {
      if (iter == 1 && symb != '\n') {
        printf("%6d\t", *num);
        *num += 1;
      }
      if (pastSymb == '\n' && symb != '\n') {
        printf("%6d\t", *num);
        *num += 1;
      }
    }
    if (flags.number && !flags.nonBlank) {
      if (iter == 1) {
        printf("%6d\t", *num);
        *num += 1;
      }
      if (pastSymb == '\n') {
        printf("%6d\t", *num);
        *num += 1;
      }
    }
    if (flags.showEnds) {
      if (symb == '\n') {
        printf("$");
      }
    }
    if (flags.showTabs) {
      if (symb == '\t') {
        printf("^I");
        *out = 0;
      }
    }
    if (flags.nonPrinting && symb != '\n' && symb != '\t') {
      printNonPrinting(symb, flags.showTabs);
      *out = 0;
    }
  }
  return !stopper;
}

void printNonPrinting(char symb, int showTabs) {
  if (symb & 128) {
    printf("M-");
    symb &= ~128;
  }

  if (symb == 9) {
    printf("%c", symb);
  } else if (symb == 10 && !showTabs) {
    printf("%c", symb);
  } else if (symb < 32) {
    printf("^%c", symb + 64);
  } else if (symb < 127) {
    printf("%c", symb);
  } else {
    printf("^?");
  }
}
