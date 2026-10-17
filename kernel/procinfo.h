#pragma once

#define PROCINFO_NAME_MAX 16

enum procinfo_state {
  PROCINFO_UNUSED   = 0,
  PROCINFO_USED     = 1,
  PROCINFO_SLEEPING = 2,
  PROCINFO_RUNNABLE = 3,
  PROCINFO_RUNNING  = 4,
  PROCINFO_ZOMBIE   = 5,
};

struct procinfo {
  int  pid;
  char name[PROCINFO_NAME_MAX];
  enum procinfo_state state;
  int  ppid;
};