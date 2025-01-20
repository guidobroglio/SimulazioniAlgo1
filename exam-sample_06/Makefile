CFLAGS += -Wall -Wextra -std=c11 -pedantic
CFLAGS += -g
CFLAGS += -I./include
LDFLAGS += -L./lib -no-pie
LDLIBS += -lupoalglib -lm


.PHONY: all clean

all: test/bst_is_not_empty test/ht_sepchain_is_not_empty

test/bst_is_not_empty: test/bst_is_not_empty.o exam.o
test/ht_sepchain_is_not_empty: test/ht_sepchain_is_not_empty.o exam.o

clean:
	$(RM) test/bst_is_not_empty
	$(RM) test/ht_sepchain_is_not_empty
	$(RM) *.o test/*.o
