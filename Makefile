OBJS = compdetect_client.o preprobing.o probing.o postprobing.o payload_generator.o
PROGS = compdetect_client
LDFLAGS = -lcjson

%.o: %.c client.h payload_generator.h
	gcc -c -g -o $@ $< 

$(PROGS): $(OBJS)
	gcc -g -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJS) $(PROGS)
