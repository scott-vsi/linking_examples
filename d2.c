#include<stdio.h>

void base_print(int, const char*);
void base_v2_only(const char*);
//void d1_print(const char*);

static
void
__attribute__ ((constructor))
d2_init(void)
{
  printf("d2> init\n");
}

static
void
__attribute__ ((destructor))
d2_fini(void)
{
  printf("d2> fini\n");
}


void d2_print(const char* s)
{
  printf(" d2_print()\n");
  base_print(3, s);
  base_v2_only(s);
}

//void d2()
//{
//  printf("d2()\n");
//  d1_print("d2");
//}


