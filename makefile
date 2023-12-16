CC = gcc
SRCAUTO = mempool_autolength.c
SRCFIX = mempool_fixlength.c
OBJAUTO = $(SRCAUTO:.c=.o)
OBJFIX = $(SRCFIX:.c=.o)
TARGETAUTO = mempool_autolength
TARGETFIX = mempool_fixlength

all:$(TARGETAUTO) $(TARGETFIX)

$(TARGETAUTO):$(OBJAUTO)
	gcc -o $@ $^

$(TARGETFIX):$(OBJFIX)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) -c $^ -o $@

clean:
	rm -rf $(TARGETAUTO) $(TARGETFIX) $(OBJAUTO) $(OBJFIX)