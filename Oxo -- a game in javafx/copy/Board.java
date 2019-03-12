import java.util.*;
import javafx.application.Application;
class Board
{   private Oxo[][] grid = new Oxo[3][3];

    public void run()
    {   String instruct;
        int i;
        boolean going = true, move_succe = false;
        for(i=0;going;i++)
        {   Display.print_player(i);
            instruct = Display.scan();
            if(instruct.equals("xx"))
            {   Display.print_movefail();
                i--;
            }
            else
            {   if(i%2==0) move_succe = move(instruct,Oxo.o);
                else move_succe = move(instruct,Oxo.x);
                if(!move_succe)
                {   Display.print_movefail();
                    i--;
                }
            }

            Display.print_grid(grid,3,3);

            if(win_check())
            {   Display.print_win(i);
                going = false;
            }
        }
    }

    private boolean move(String location, Oxo player)
    {   int y = location.charAt(0)-'a';
        int x = location.charAt(1)-'1';
        if(grid[y][x]== null)
        {   grid[y][x]=player;
            return true;
        }
        else
        {   return false;
        }
    }

    private boolean win_check()
    {   int i;
        // straight line win
        for(i=0;i<3;i++)
        {   if(grid[i][0]==grid[i][1] && grid[i][1]==grid[i][2] && grid[i][2]!=null)
            {   return true;
            }
            if(grid[0][i]==grid[1][i] && grid[1][i]==grid[2][i] && grid[2][i]!=null)
            {   return true;
            }
        }
        // diagnoal win
        if(grid[0][0]==grid[1][1] && grid[1][1]==grid[2][2] && grid[2][2]!=null)
        {   return true;
        }
        if(grid[2][0]==grid[1][1] && grid[1][1]==grid[0][2] && grid[0][2]!=null)
        {   return true;
        }

        return false;
    }
}
