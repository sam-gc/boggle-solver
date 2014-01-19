solver: boggle.c build.h definitions.h dictionary.c dictionary.h hashset.c hashset.h solver.c solver.h
	incrbuild build.h
	gcc -o solver boggle.c dictionary.c hashset.c solver.c

clean:
	rm solver
