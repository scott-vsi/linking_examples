#include<stdio.h>
#include <dlfcn.h>

void d1_print();

int main()
{
  void *handle;
  void (*func_d2_print)(const char*);

  printf("dlopen called\n");
  handle = dlopen("libd2.so", RTLD_NOW);
  if (!handle) {
      /* fail to load the library */
      printf("Error: %s\n", dlerror());
      return -1;
  }
  *(void**)(&func_d2_print) = dlsym(handle, "d2_print");
  if (!func_d2_print) {
      /* no such symbol */
      printf("Error: %s\n", dlerror());
      dlclose(handle);
      return -1;
  }

  d1_print("d1"); // d1.so -> libbase.so [specifically libbase.so.1]
  func_d2_print("d2"); // d2.so -> libbase.so [specifically libbase.so.2]

  dlclose(handle);
  return 0;
}

// output:
#if 0
base version 1> init
d1> init
dlopen called
d2> init
 d1_print()
  base version 1; called from: d1
 d2_print()
Segmentation fault
#endif
