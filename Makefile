TARGETS = collect_sift train_bow

TEST_SOURCES = \
	tests/test_utils.cpp \

CXXFLAGS = -Wall -O3 -g -I/Users/stranger/Works/local/include -I/opt/local/include -pipe
LDFLAGS = -L/Users/stranger/Works/local/lib \
	-L/opt/local/lib \
	-lopencv_features2d \
	-lopencv_core \
	-lopencv_imgproc \
	-lopencv_flann \
	-lopencv_highgui \
	-lopencv_ml \
	-lboost_filesystem \
	-lboost_system \

LD_LIBRARY_PATH=/Users/stranger/Works/local/lib
export LD_LIBRARY_PATH

all: ${TARGETS}

collect_sift_SRCS = \
	utils.cpp \
	collect_sift.cpp \

train_bow_SRCS = \
	utils.cpp \
	train_bow.cpp \

# define PROGRAM_template = 
# $(1): $$($(1)_SRCS:.cpp=.o}
# 	$${LINK.cpp} $$^ -o $$@
# endef

# $(foreach target, ${TARGETS}, $(eval $(call $(PROGRAM_template, $(target)))))

collect_sift: ${collect_sift_SRCS:.cpp=.o}
	${LINK.cpp} $^ -o $@

train_bow: ${train_bow_SRCS:.cpp=.o}
	${LINK.cpp} $^ -o $@

check: LDFLAGS += -lboost_unit_test_framework -I/opt/local/include
check: ${TEST_SOURCES:.cpp=}
	$(foreach target, ${TEST_SOURCES:.cpp=}, ./${target})

${TEST_SOURCES:.cpp=}: ${TEST_SOURCES:.cpp=.o} ${patsubst test_%,%,${TEST_SOURCES}}

clean:
	${RM} *.o ${TARGETS} ${TEST_SOURCES:.cpp=}
