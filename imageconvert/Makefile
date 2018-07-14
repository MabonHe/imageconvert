CXX=$(CROSS_COMPILE)g++
AR=$(CROSS_COMPILE)ar

#CXXFLAGS= -g -I/usr/include -I./utils/ -I../src -I/usr/include/libcamhal/ `pkg-config --cflags gstreamer-1.0 glib-2.0 gstreamer-video-1.0 gtk+-3.0`
#LDFLAGS= -L/usr/lib64/ -lcamhal -lpthread `pkg-config --libs  gstreamer-1.0 gobject-2.0  glib-2.0 gstreamer-video-1.0 gtk+-3.0`

CXXFLAGS= -I/usr/local/include -L/usr/local/lib  `pkg-config --cflags --libs opencv ` -g -std=c++11
LDFLAGS= -I/usr/local/include -L/usr/local/lib ` pkg-config --cflags --libs opencv ` -g -std=c++11

casedir=.

srcobjs=$(patsubst %.cpp, %.o, $(shell ls $(casedir)/*.cpp))

mainobj=main.o

testbin=imageconvert

all: $(testbin)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(testbin): $(mainobj) $(srcobjs)
	$(CXX) -o $@ $^ $(LDFLAGS)
.PHONY : clean
clean:
	rm -f $(mainobj) $(srcobjs) $(testbin)
