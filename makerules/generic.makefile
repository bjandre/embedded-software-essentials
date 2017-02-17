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

$(LIB) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(EXE) : $(OBJS) $(DEPEND_LIBS)
#	$(CC) $(CFLAGS) -o $@ $^ -v
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)
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
all : $(SUBDIRS) $(LIB) $(EXE)

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
