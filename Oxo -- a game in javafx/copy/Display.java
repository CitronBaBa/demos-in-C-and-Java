import java.util.*;
import javafx.application.*;
import javafx.stage.*;
import javafx.scene.*;
import javafx.scene.control.*;

class Display extends Application
{
    public void start(Stage stage)
    {   Label hello = new Label("Hello JavaFX world!");
        Group root = new Group(hello);
        Scene scene = new Scene(root);
        stage.setTitle("Hello World!");
        stage.setScene(scene);
        stage.show();
    }

    public static void print_grid(Oxo[][] grid, int y, int x)
    {   int i,j;
        String linetag = "abc";
        System.out.println("   123");
        for(i=0;i<y;i++)
        {   System.out.print(linetag.charAt(i)+"  ");
            for(j=0;j<x;j++)
            {   if(grid[i][j]==Oxo.x) System.out.print("X");
                else if (grid[i][j]==Oxo.o) System.out.print("O");
                else System.out.print(".");
            }
            System.out.println();
        }
    }

    public static void print_player(int i)
    {   if (i%2==0) System.out.println("player 1 has his turn");
        else System.out.println("player 2 has his turn");
    }

    public static void print_movefail()
    {   System.out.println("You have a unsuccessful/incorrect move, try it again");
    }

    public static void print_win(int i)
    {   if (i%2==0) System.out.println("player 1 has won");
        else System.out.println("player 2 has won");
    }

    public static String scan()
    {   Scanner input = new Scanner(System.in);
        String move;
        char y,x;
        System.out.print("Enter next move :");
        move = input.nextLine();
        if (move.length()!=2)
        {   return "xx";
        }
        y = move.charAt(0);
        x = move.charAt(1);
        if(y>'c' || y<'a')
        {   return "xx";
        }
        if(x>'3' || y<'1')
        {   return "xx";
        }
        return move;
     }
}
