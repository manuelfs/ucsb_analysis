EXEDIR := run
OBJDIR := bin
SRCDIR := src
INCDIR := inc
MAKEDIR := bin

CXX := $(shell root-config --cxx)
CXXFLAGS := -isystem $(shell root-config --incdir) -Wall -Wextra -pedantic -Wshadow $(shell root-config --cflags) -O2 -I $(INCDIR)
LD := $(shell root-config --ld)
LDFLAGS := $(shell root-config --ldflags)
LDLIBS := $(shell root-config --libs) -lMinuit

vpath %.cpp $(SRCDIR)
vpath %.hpp $(INCDIR)
vpath %.o $(OBJDIR)
vpath %.so $(OBJDIR)
vpath %.exe $(EXEDIR)
vpath %.d $(MAKEDIR)

# Add new executables to this list
all: calc_trig_efficiency.exe

# List any object files your executable need to be linked with
$(EXEDIR)/calc_trig_efficiency.exe: calc_trig_efficiency.o ra4_handler.o cfa.o in_json_2012.o ucsb_utils.o timer.o styles.o

-include $(addsuffix .d,$(addprefix $(MAKEDIR)/,$(notdir $(basename $(wildcard $(SRCDIR)/*.cpp)))))

$(MAKEDIR)/%.d: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM -MG -MF $@ $< 
	sed -i'' 's#$*.o#$(OBJDIR)/$*.o $(MAKEDIR)/$*.d#g' $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# This is a bit ugly. Shouldn't need the dependency explicitly.
$(EXEDIR)/%.exe: $(OBJDIR)/%.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)


.DELETE_ON_ERROR:

.PHONY: clean

clean:
	-rm -rf $(EXEDIR)/*.exe $(OBJDIR)/*.o $(MAKEDIR)/*.d *.exe *.o *.d
	./run/remove_backups.sh
