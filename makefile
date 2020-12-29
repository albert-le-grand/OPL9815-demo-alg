######################################################################################################################################
######################################################################################################################################
######################################################################################################################################
#
#	Application Makefile
# 
######################################################################################################################################
######################################################################################################################################
######################################################################################################################################
TERMINAL_TYPE=OPL9815
# List the application version here. Note that this may not be larger than 15 characters or you will get a linker error! 
# The OS can read this version and show it in the system menu.
VERSION=ALG0901

# The final output file we want to create. The extention is always .hex and this should not be added to the name here.
TARGET = $(VERSION)

# C source files. List all the c-files here and add ".c" to the filename
CSRC = main.c menu.c gps.c fancy.c utils.c

# Assembler source files. List all assembly files here and add ".asm" to the filename
ASRC =

# List all the additional libraries here, sepatarted by spaces. These libraries 
# have to be in the root of your project and must start with "lib" and end with ".a" (e.g. libusb.a) 
# The entry here must be without the "lib" and without ".a" (e.g. usb in case of libusb.a)
# The standard libray libc.a is always included. libm.a (Math library) is optional, so you might add m here.
LIBS = STARTUP$(TERMINAL_TYPE)

# Optimization level, can be [0, 1, 2, 3, s]. 
OPT = s

# Debugging format. 0: No debugging information; Nothing: Produce debugging information in the operating system's native format
DEBUG = 0

# The paths of your project. 
# The directlry for the object files and other temp. files
OUT = obj

# The directory for your c source files
SRC = Sources

# The directory for you assmbly source files
ASM = Sources

# List all the include directories here, separated by spaces
EXTRAINCDIRS = $(SRC)/include 


######################################################################################################################################
######################################################################################################################################
######################################################################################################################################
#
#		................................Do not change anything below these lines........................
#
######################################################################################################################################
######################################################################################################################################
######################################################################################################################################
ifndef VERSION
VERSION="Opticon 1.0"
endif

DOUBLE_FORMAT=

LINKER_SCRIPT = linker
CSTANDARD = gnu99
CFLAGS = -g$(DEBUG) $(CDEFS) $(CINCS) -O$(OPT) -Wall $(DOUBLE_FORMAT) -Wno-main -funsigned-char -funsigned-bitfields -fshort-enums -Wa,-adhlns=$(OUT)/$(notdir $(@:%.o=%.lst)) $(patsubst %,-I%,$(EXTRAINCDIRS)) -std=$(CSTANDARD) -MMD
ASFLAGS = $(ADEFS) -Wall $(patsubst %,-I%,$(EXTRAINCDIRS)) $(DOUBLE_FORMAT) -Wa,-adhlns=$(<:$(ASM)/%.asm=obj/%.lst),-g$(DEBUG)
LDFLAGS = -nostartfiles --entry _Startup -Wl,-Map=$(TARGET).map,--cref $(DOUBLE_FORMAT) -T$(LINKER_SCRIPT).ld -L. $(patsubst %,-l%,$(LIBS))

PREF = rx-elf-
#SHELL = command
CC = $(PREF)gcc
OBJCOPY = $(PREF)objcopy
OBJDUMP = $(PREF)objdump
SIZE = $(PREF)size
NM = $(PREF)nm
REMOVE = rm
COPY = cp

# Define Messages
# English
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_FLASH = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:

OBJ = $(ASRC:%.asm=$(OUT)/%.o) $(CSRC:%.c=$(OUT)/%.o) $(OUT)/app_version.o

A_OBJ = $(ASRC:%.asm=$(OUT)/%.o)
C_OBJ = $(CSRC:%.c=$(OUT)/%.o)
LST = $(OBJ:%.o=%.lst) 
DEPS = $(OBJ:%.o=%.d)

all: begin elf hex end

elf: $(TARGET).elf
hex: $(TARGET).hex
lss: $(TARGET).lss 
sym: $(TARGET).sym

begin:
	@echo
	@echo $(MSG_BEGIN)
	@if [ ! -d $(OUT) ]; then mkdir $(OUT); fi

end:
	@echo
	@echo $(MSG_END)
	@echo

%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	@$(OBJCOPY) -O ihex $< $@
	rx_checksum -m $(TERMINAL_TYPE) $(TARGET).hex

%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	@$(OBJDUMP) -h -S $< > $@

%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	@$(NM) -n $< > $@

.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ) Makefile linker.ld 
	@echo
	@echo $(MSG_LINKING) $@
	@$(CC) $(CFLAGS) $(OBJ)  --output $@ $(LDFLAGS)	
	
$(C_OBJ) : $(OUT)/%.o : $(SRC)/%.c 
	@echo
	@echo $(MSG_COMPILING) $<
	@$(CC) -c $(CFLAGS) $< -o $@

$(A_OBJ): $(OUT)/%.o : $(ASM)/%.asm
	@echo
	@echo $(MSG_ASSEMBLING) $<
	@$(CC) -c -x assembler-with-cpp $(ASFLAGS) $< -o $@


$(OUT)/app_version.o: Makefile
	@echo "//" > $(SRC)/app_version.c
	@echo "// Created by Makefile utility, please do not edit here. Edit the Makefile instead" >> $(SRC)/app_version.c
	@echo "//" >> $(SRC)/app_version.c
	@echo "#define VERSION_SECT  __attribute__ (( section (\".version\")))" >> $(SRC)/app_version.c
	@echo "VERSION_SECT const char app_version[]=\""$(VERSION)\"\; >> $(SRC)/app_version.c
	@$(CC) -c $(CFLAGS) $(SRC)/app_version.c -o $@

clean :
	@echo
	@echo $(MSG_CLEANING)
	
	$(REMOVE) -Rf $(OUT)
	$(REMOVE) -Rf *.elf
	$(REMOVE) -Rf *.map
	$(REMOVE) -Rf $(TARGET).obj
	$(REMOVE) -Rf $(TARGET).sym
	$(REMOVE) -Rf $(TARGET).lnk
	$(REMOVE) -Rf $(TARGET).lss

-include $(DEPS)

