C_PROGRAMS += test/vec
C_PROGRAMS += test/mat4

run-tests: run-vec-tests
run-vec-tests:
	DEPENDS=vec-tests sh run-tests.sh test/vec test/mat4

depend: vec-depend
vec-depend:
	cdeps src/vec > src/vec/depends.makefile

vec-tests: test/vec
vec-tests: test/mat4

test/vec: LDLIBS += -lm
test/vec: src/vec/test/vec.test.o

test/mat4: LDLIBS += -lm
test/mat4: src/vec/test/mat4.test.o
test/mat4: src/vec/mat4.o
test/mat4: src/log/log.o

tests: vec-tests
