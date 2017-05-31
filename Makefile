all: solver

solver:
	$(MAKE) -C src all
	cp src/solver main
clean: 
	$(MAKE) -C src clean