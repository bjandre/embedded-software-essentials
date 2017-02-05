#
# Generic rule to generate various targets
#
$(shell mkdir -p $(DEPENDS_DIR))

%.o : %.c
%.o : %.c $(DEPENDS_DIR)/%.d
	$(CC) $(DEPENDS_FLAGS) $(CFLAGS) $(INCLUDES) -c -o $@ $<
	$(POSTCOMPILE)

%.i : %.c
	$(CPP) $(CPPFLAGS) -c -o $*.i $<

$(LIB) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(EXE) : $(OBJS) $(DEPEND_LIBS)
#	$(CC) $(CFLAGS) -o $@ $^ -v
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)
#	file $@
#	ldd $@

$(DEPENDS_DIR)/%.d : ;
.PRECIOUS : $(DEPENDS_DIR)/%.d

-include $(patsubst %, $(DEPENDS_DIR)/%.d, $(basename, $(SRCS)))

.PHONY : compile-all
compile-all : $(SUBDIRS) $(OBJS)

.PHONY : clean
clean :
	@-$(RM) -rf $(EDITOR_FILES) $(BUILD_ARTIFACTS)

