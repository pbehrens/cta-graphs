#R_HOME=$(shell R RHOME)

PYTHON_INCLUDES=/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7

PIC_FLAGS=-fPIC

CFLAGS=$(PIC_FLAGS) -g -I$(PYTHON_INCLUDES) -I$(R_HOME)/include
OBJS=$(shell find src -name '*.o')

lib RSInternal.so: $(OBJS)
	$(CC) -shared -o /libs/RSInternal.so $(OBJS) -lR -L$(R_HOME)/lib
