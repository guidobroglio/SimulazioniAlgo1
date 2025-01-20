CFLAGS += -Wall -Wextra -std=c11 -pedantic
CFLAGS += -g
CFLAGS += -I./include
LDFLAGS += -L./lib -no-pie
LDLIBS += -lupoalglib -lm


.PHONY: all clean

all: test/bst_dup_key_insert test/ht_sepchain_merge

test/bst_dup_key_insert: test/bst_dup_key_insert.o exam.o
test/ht_sepchain_merge: test/ht_sepchain_merge.o exam.o

clean:
	$(RM) test/bst_dup_key_insert
	$(RM) test/ht_sepchain_merge
	$(RM) *.o test/*.o
