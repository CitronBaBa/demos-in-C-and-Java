 --- There is pre-compiled windows.exe file in the third zip, it can run without any sdl libraries installed

The program is a combination of parser interpreter and graphical interface:
you type in a "turtle" language and turtle will move around the screen to avoid collision with rabbits
standard version : focus on parser and testing
extended version : focus on making it a game


Basic Commands:
FD 40 ;  move forward by 40 
LT  20 ;  change angle by 20 degree left
RT  20 ;
etc...

example:
{  DO A FROM 1 TO 50 
{  FD A
   RT 30
   DO B FROM 1 TO 8
   {  SET C := A 5 / ;
      FD C 
      RT 45 }
    }
}

---Standard version contains 4 compile targets, std_win is to be used on the windows platform, 
      std_test use both white/black box testing to test the program. Error cases are also tested, which will be printed as     
    "Expected error". 
   
 --- Extended version.  I have not been able to embed libraries inside the program other than on windows, so     
    there should be

     !!!!<SDL_ttf> <SDL_image> <SDL_mixer> !!!! 

     in the computer for both compiling the program and running any executable files on linux and macs. 
     Please tell me if it doesn't compile or run.
     Because of SDL, the program is more stable on windows than on mac, normally it will work on both platforms. 
     on windows it is compiled using mingw.
     There is also a sound effect, make sure to switch the audio on.
      
  make method in standard version:
        make std
        make std_win
        make std_parser
        make std_test

  make method in extended version: 
        make turtlebay
        make turtlebay_win


Testing Strategy :
     black box testing tests whether the ending point of the turtle meets the expected point.

     For white box testing, test.h replace the PUTERROR macro, which allows the interpreter to not exit the entire program
     when meeting an error, which makes it possible to test error cases.
     However, to make this viable. Changes were made to the interpreter.
     1   after every possible errors generator, check whether the error flag is on and return from these functions.
     2   there is a switch to switch off the graphic, hence the interpretation when doing these tests.
         In black box testing it is switched off by default, you can switch it on by changing the macro.
         In white box testing it is always switched off to avoid confusion.
     Both factors only affect the std_test, in std they will be there but take no effect.


Extension contents:
     
    Struct article is a home made basic text editor to enter the program directly in the window, press 'ENTER' key will allow      
     the program to be executed. 
     The turtle will follow the program entered to move on the window.
     There are two extra instructions added :
               "ROTATE" + VARNUM ex: ROTATE 3  will allow the turtle to rotate constantly by the varnum value.
               "HARD" + VARNUM  ex:  HARD 2.0  will change the hardness of the game, the speed by which rabbits move.
               you can stop all the rabbits by "{ HARD 0 }".
     Once it collides with others (rabbits), the game will restart.
