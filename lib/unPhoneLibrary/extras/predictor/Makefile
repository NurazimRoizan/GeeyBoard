# Makefile -- Mark's predictive text stuff adapted to generate C++ model

c-loop:
	bash -c "while :; do clear; make predict |grep -v 'ing directory'; ./predict; rm predict; read; done"
py-loop:
	bash -c "while :; do clear; python3 build-word-tables.py |fold -s -w 200; echo; read; done"
predict: predictor.h predictor.cpp
	@echo
	g++ -O3 -I. -std=c++11 -DPREDICTOR_MAIN=1 -o predict predictor.cpp
predictor.h: build-word-tables.py
	python3 build-word-tables.py >$@
