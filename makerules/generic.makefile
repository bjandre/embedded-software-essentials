#
# Generic rule to generate various targets
#
$(shell mkdir -p $(DEPENDS_DIR))

%.o : %.c
%.o : %.c $(DEPENDS_DIR)/%.d
	$(CC) $(DEPENDS_FLAGS) $(CFLAGS) $(INCLUDES) -c -o $@ $<
	$(POSTCOMPILE)

%.i : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) -c -o $*.i $<

%.asm : %.c
	$(CC) $(C_ASM_FLAGS) $(CFLAGS) $(INCLUDES) -c -o $*.asm $<

%.o : %.S
	$(AS) $(ASMFLAGS) $(INCLUDES) -c -o $@ $<

$(LIB) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(STARTUP_LIB) : $(STARTUP_OBJS) $(ASM_OBJS)
	$(AR) $(ARFLAGS) $@ $(ASM_OBJS) $(STARTUP_OBJS)

# debugging flag to turn on verbose output and see gcc wrapper link info
#DEBUG_CC_LINK := -v
DEBUG_CC_LINK :=

$(EXE) : $(DEPEND_STARTUP) $(OBJS) $(DEPEND_LIBS)
	$(CC) $(CFLAGS) $(CC_LDFLAGS) $(DEBUG_CC_LINK) -o $@ $^ $(DEPEND_STARTUP)
#	$(LD) $(LDFLAGS) -o $@ $(LDLIBS_PRE) $^ $(LDLIBS_POST)
	$(SIZE) $(SIZEFLAGS) $@
#	file $@
#	ldd $@

#
# macro for executing TARGET in all SUBDIRS
#
ifdef SUBDIRS
.PHONY : $(SUBDIRS)
$(SUBDIRS) : 
	@if [ -d $@ ]; then \
		$(MAKE) --no-print-directory --directory=$@ $(MAKECMDGOALS); \
	fi
	$(BUILD_COMPLETE)
endif	

$(DEPENDS_DIR)/%.d : ;
.PRECIOUS : $(DEPENDS_DIR)/%.d

-include $(SRCS:%.c=$(DEPENDS_DIR)/%.d)

.PHONY : all
all : $(SUBDIRS) $(LIB) $(STARTUP_LIB) $(EXE) $(BIN)

.PHONY : bin
bin : $(SUBDIRS) $(EXE)

.PHONY : dump
dump : $(SUBDIRS) $(EXE)
ifdef EXE
	$(OBJDUMP) $(OBJDUMP_FLAGS) $(EXE) > $(EXE:%.$(EXE_EXTENSION)=%.$(DUMP_EXTENSION))
endif


.PHONY : test
test : $(SUBDIRS) all $(TEST_EXE)
ifdef TEST_EXE
	if [ -e ./$(TEST_EXE) ]; then \
    ./$(TEST_EXE); \
fi
endif

.PHONY : compile-all
compile-all : $(SUBDIRS) $(OBJS)

.PHONY : build-lib
build-lib : $(SUBDIRS) $(LIB)

.PHONY : build
build : all

.PHONY : clean
clean : $(SUBDIRS)
	@-$(RM) -rf $(EDITOR_FILES) $(BUILD_ARTIFACTS)

.PHONY : clobber
clobber : $(THIRD_PARTY_DIR)

# transfer cross compiled executable to the bbb. Prepend executable
# name with $(CX_PREFIX) to indicate a cross compiled executable.
.PHONEY : tx-bbb
tx-bbb : $(SUBDIRS) all
ifdef EXE
	if [ -e ./$(EXE) ]; then $(SCP) $(EXE) $(SSH_HOST):$(CX_PREFIX)$(EXE); fi
endif

FORCE :


.PHONY : echo
echo :
	echo DEPENDS_DIR = $(DEPENDS_DIR)
