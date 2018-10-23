ELF link time dependencies are scanned in a breadth-first search according to their order on the link line


1. `e58d5c6` works; [compatible definitions]
    ```
    app -> libdynamicA -> libtwo.so [duplicate]
      libdynamicB.so -> libthree.so [duplicate]
    ```

1. `a774a61` works; [compatible definitions]<br>
    RE dynamic linker (ld.so) won't reload libtwo.so.2 since it has already been loaded
    ```
    app -> libdynamicA -> libtwo.so [duplicate] # linked against libtwo.so.1
      libdynamicB.so -> libtwo.so [duplicate] # linked against libtwo.so.2
    ```

1. `8e1cfed` won't work; [diamond linking problem with *incompatible* definitions]<br/>
    RE dynamic linker (ld.so) won't reload libtwo.so.2 since it has already been loaded
    ```
    app -> libdynamicA -> libtwo.so [duplicate] # linked against libtwo.so.1
      libdynamicB.so -> libtwo.so [duplicate] # linked against libtwo.so.2
    ```

1. `5f7a95e` won't work; [multiple incompatible definitions in different SONAMEs; global scope is searched first]<br/>
    RE both libone.so and libtwo.so are ultimately loaded<br/>
    RE the fact that the second .so has a different name doesn't actually matter (symbols are not namespaced by library)
    ```
    app -> libdynamicA -> libone.so [duplicate]
      libdynamicB.so -> libtwo.so [duplicate]
    ```

1. `966d1f6` works - [a la *manylinux* python import]<br/>
    RE dynamic libraries are each loaded into a private scope appended to the global scope and 'duplicate' is not also defined in the global scope
    ```
    app
      libdynamicA.so -> libtwo.so [duplicate]
      libdynamicB.so -> libthree.so [duplicate]
    ```

1. `70f3c8c` won't work - [a la python import]<br/>
    RE similar to the diamond linking problem; the dynamic linker (ld.so) won't reload libtwo.so[.2] as it has already been loaded (i think)
    ```
    app
      libdynamicA.so -> libtwo.so [duplicate] # linked against libtwo.so.1 (under the SONAME of
                                              # libtwo.so) and loaded libtwo.so.1
      libdynamicB.so -> libtwo.so [duplicate] # linked against libtwo.so.2 (under the SONAME of
                                              # libtwo.so) *but* libtwo.so.2 not loaded
    ```

1. `fa1bab5` won't work; similar to 5f7a95e<br/>
    RE RTLD_GLOBAL adds loaded symbols (including those from dependencies) into the global scope
    ```
    app
      libdynamicA.so (RTLD_GLOBAL) -> libtwo.so [duplicate]
      libdynamicB.so -> libthree.so [duplicate]
    ```

---
[verilook case]
shouldn't work; [diamond linking problem again]<br/>
RE similar to `70f3c8c`
```
app
  libdynamicA.so -> libtwo.so [duplicate]
  libdynamicB.so
    libdynamicJ.so -> libtwo.so [duplicate]
```

shouldn't work; [diamond linking problem again]<br/>
RE similar to `70f3c8c`
```
app
  libdynamicA.so
    libdynamicK.so -> libtwo.so [duplicate]
  libdynamicB.so
    libdynamicJ.so -> libtwo.so [duplicate]
```
