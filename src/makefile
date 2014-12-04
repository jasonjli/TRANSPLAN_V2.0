include makedefs

.cc.o:
	$(CCC) $(CFLAGS) -I. $(TRANS_PLAN_INCL) -c $*.cc

%.d : %.cc
	$(CCC) -MM $(CFLAGS) -I. $(TRANS_PLAN_INCL) $< > $@

#CSPPLANEXESRC = driver.cc 
CSPPLANEXESRC = factory_reader.cc 
#CSPPLANEXESRC = jss_reader.cc 

CSPPLANVER = factory_solver
#CSPPLANVER = fjs_solver.exe

CSPPLANSRC = utils.cc \
	     transplan.cc  \
             cspintvar.cc \
	     cspintervalvar.cc \
             cspboolvar.cc \
             cspauxsetvar.cc \
             cspauxpairsetvar.cc \
             cspauxsetmonotonicincvar.cc \
             cspauxsetmonotonicdecvar.cc \
             cspauxintvar.cc \
             cspmaxcountervar.cc \
             cspgraphvar.cc \
             search.cc \
             searchstate.cc \
             variablepropagatormanager.cc  \
            $(CSPPLANEXESRC)

all: $(CSPPLANSRC:.cc=.o) solver
	cp $(CSPPLANVER) ../


solver: $(CSPPLANSRC:.cc=.o)
		$(LINKER) $(LFLAGS) $(CSPPLANSRC:.cc=.o) -o $(CSPPLANVER)

clean:
	rm -f *.o *.d $(CSPPLANEXESRC:.cc=$(EXEEXT))


-include $(CSPPLANSRC:.cc=.d) $(CSPPLANEXESRC:.cc=.d)
# DO NOT DELETE
