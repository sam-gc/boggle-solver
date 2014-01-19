solver: boggle.c build.h definitions.h dictionary.c dictionary.h hashset.c hashset.h solver.c solver.h progress.c progress.h
	incrbuild build.h
	gcc -o solver boggle.c dictionary.c hashset.c solver.c progress.c -pthread -g -O0

solver-release: boggle.c build.h definitions.h dictionary.c dictionary.h hashset.c hashset.h solver.c solver.h progress.c progress.h
	incrbuild build.h
	gcc -o solver boggle.c dictionary.c hashset.c solver.c progress.c -pthread -O3