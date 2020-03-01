CC=gcc
CFLAGS= -c
THREAD_FLAGS= -pthread -lrt -std=c99
MATH_FLAGS= -lm

all: one two three

one: shm msgq

two: mutex sem_sysv

three: signals unnamed_pipe

shm: shm_srv shm_clnt

shm_srv: one/shm_srv.o model/srv.o util/color.o
	$(CC)  $^ -o $@ $(MATH_FLAGS)

shm_clnt: one/shm_clnt.o model/clnt.o util/color.o
	$(CC) $^ -o $@

msgq: msgq_srv msgq_clnt

msgq_srv: one/msgq_srv.o model/srv.o util/color.o
	$(CC) $^ -o $@ $(MATH_FLAGS)

msgq_clnt: one/msgq_clnt.o model/clnt.o util/color.o
	$(CC) $^ -o $@

mutex: two/mutex.o util/color.o model/letters.o
	$(CC) $(THREAD_FLAGS) $^ -o $@

sem_sysv: two/sem_sysv.o util/color.o model/letters.o
	$(CC) $(THREAD_FLAGS) $^ -o $@

signals: three/signals.o model/srv.o
	$(CC) $(THREAD_FLAGS) $^ -o $@ $(MATH_FLAGS)

unnamed_pipe: three/unnamed_pipe.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -lthread -D_XOPEN_SOURCE -o $@ $<

.PHONY: clean

clean:
	rm -f model/*.o util/*.o one/*.o three/*.o two/*.o