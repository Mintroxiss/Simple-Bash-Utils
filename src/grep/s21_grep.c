#include "s21_grep.h"

int main(int argc, char **argv) {
  Flags flags = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  char *regExps = (char *)malloc(REG_SIZE * sizeof(char));
  regExps[0] = '\0';
  int stopper = optionsParser(argc, argv, &flags, regExps);
  if (!stopper) {
    optionsWork(argc, argv, flags, regExps);
  }
  free(regExps);
  return 0;
}

// занесение опций в структуру
int optionsParser(int argc, char **argv, Flags *flags, char *regExps) {
  int stopper = 0;
  int opt;
  struct option longOpt[] = {{0, 0, 0, 0}};
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", longOpt, NULL)) != -1 && !stopper) {
    switch (opt) {
      case 'e':
        strcat(regExps, remUnChar(optarg));
        regExps[strlen(regExps)] = '|';
        regExps[strlen(regExps)] = '\0';
        break;
      case 'i':
        flags->flI = 1;
        break;
      case 'v':
        flags->flV = 1;
        break;
      case 'c':
        flags->flC = 1;
        break;
      case 'l':
        flags->flL = 1;
        break;
      case 'n':
        flags->flN = 1;
        break;
      case 'h':
        flags->flH = 1;
        break;
      case 's':
        flags->flS = 1;
        break;
      case 'f':
        stopper = fillRegExps(regExps, optarg);
        break;
      case 'o':
        flags->flO = 1;
        break;
      case '?':
        stopper = 1;
        break;
    }
  }
  flags->fileInd = optind;
  if (argc - flags->fileInd < 1) {
    stopper = 1;
  }
  if (strlen(regExps) == 0 && !stopper) {
    strcat(regExps, remUnChar(argv[flags->fileInd]));
    flags->fileInd += 1;
  } else {
    regExps[strlen(regExps) - 1] = '\0';
  }
  if (!stopper) {
    stopper = compatCheckFl(flags);
  }
  return stopper;
}

// избавление от бэкслэша перед пайпом
char *remUnChar(char *newArg) {
  int size = strlen(newArg);
  int j = 0;
  for (int i = 0; i < size; i++) {
    if (!(newArg[i] == '\\' && newArg[i + 1] == '|') && newArg[i] != '\r') {
      newArg[j] = newArg[i];
      j++;
    }
  }
  newArg[j] = '\0';
  return newArg;
}

// заполнение строки с рег. выражениями
int fillRegExps(char *regExps, char *fileWithRegs) {
  int stopper = 0;
  FILE *file = fopen(fileWithRegs, "r");
  if (file == NULL) {
    printf("Файл \"%s\" отсутствует в директории...\n", fileWithRegs);
    stopper = 1;
  }
  char newRegExp[BUFFER_SIZE];
  while (fgets(newRegExp, BUFFER_SIZE, file) != NULL && !stopper) {
    strcat(regExps, remUnChar(newRegExp));
    regExps[strlen(regExps) - 1] = '|';
  }
  fclose(file);
  return stopper;
}

// реализация иерархии опций
int compatCheckFl(Flags *flags) {
  int stopper = 0;
  if (flags->flL) {
    flags->flC = 0;
    flags->flN = 0;
    flags->flO = 0;
  }
  if (flags->flC) {
    flags->flO = 0;
    flags->flN = 0;
  }
  if (flags->flV && flags->flO) {
    stopper = 1;
  }
  return stopper;
}

// вывод в консоль
void optionsWork(int argc, char **argv, Flags flags, char *regExps) {
  int outNameFile = 0;
  if (argc - flags.fileInd > 1 && !flags.flH) {
    outNameFile = 1;
  }
  while (flags.fileInd < argc) {
    FILE *file = fopen(argv[flags.fileInd], "r");
    if (file == NULL) {
      if (!flags.flS) {
        printf("Файл \"%s\" отсутствует в директории...\n",
               argv[flags.fileInd]);
      }
      flags.fileInd += 1;
      continue;
    }
    int num = 1, count = 0;
    char str[BUFFER_SIZE];
    int inpFileName = 0;
    while (fgets(str, BUFFER_SIZE, file) != NULL) {
      if (str[strlen(str) - 1] != '\n') {
        str[strlen(str)] = '\n';
        str[strlen(str) + 1] = '\0';
      }
      int out = 0;
      regex_t reg;
      regmatch_t matches[1];
      int regFlags = REG_EXTENDED;
      if (flags.flI) {
        regFlags |= REG_ICASE;
      }
      int regSearching = regcomp(&reg, regExps, regFlags);
      if (regSearching != 0) {
        regfree(&reg);
        flags.fileInd += 1;
        continue;
      }
      regSearching = regexec(&reg, str, 0, NULL, 0);
      if (regSearching == 0) {
        out = 1;
      }
      int outCond = (flags.flV && !out) || (!flags.flV && out);
      if (outNameFile && !flags.flL && outCond && !flags.flC && !flags.flO) {
        printf("%s:", argv[flags.fileInd]);
      }
      if (flags.flN && outCond) {
        printf("%d:", num);
      }
      if (out && flags.flO) {
        workFlO(str, matches, reg, outNameFile, argv[flags.fileInd]);
      }
      regfree(&reg);
      if (!flags.flC && !flags.flL && outCond && !flags.flO) {
        printf("%s", str);
      }
      if (flags.flL && outCond) {
        inpFileName = 1;
      }
      if (flags.flC && outCond) {
        count++;
      }
      num++;
    }
    if (inpFileName) {
      printf("%s\n", argv[flags.fileInd]);
    }
    if (flags.flC) {
      if (outNameFile) {
        printf("%s:", argv[flags.fileInd]);
      }
      printf("%d\n", count);
    }
    fclose(file);
    flags.fileInd += 1;
  }
}

// реализация опции -o
void workFlO(char *str, regmatch_t *matches, regex_t reg, int outNameFile,
             char *nameFile) {
  int rez = 0;
  char *newStr = str;
  int curOutLen = 0;
  matches[0].rm_so = 0;
  matches[0].rm_eo = 0;
  while (matches[0].rm_so != -1) {
    curOutLen = (int)(matches[0].rm_eo - matches[0].rm_so);
    if (curOutLen != 0) {
      if (outNameFile) {
        printf("%s:", nameFile);
      }
      printf("%.*s\n", curOutLen, &newStr[matches[0].rm_so]);
    }
    newStr = &newStr[matches[0].rm_eo];
    if ((rez = regexec(&reg, newStr, 1, matches, 0)) != 0) {
      matches[0].rm_so = -1;
    }
  }
}