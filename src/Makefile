APP=hello-gtk
SRC=main.c

all:
	cd /work/src
	$(CC) $(SRC) -o $(APP) `pkg-config --cflags --libs gtk+-3.0`

run: all
	cd /work/src
	./$(APP)

clean:
	cd /work/src
	rm -f $(APP)
