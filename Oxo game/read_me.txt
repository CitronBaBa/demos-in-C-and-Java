make method:
	make run	
	make Ai
	make Board
	make Controller

Graphical Interface:
              //Let Me help you button// :  compute and excute an optimal move for the current player
              //AI checkbox// :  let computer play the opposition player
              Computer will predict the winner on the buttom of the window.

design:    Controller.java -> Ai.java -> Board.java -> Ox.java
	     The Ai and Board classes are well separated from Controller,
 	but the Controller is mixed with graph and control abilities.
	     Because I used the built in button class of Jfx without dealing with
 	input event queue directly, it is hard for me to separate the graphical function
	from the Controller, as I only pass a response function into the button.
 	     But I still managed to test it with a switch, only some features were tested in 
	the controller. Ai and board classes were thoroughly tested.

default JFX directory: ../ FX / 
