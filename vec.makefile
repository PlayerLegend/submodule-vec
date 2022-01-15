C_PROGRAMS += test/vec
C_PROGRAMS += test/mat4
RUN_TESTS += test/vec
#RUN_TESTS += test/mat4

vec-tests: test/vec
vec-tests: test/mat4

test/vec: LDLIBS += -lm
test/vec: src/vec/test/vec.test.o

test/mat4: LDLIBS += -lm
test/mat4: src/vec/test/mat4.test.o
test/mat4: src/vec/mat4.o
test/mat4: src/log/log.o

tests: vec-tests

