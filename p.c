#include<stdio.h>
#include <dlfcn.h>

int main()
{
  void *handle1;
  void (*func_base1_print)(const char*);
  void *handle2;
  void (*func_base2_print)(int, const char*);

  printf("dlopen called\n");
  // dlopen treats the filename as a pathname if it contains a slash ('/')
  // (ignoring the rpath)
  handle1 = dlopen("build/base1/libbase.so", RTLD_NOW);
  if (!handle1) {
      /* fail to load the library */
      printf("Error: %s\n", dlerror());
      return -1;
  }
  *(void**)(&func_base1_print) = dlsym(handle1, "base_print");
  if (!func_base1_print) {
      /* no such symbol */
      printf("Error: %s\n", dlerror());
      dlclose(handle1);
      return -1;
  }

  handle2 = dlopen("build/base2/libbase.so", RTLD_NOW);
  if (!handle2) {
      /* fail to load the library */
      printf("Error: %s\n", dlerror());
      return -1;
  }
  *(void**)(&func_base2_print) = dlsym(handle2, "base_print");
  if (!func_base2_print) {
      /* no such symbol */
      printf("Error: %s\n", dlerror());
      dlclose(handle2);
      return -1;
  }

  func_base1_print("b1");
  func_base2_print(4, "b2");

  dlclose(handle1);
  dlclose(handle2);
  return 0;
}

// output:
#if 0
dlopen called
base version 1> init
base version 2> init
  base version 1; called from: b1
  base version 2; called from: b2 with extra arg: 4
base version 1> fini
base version 2> fini
#endif
