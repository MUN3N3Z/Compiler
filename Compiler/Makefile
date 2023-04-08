FILES = Lexer.c Parser.c Expression.c constfolding.c constprop.c deadassign.c optimizer.c codegen.c main.c symtab.c
CC = gcc
CFLAGS = -g -ansi -std=gnu11
TESTCASES = tests
TESTASSEMBLY = testassembly

323compiler: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o 323compiler

testopt: clean 323compiler
	@for i in $(shell ls ${TESTCASES}); do \
		./323compiler ${TESTCASES}/$${i} > test.ir; \
		cp goldIR/$${i}.ir gold.ir; \
		cmp -s test.ir gold.ir; \
		RETVALOPT=$$?; \
		if [ $$RETVALOPT -eq 0 ]; then\
			echo $${i} "OPTIMIZATION PASS"; \
		else \
			echo $${i} "OPTIMIZATION FAIL"; \
		fi \
	done;
	@rm gold.ir
	@rm test.ir

testcg: clean 323compiler
	@for i in $(shell ls ${TESTCASES}); do \
		./323compiler ${TESTCASES}/$${i} > dummy.ir; \
		gcc ${TESTASSEMBLY}/main.c assembly.s -o test; \
		./test > test.cg; \
		gcc -S ${TESTCASES}/$${i} -o temp.s; \
		gcc ${TESTASSEMBLY}/main.c temp.s -o gold; \
		./gold > gold.cg; \
		cmp -s test.cg gold.cg; \
		RETVALCG=$$?; \
		if [ $$RETVALCG -eq 0 ]; then\
			echo $${i} "CODEGEN PASS"; \
		else \
			echo $${i} "CODEGEN FAIL"; \
		fi \
	done;	
	@rm dummy.ir
	@rm gold.cg
	@rm test.cg
all: testopt testcg
clean:
	rm -f *.o *~ 323compiler gold test *.s a.out *.ir *.cg
