# Master Makefile


all: libmsocket.a initmsocket user1 user2

libmsocket.a:
    $(MAKE) -f Makefile_lib.mak

initmsocket:
    $(MAKE) -f Makefile_app.mak initmsocket

user1:
    $(MAKE) -f Makefile_app.mak user1

user2:
    $(MAKE) -f Makefile_app.mak user2

clean:
    $(MAKE) -f Makefile_lib clean
    $(MAKE) -f Makefile_app.mak clean
