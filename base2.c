#include<stdio.h>

static
void
__attribute__ ((constructor))
base_init(void)
{
  printf("base version 2> init\n");
}

static
void
__attribute__ ((destructor))
base_fini(void)
{
  printf("base version 2> fini\n");
}


void base_print(int n, const char* s)
{
  printf("  base version 2; called from: %s with extra arg: %d\n", s, n);
}

void base_v2_only(const char* s)
{
  printf("  base function only in version 2; called from: %s\n", s);
}
