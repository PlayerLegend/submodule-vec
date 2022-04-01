C_PROGRAMS += test/vec
C_PROGRAMS += test/mat4

run-tests: run-vec-tests
run-vec-tests:
	DEPENDS=vec-tests sh run-tests.sh test/vec test/mat4

depend: vec-depend
vec-depend:
	sh makedepend.sh src/vec/vec.makefile

vec-tests: test/vec
vec-tests: test/mat4

test/vec: LDLIBS += -lm
test/vec: src/vec/test/vec.test.o

test/mat4: LDLIBS += -lm
test/mat4: src/vec/test/mat4.test.o
test/mat4: src/vec/mat4.o
test/mat4: src/log/log.o

tests: vec-tests
# DO NOT DELETE

src/vec/view.o: src/vec/vec.h src/vec/vec3.h src/vec/vec4.h src/vec/view.h
src/vec/view.o: src/keyargs/keyargs.h src/vec/mat4.h
src/vec/vec3.o: src/vec/vec.h src/vec/vec3.h
src/vec/test/mat4.test.o: src/vec/vec.h src/vec/vec3.h src/keyargs/keyargs.h
src/vec/test/mat4.test.o: src/vec/vec4.h src/vec/mat4.h src/log/log.h
src/vec/test/vec.test.o: src/vec/vec.h src/vec/vec2.h src/vec/vec3.h
src/vec/mat4.o: src/vec/vec.h src/vec/vec2.h src/vec/vec3.h src/vec/vec4.h
src/vec/mat4.o: src/keyargs/keyargs.h src/vec/mat4.h src/log/log.h
src/vec/vec4.o: src/vec/vec.h src/vec/vec3.h src/vec/vec4.h
