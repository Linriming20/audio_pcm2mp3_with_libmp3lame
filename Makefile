src := main.c
static_lib := lib/libmp3lame.a

CC := gcc
CFLAGS := -I./include
LDFLAGS := -lm


all: pcm2mp3


pcm2mp3: $(src) $(static_lib)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f pcm2mp3 out*
.PHONY := clean
