CUDA_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(CUDA_DIR)/../Make.inc


DUMMY=
SPACE=$(DUMMY) $(DUMMY)
COMMA=$(DUMMY),$(DUMMY)

define join-list
$(subst $(SPACE),$(2),$(1))
endef


CUDA_ROOT = /opt/cuda

MACHINE := $(shell uname -m)

ifeq ($(MACHINE), x86_64)
LDFLAGS += -L$(CUDA_ROOT)/lib64
endif
ifeq ($(MACHINE), i686)
LDFLAGS += -L$(CUDA_ROOT)/lib
endif

LDLIBS   += -lcudart

CPPFLAGS += -I$(CUDA_ROOT)/include

NVCC=$(CUDA_ROOT)/bin/nvcc

ifneq ($(strip $(LDLIBS)),)
NVCC_LDLIBS = -Xcompiler $(call join-list,$(LDLIBS),$(COMMA))
endif

%: %.cu
	$(NVCC) $(CPPFLAGS) $(NVCCFLAGS) $(NVCC_LDLIBS) -o $@ $^

%.o: %.cu
	$(NVCC) $(CPPFLAGS) $(NVCCFLAGS) -c -o $@ $<