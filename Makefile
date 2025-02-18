OBJS = compdetect_client.o preprobing.o
PROGS = compdetect_client
LDFLAGS = -lcjson

%.o: %.c client.h
	gcc -c -g -o $@ $< 

$(PROGS): $(OBJS)
	gcc -g -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJS) $(PROGS)
