TOPDIR=${HOME}


checksetup:
	echo 'TOPDIR is set to' ${TOPDIR}

all: postbox

postbox: postbox.cc
	g++ postbox.cc -o postbox $(shell root-config --cflags --glibs)
Debug: postbox.cc
	g++ -g postbox.cc -o postbox $(shell root-config --cflags --glibs)
clean:
	rm *.o *.a postbox
install:
	cp *.h ${TOPDIR}/include/.
	cp *.a ${TOPDIR}/lib/.
	cp postbox ${TOPDIR}/bin/.

