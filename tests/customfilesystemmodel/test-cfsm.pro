TARGET = test-cfsm

HEADERS	+=						\
	testCFSM.h					\
	scenario-num.h					\
	../../src/customfilesystemmodel.h

SOURCES	+= test-cfsm.cpp				\
	testCFSM.cpp					\
	../../src/customfilesystemmodel.cpp

include(../tests-include.pro)
