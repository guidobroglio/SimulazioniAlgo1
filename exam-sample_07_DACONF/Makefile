CFLAGS += -Wall -Wextra -std=c11 -pedantic
CFLAGS += -g
CFLAGS += -I./include
LDFLAGS += -L./lib
LDLIBS += -lupoalglib -lm


.PHONY: all clean

all: test/bst_ksum_range test/ht_linprob_max_collisions

test/bst_ksum_range: test/bst_ksum_range.o exam.o
test/ht_linprob_max_collisions: test/ht_linprob_max_collisions.o exam.o

clean:
	$(RM) test/bst_ksum_range
	$(RM) test/ht_linprob_max_collisions
	$(RM) *.o test/*.o
	$(RM) vgcore.* test/vgcore.*
