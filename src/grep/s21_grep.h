#define REG_SIZE 999999
#define BUFFER_SIZE 9999

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int flI;  // Игнорирует различия регистра.
  int flV;  // Инвертирует смысл поиска соответствий.
  int flC;  // Выводит только количество совпадающих строк.
  int flL;  // Выводит только совпадающие файлы.
  int flN;  // Предваряет каждую строку вывода номером строки из файла ввода.
  int flH;  // Выводит совпадающие строки, не предваряя их именами файлов.
  int flS;  // Подавляет сообщения об ошибках о несуществующих или нечитаемых
            // файлах.
  int flO;  // Печатает только совпадающие (непустые) части совпавшей строки.
  int fileInd;
} Flags;

int optionsParser(int argc, char **argv, Flags *flags, char *regExps);
int fillRegExps(char *regExps, char *fileWithRegs);
int compatCheckFl(Flags *flags);
void optionsWork(int argc, char **argv, Flags flags, char *regExps);
char *remUnChar(char *newArg);
void workFlO(char *str, regmatch_t *matches, regex_t reg, int outNameFile,
             char *nameFile);
