ExtLib_H  = $(C_INCLUDE_PATH)/ExtLib.h

ExtLib_C  = ExtLib.c
Xm_C      = libxm/src/context.c \
			libxm/src/load.c \
			libxm/src/play.c \
			libxm/src/xm.c
Zip_C     = zip/src/zip.c
Audio_C   = miniaudio.c
Mp3_C     = mp3.c

ExtLib_Linux_O  = $(foreach f,$(ExtLib_C:.c=.o), bin/linux/$f)
Zip_Linux_O     = $(foreach f,$(Zip_C:.c=.o), bin/linux/$f)
Audio_Linux_O   = $(foreach f,$(Audio_C:.c=.o), bin/linux/$f)
Mp3_Linux_O     = $(foreach f,$(Mp3_C:.c=.o), bin/linux/$f)
Xm_Linux_O      = $(foreach f,$(Xm_C:.c=.o), bin/linux/$f)

ExtLib_Win32_O  = $(foreach f,$(ExtLib_C:.c=.o), bin/win32/$f)
Zip_Win32_O     = $(foreach f,$(Zip_C:.c=.o), bin/win32/$f)
Audio_Win32_O   = $(foreach f,$(Audio_C:.c=.o), bin/win32/$f)
Mp3_Win32_O     = $(foreach f,$(Mp3_C:.c=.o), bin/win32/$f)
Xm_Win32_O      = $(foreach f,$(Xm_C:.c=.o), bin/win32/$f)

# Make build directories
$(shell mkdir -p bin/ $(foreach dir, \
	$(dir $(ExtLib_Linux_O)) \
	$(dir $(Zip_Linux_O)) \
	$(dir $(Audio_Linux_O)) \
	$(dir $(Mp3_Linux_O)) \
	$(dir $(Xm_Linux_O)) \
	\
	$(dir $(ExtLib_Win32_O)) \
	$(dir $(Zip_Win32_O)) \
	$(dir $(Audio_Win32_O)) \
	$(dir $(Mp3_Win32_O)) \
	$(dir $(Xm_Win32_O)) \
	, $(dir)))
	
extlib_linux: $(ExtLib_Linux_O)
extlib_win32: $(ExtLib_Win32_O)
	
bin/win32/ExtLib.o: $(C_INCLUDE_PATH)/ExtLib.c $(C_INCLUDE_PATH)/ExtLib.h
bin/win32/%.o: $(C_INCLUDE_PATH)/%.c
	@echo "$(PRNT_RSET)[$(PRNT_BLUE)$(notdir $@)$(PRNT_RSET)]"
	@i686-w64-mingw32.static-gcc -c -o $@ $< $(OPT_WIN32) $(CFLAGS) -Wno-format-truncation -Wno-strict-aliasing -Wno-implicit-function-declaration -DNDEBUG -D_WIN32

bin/linux/ExtLib.o: $(C_INCLUDE_PATH)/ExtLib.c $(C_INCLUDE_PATH)/ExtLib.h
bin/linux/%.o: $(C_INCLUDE_PATH)/%.c
	@echo "$(PRNT_RSET)[$(PRNT_BLUE)$(notdir $@)$(PRNT_RSET)]"
	@gcc -c -o $@ $< $(OPT_LINUX) $(CFLAGS) -Wno-unused-result -Wno-format-truncation -Wno-strict-aliasing -Wno-implicit-function-declaration -DNDEBUG