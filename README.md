Experiment with dynamic symbol resolution across dynamically linked and loaded ELF libraries

TLDR;

- Dynamically linked dependencies are scanned in a breadth-first search according to their order on the link line.
- Dynamically loaded dependencies (`dlopen`) are, by default, added to a private link chain.


Compiling and running
---------------------
```bash
$ make && ./p # see p.c for expected output
```

1. `e58d5c6` works; [compatible definitions]
    ```
    p -> libd1 -> libbase.so.1 [duplicate]
      libd2.so -> libbase.so.2 [duplicate]
    ```

1. `a774a61` works; [compatible definitions]<br/>
    RE dynamic linker (ld.so) won't reload libbase.so[.2] since it has already been loaded
    ```
    p -> libd1 -> libbase.so [duplicate] # linked against libbase.so.1
      libd2.so -> libbase.so [duplicate] # linked against libbase.so.2
    ```

1. `8e1cfed` won't work; [diamond linking problem with **incompatible** definitions]<br/>
    RE dynamic linker (ld.so) won't reload libbase.so[.2] since it has already been loaded
    ```
    p -> libd1 -> libbase.so [duplicate] # linked against libbase.so.1
      libd2.so -> libbase.so [duplicate] # linked against libbase.so.2
    ```

1. `5f7a95e` won't work; [multiple incompatible definitions in different SONAMEs; global scope is searched first]<br/>
    RE both libbase.so.1 and libbase.so.2 are ultimately loaded<br/>
    RE the fact that the second .so has a different name doesn't actually matter (symbols are not namespaced by library)
    ```
    p -> libd1 -> libbase.so.1 [duplicate]
      libd2.so -> libbase.so.2 [duplicate]
    ```

1. `966d1f6` works - [a la *manylinux* python import]<br/>
    RE dynamic libraries are each loaded into a private scope appended to the global scope and 'duplicate' is not also defined in the global scope
    ```
    p
      libd1.so -> libbase.so.1 [duplicate]
      libd2.so -> libbase.so.2 [duplicate]
    ```

1. `70f3c8c` won't work - [a la python import]<br/>
    RE similar to the diamond linking problem; the dynamic linker (ld.so) won't reload libbase.so[.2] as it has already been loaded
    ```
    p
      libd1.so -> libbase.so [duplicate] # linked against libbase.so.1 (under the SONAME of
                                         # libbase.so) and loaded libbase.so.1
      libd2.so -> libbase.so [duplicate] # linked against libbase.so.2 (under the SONAME of
                                         # libbase.so) *but* libbase.so.2 not loaded
    ```

1. `fa1bab5` won't work; similar to 5f7a95e<br/>
    RE RTLD_GLOBAL adds loaded symbols (including those from dependencies) into the global scope
    ```
    p
      libd1.so (RTLD_GLOBAL) -> libbase.so.1 [duplicate]
      libd2.so -> libbase.so.2 [duplicate]
    ```

1. `482352f` works - dynamically load incompatible libraries
    ```
    p
      libbase.so.1 [duplicate]
      libbase.so.2 [duplicate]
    ```

---
[verilook case]
shouldn't work; [diamond linking problem again]<br/>
RE similar to `70f3c8c`
```
p
  libd1.so -> libbase.so [duplicate]
  libd2.so
    lib3.so -> libbase.so [duplicate]
```

shouldn't work; [diamond linking problem again]<br/>
RE similar to `70f3c8c`
```
p
  libd1.so
    libdynamicK.so -> libbase.so [duplicate]
  libd2.so
    lib3.so -> libbase.so [duplicate]
```
