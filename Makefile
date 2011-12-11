TARGETS = collect train_bow train_svm predict

TEST_SOURCES = \
	tests/test_utils.cpp \
#	tests/test_tar.cpp \

OPENCV_ROOT=/Users/stranger/Works/local
#/home/stranger/stuff/opencv/usr/local

CXX = g++-4.2
CXXFLAGS = -Wall -O2 -g -I${OPENCV_ROOT}/include -I/opt/local/include -pipe #-std=c++0x
LDFLAGS = \
	-L${OPENCV_ROOT}/lib \
	-L/opt/local/lib \
	-lopencv_features2d \
	-lopencv_core \
	-lopencv_imgproc \
	-lopencv_flann \
	-lopencv_highgui \
	-lopencv_ml \
	-lboost_filesystem \
	-lboost_system \

LD_LIBRARY_PATH=${OPENCV_ROOT}/lib
export LD_LIBRARY_PATH
DYLD_LIBRARY_PATH=$(OPENCV_ROOT)/lib
export DYLD_LIBRARY_PATH

collect_SRCS = \
	utils.cpp \
	collect.cpp \

#collect_sift_LDFLAGS = \
#	-ltar \

train_bow_SRCS = \
	utils.cpp \
	train_bow.cpp \

train_svm_SRCS = \
	utils.cpp \
	train_svm.cpp \

predict_SRCS = \
	predict.cpp \

define PROGRAM_template
$(1): $$($(1)_SRCS:.cpp=.o)
	$${LINK.cpp} $$($(1)_LDFLAGS) $$^ -o $$@
endef

define TEST_template
$(1:.cpp=): $(1) $$(patsubst tests/test_%,%,$(1))
	$${LINK.cpp} $$^ -o $$@
endef

all: Makefile check ${TARGETS}

$(foreach target, ${TARGETS}, $(eval $(call PROGRAM_template,$(target))))
$(foreach test, $(TEST_SOURCES), $(eval $(call TEST_template,$(test))))

check: LDFLAGS += -lboost_unit_test_framework -ltar
check: $(TEST_SOURCES:.cpp=)
	@for test in $(TEST_SOURCES:.cpp=); do ./$$test; done

clean:
	${RM} *.o ${TARGETS} ${TEST_SOURCES:.cpp=} ${TEST_SOURCES:.cpp=.o}
