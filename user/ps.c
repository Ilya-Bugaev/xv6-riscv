#include "kernel/types.h"
#include "kernel/procinfo.h"
#include "user/user.h"

static const char *statename[] = {"UNUSED",   "USED",    "SLEEPING",
                                  "RUNNABLE", "RUNNING", "ZOMBIE"};

/*
Вспомогательная функция: печатает строку s, затем добивает её пробелами
до ширины width. Нужна для выравнивания столбцов таблицы.
*/
static void
pad(const char *s, int width)
{
  int len = strlen(s);
  printf("%s", s);
  for (; len < width; len++)
    printf(" ");
}

/*
Вспомогательная функция: печатает число x, затем добивает пробелами
до ширины width.
*/
static void
padnum(int x, int width)
{
  int digits = 1;
  for (int t = x; t >= 10; t /= 10)
    digits++;
  printf("%d", x);
  for (int i = digits; i < width; i++)
    printf(" ");
}

int
main(void)
{
  int cap = 4;
  struct procinfo *buf;
  int n;

  /* 
    Итеративно растим буфер, пока все процессы не поместятся.
    Предварительно узнать число процессов нельзя: оно может измениться
    между вызовами, поэтому верхней границей считаем возвращённое n.
    */
  for (;;) {
    buf = malloc(cap * sizeof(struct procinfo));
    if (buf == 0) {
      printf("ps: malloc failed\n");
      exit(1);
    }

    /*
    Системный вызов: просим ядро заполнить buf информацией о процессах.
    Возвращает общее число живых процессов.
    
    */
    n = ps_listinfo((uint64)buf, cap);

    if (n < 0) {
      printf("ps: ps_listinfo failed\n");
      free(buf);
      exit(1);

      if (n <= cap)
        break;

      free(buf);
      cap = n;
    }

    /*
    Печатаем заголовок таблицы. Ширины подобраны так, чтобы
    самые длинные значения выравнивались по левому краю
    */
    pad("PID", 8);
    pad("PPID", 8);
    pad("STATE", 12);
    printf("NAME\n");

    for (int i = 0; i < n; i++) {
      struct procinfo *p = &buf[i];
      padnum(p->pid, 8);
      padnum(p->ppid, 8);
      pad(statename[p->state], 12);
      printf("%s\n", p->name);
    }

    free(buf);
    exit(0);
  }
}