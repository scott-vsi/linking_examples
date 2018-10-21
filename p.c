void d1();
void d2();

int main()
{
  d1();
  d2();
  return 0;
}

// output:
#if 0
base version 2> init
base version 1> init
d1> init
d2> init
d1()
 d2_print()
  base version 2; called from: d1
d2()
 d1_print()
  base version 1; called from: d2
d2> fini
d1> fini
base version 1> fini
base version 2> fini
#endif