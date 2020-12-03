CFLAGS += -std=c99 -iquote -Wall -Wextra -Og -ggdb
CFLAGS += -Wfloat-equal -Wparentheses -Wmisleading-indentation -Wmissing-field-initializers
CFLAGS += -Werror=incompatible-pointer-types -Werror=discarded-qualifiers -Werror=int-conversion -Werror=div-by-zero -Werror=sequence-point -Werror=uninitialized -Werror=duplicated-cond -Werror=return-type -Werror=implicit-function-declaration -Werror=implicit-int

water-sdl: water-sdl.c
	$(CC) $(CFLAGS) -lSDL2 -lm -o water-sdl water-sdl.c
