#include<stdio.h>
#include <dlfcn.h>

int main()
{
  void *handle1;
  void (*func_d1_print)(const char*);
  void *handle2;
  void (*func_d2_print)(const char*);

  printf("dlopen called\n");
  handle1 = dlopen("libd1.so", RTLD_NOW);
  if (!handle1) {
      /* fail to load the library */
      printf("Error: %s\n", dlerror());
      return -1;
  }
  *(void**)(&func_d1_print) = dlsym(handle1, "d1_print");
  if (!func_d1_print) {
      /* no such symbol */
      printf("Error: %s\n", dlerror());
      dlclose(handle1);
      return -1;
  }

  handle2 = dlopen("libd2.so", RTLD_NOW);
  if (!handle2) {
      /* fail to load the library */
      printf("Error: %s\n", dlerror());
      return -1;
  }
  *(void**)(&func_d2_print) = dlsym(handle2, "d2_print");
  if (!func_d2_print) {
      /* no such symbol */
      printf("Error: %s\n", dlerror());
      dlclose(handle2);
      return -1;
  }

  func_d1_print("d1"); // d1.so -> libbase.so [specifically, libbase.so.1]
  func_d2_print("d2"); // d2.so -> libbase.so [specifically, libbase.so.2]

  dlclose(handle1);
  dlclose(handle2);
  return 0;
}

// output:
#if 0
dlopen called
base version 1> init
d1> init
Error: build/libd2.so: undefined symbol: base_v2_only
d1> fini
base version 1> fini
#endif
