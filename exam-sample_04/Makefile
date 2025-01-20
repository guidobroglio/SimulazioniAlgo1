CFLAGS += -Wall -Wextra -std=c11 -pedantic
CFLAGS += -g
CFLAGS += -I./include
LDFLAGS += -L./lib
LDLIBS += -lupoalglib -lm


.PHONY: all clean

all: test/bst_subtree_max test/ht_sepchain_perc_collisions

test/bst_subtree_max: test/bst_subtree_max.o exam.o
test/ht_sepchain_perc_collisions: test/ht_sepchain_perc_collisions.o exam.o

clean:
	$(RM) test/bst_subtree_max
	$(RM) test/ht_sepchain_perc_collisions
	$(RM) *.o test/*.o
	$(RM) vgcore.* test/vgcore.*
