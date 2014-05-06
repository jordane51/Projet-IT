PROGRAMS=evaluation
TESTS=test_automate test_ensemble test_table

CPPFLAGS=-g -O0 -Wall -Werror
CFLAGS=
LDFLAGS= -lm

all: $(PROGRAMS) $(TESTS) 

evaluation: evaluation.o libautomate.a

test_table: test_table.o libautomate.a
test_automate: test_automate.o libautomate.a
test_ensemble: test_ensemble.o libautomate.a

libautomate.a: libautomate.a(automate.o table.o ensemble.o avl.o fifo.o outils.o)

clean:
	-rm -rf *.o
	-rm -rf *.a
	-rm -rf $(TESTS)
	-rm -rf $(PROGRAMS)

.PHONY: all clean
