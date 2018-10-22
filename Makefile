# i have no idea how Makefiles work

BUILD=build
CFLAGS=-fPIC -Wall -pedantic
LD=gcc
LDFLAGS=-Wl,-rpath=$(BUILD)

all: $(BUILD)/libd1.so $(BUILD)/libd2.so p

%.png: %.dot
	dot -Tpng -o $@ $^

$(BUILD)/%.o: %.c
	$(CC) -fPIC -Wall -pedantic   -c -o $@ $<

# Base library version 1
$(BUILD)/base1/libbase.so.1.0: $(BUILD)/base1.o
	$(LD) -shared \
		-Wl,-soname,libbase.so \
		-o $@ $<
	ln -rfs $(BUILD)/base1/libbase.so.1.0 $(BUILD)/base1/libbase.so.1  # library to link with
	ln -rfs $(BUILD)/base1/libbase.so.1 $(BUILD)/base1/libbase.so

# Base library version 2
$(BUILD)/base2/libbase.so.2.0: $(BUILD)/base2.o
	$(LD) -shared \
		-Wl,-soname,libbase.so \
		-o $@ $<
	ln -rfs $(BUILD)/base2/libbase.so.2.0 $(BUILD)/base2/libbase.so.2  # library to link with
	ln -rfs $(BUILD)/base2/libbase.so.2 $(BUILD)/base2/libbase.so

# libd1.so:
# * Uses libbase. Compiled against libbase.so.1
# * provides versioned symbols
$(BUILD)/libd1.so: $(BUILD)/d1.o $(BUILD)/base1/libbase.so.1.0
	ldconfig -N -f ld.so.conf
	$(LD) -Wl,-rpath=$(BUILD)/base1 -L./$(BUILD) -L./$(BUILD)/base1 \
		-Wl,-soname,libd1.so \
		-shared -o $@ $< -lbase
	#rm $(BUILD)/base1/libbase.so

# libd2.so:
# * Uses libbase. Compiled against libbase.so.2
# * provides unversioned symbols
$(BUILD)/libd2.so: $(BUILD)/d2.o $(BUILD)/base2/libbase.so.2.0 $(BUILD)/libd1.so
	ldconfig -N -f ld.so.conf
	$(LD) -Wl,-rpath=$(BUILD)/base2 -L./$(BUILD) -L./$(BUILD)/base2 \
		-Wl,-soname,libd2.so \
		-shared -o $@ $< -lbase
	#rm $(BUILD)/base2/libbase.so

# p: Test program that depends on both libd1 and dynamically loads libd2.
p: $(BUILD)/p.o
	$(CC) $(LDFLAGS) -o $(BUILD)/p $(BUILD)/p.o -L$(BUILD) -Wl,-rpath=$(BUILD) -ld1 -ldl

clean:
	rm -f $(BUILD)/*.o $(BUILD)/*.so $(BUILD)/base[12]/*.so* $(BUILD)/*.[012] $(BUILD)/*~ $(BUILD)/p $(BUILD)/*.png
