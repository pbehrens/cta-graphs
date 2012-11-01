ifndef R_HOME
 R_HOME=/home2/FlowData/AltProjects/R-shared
endif

PYTHON_INCLUDES=/usr/local/include/python1.5
CFLAGS=-I$(PYTHON_INCLUDES) -I$(R_HOME)/include

RS.so: rnorm.o
	$(CC) -shared -o $@ rnorm.o -lR -L$(R_HOME)/bin


PYGTK_DIR=$(HOME)/pygtk-0.6.11
RGTK_DIR=$(HOME)/Rpackage/RGtk
RGTK_INCLUDE=$(RGTK_DIR)/include
RGTK_LIB=-L$(RGTK_DIR)/libs -lRGtk
PYTHON_INCLUDE=/usr/local/include/python2.4
PYTHON_LIB=-L/usr/local/lib/python2.4/config -lpython2.4 -lpthread -lutil

gtkConversion.so: gtkConversion.c
	(PKG_CPPFLAGS="-DHAVE_CONFIG_H -I.. -I$(PYGTK_DIR) -I$(RGTK_INCLUDE) -I$(PYTHON_INCLUDE) -D_R_=1 -DUSE_R=1 `gtk-config --cflags`"; PKG_LIBS="$(RGTK_LIB) $(PYTHON_LIB) `gtk-config --libs`" ; export PKG_CPPFLAGS ; export PKG_LIBS ; R CMD SHLIB -o $@  $^)
