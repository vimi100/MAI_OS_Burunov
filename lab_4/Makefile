MATH_FLAG = -lm
PIC_FLAG = -fPIC
SHARED_FLAG = -shared
DYNAMIC_LOADING_FLAG = -ldl
EXT = c
CC = gcc
CFLAGS = -std=c99 -pedantic -Wall


all:  stat_main dynamic dynamic_realization1
# stat_main: stat_main.$(EXT) first_realization.$(EXT) function.h
# 	$(CC) $(CFLAGS) stat_main.$(EXT) first_realization.$(EXT) -o stat_main $(MATH_FLAG)

# stat_main : stat_main.o dynamic_realization1
# 		$(COMPILER) $(COMPILER_FLAGS) stat_main.o -L. -lfirst_realization -o stat_main $(MATH_FLAG)

stat_main: stat_main.$(EXT) dynamic_realization1 function.h
	$(CC) $(CFLAGS) stat_main.$(EXT) -L. ./libfirst_realization.so -o stat_main $(MATH_FLAG)

# stat_main: stat_main.c
# 	$(CC) $(CFLAGS) -c stat_main.c

dynamic: dyn_main dynamic_realization1 dynamic_realization2
	$(CC) $(CFLAGS) dyn_main.o -o dynamic $(DYNAMIC_LOADING_FLAG)

dyn_main : dyn_main.$(EXT)
	$(CC) $(CFLAGS) -c dyn_main.$(EXT)

dynamic_realization2: second_realization.$(EXT) function.h
	$(CC) $(CFLAGS) $(PIC_FLAG) $(SHARED_FLAG) -o libsecond_realization.so second_realization.$(EXT) $(MATH_FLAG)

dynamic_realization1: first_realization.$(EXT) function.h
	$(CC) $(CFLAGS) $(PIC_FLAG) $(SHARED_FLAG) -o libfirst_realization.so first_realization.$(EXT) $(MATH_FLAG)

clean :
	rm *.o *.so  stat_main dynamic