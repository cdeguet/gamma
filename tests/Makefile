CASES = $(wildcard cases/*.gm)
CASES_BASE = $(CASES:cases/%.gm=%)

diff:
	@cd cases; $(foreach case,$(CASES_BASE),../../gammac/out/bin/gammac -d $(case).gm | diff ../baseline/$(case).txt -;)

rebuild: $(CASES)
	@cd cases; $(foreach case,$(CASES_BASE),../../gammac/out/bin/gammac -d $(case).gm >| ../baseline/$(case).txt;)