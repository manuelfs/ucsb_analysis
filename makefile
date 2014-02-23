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
all: calc_trig_efficiency.exe make_tree.exe generate_small_tree.exe

# List any object files your executable need to be linked with
$(EXEDIR)/calc_trig_efficiency.exe: calc_trig_efficiency.o ra4_handler.o cfa.o in_json_2012.o ucsb_utils.o timer.o styles.o small_tree.o
$(EXEDIR)/make_tree.exe: make_tree.o ra4_handler.o cfa.o in_json_2012.o ucsb_utils.o timer.o styles.o small_tree.o
$(EXEDIR)/generate_small_tree.exe: generate_small_tree.o

-include $(addsuffix .d,$(addprefix $(MAKEDIR)/,$(notdir $(basename $(wildcard $(SRCDIR)/*.cpp)))))
-include $(MAKEDIR)/small_tree.d

$(MAKEDIR)/%.d: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM -MG -MF $@ $< 
	sed -i'' 's#$*.o#$(OBJDIR)/$*.o $(MAKEDIR)/$*.d#g' $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# This is a bit ugly. Shouldn't need the dependency explicitly.
$(EXEDIR)/%.exe: $(OBJDIR)/%.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)


# small_tree.cpp and small_tree.hpp need special treatment. Probably cleaner ways to do this.
$(SRCDIR)/small_tree.cpp $(INCDIR)/small_tree.hpp: dummy_small_tree.all
.SECONDARY: dummy_small_tree.all
dummy_small_tree.all: $(EXEDIR)/generate_small_tree.exe 
	./$< $(word 2,$^)
.PRECIOUS: generate_small_tree.o

.DELETE_ON_ERROR:

.PHONY: clean

clean:
	-rm -rf $(EXEDIR)/*.exe $(OBJDIR)/*.o $(MAKEDIR)/*.d $(SRCDIR)/small_tree.cpp $(INCDIR)/small_tree.hpp *.exe *.o *.d
	./run/remove_backups.sh
