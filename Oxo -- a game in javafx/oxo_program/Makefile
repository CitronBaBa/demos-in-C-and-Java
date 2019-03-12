default: run
jfx = --module-path FX/lib --add-modules=javafx.controls

Board: Board.java
	javac $@.java Ox.java $(jfx)
	java -ea $(jfx) Board

Controller: Controller.java
	javac $@.java  Board.java Ai.java Ox.java $(jfx)
	java -ea $(jfx) Controller test

Ai:
	javac $@.java Board.java Ox.java $(jfx)
	java -ea Ai

run:
	javac Controller.java Board.java Ai.java Ox.java $(jfx)
	java -ea $(jfx) Controller test
	java $(jfx) Controller
