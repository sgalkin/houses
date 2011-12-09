TARGETS = collect_sift train_bow train_svm predict

TEST_SOURCES = \
	tests/test_utils.cpp \
	tests/test_tar.cpp \

OPENCV_ROOT=/home/stranger/stuff/opencv/usr/local

CXX = gcc-4.6
CXXFLAGS = -Wall -O2 -g -I${OPENCV_ROOT}/include -I/opt/local/include -pipe -std=c++0x
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

collect_sift_SRCS = \
	utils.cpp \
	collect_sift.cpp \

collect_sift_LDFLAGS = \
	-ltar \

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

$(foreach target, ${TARGETS}, $(eval $(call PROGRAM_template,$(target))))
$(foreach test, $(TEST_SOURCES), $(eval $(call TEST_template,$(test))))

all: Makefile check ${TARGETS}

check: LDFLAGS += -lboost_unit_test_framework -ltar #-I/opt/local/include
check: $(TEST_SOURCES:.cpp=)
	@for test in $(TEST_SOURCES:.cpp=); do ./$$test; done

clean:
	${RM} *.o ${TARGETS} ${TEST_SOURCES:.cpp=}
