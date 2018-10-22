void d1_print();
void d2_print();

int main()
{
  d1_print("d1"); // d1.so -> libbase.so.1
  d2_print("d2"); // d2.so -> libbase.so.2
  return 0;
}

// output:
#if 0
base version 2> init
base version 1> init
d2> init
d1> init
 d1_print()
  base version 1; called from: d1
 d2_print()
  base version 1; called from: d2
d1> fini
d2> fini
base version 1> fini
base version 2> fini
#endif
