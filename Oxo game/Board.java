import java.util.*;

class Board
{   private Oxo[][] grid = new Oxo[3][3];
    public int[][] sequences = new int[9][2];
    public int step;
    public Oxo current = Oxo.o;

    public static void main(String[] args)
    {   Board board = new Board();
        board.test();
    }

    public boolean move(int x, int y)
    {   if(grid[y][x]==null)
        {   grid[y][x] = current;
            sequences[step][0] = y;
            sequences[step][1] = x;
            step++;
            if(current==Oxo.o) current = Oxo.x;
            else current = Oxo.o;
            return true;
        }
        else
        {   return false;
        }
    }

    public void move_back()
    {   assert(step>0);
        int y = sequences[step-1][0];
        int x = sequences[step-1][1];
        grid[y][x] = null;
        if(current==Oxo.o) current = Oxo.x;
        else current = Oxo.o;
        step--;
    }

    public void rewind()
    {   int i,j;
        for(i=0;i<3;i++)
        {   for(j=0;j<3;j++)
            {   grid[i][j] = null;
            }
        }
        step = 0;
        current = Oxo.o;
    }

    public boolean win_check()
    {   String a = win_position();
        if(a.equals(""))
        {   return false;
        }
        return true;
    }

    public String win_position()
    {   int i;
        String result="";
        // straight line win
        for(i=0;i<3;i++)
        {   if(grid[i][0]==grid[i][1] && grid[i][1]==grid[i][2] && grid[i][2]!=null)
            {   return "H"+i;
            }
            if(grid[0][i]==grid[1][i] && grid[1][i]==grid[2][i] && grid[2][i]!=null)
            {   return "V"+i;
            }
        }
        // diagnoal win
        if(grid[0][0]==grid[1][1] && grid[1][1]==grid[2][2] && grid[2][2]!=null)
        {   return "D0";
        }
        if(grid[2][0]==grid[1][1] && grid[1][1]==grid[0][2] && grid[0][2]!=null)
        {   return "D1";
        }

        return "";
    }

    public Oxo winner()
    {   int y = sequences[step-1][0];
        int x = sequences[step-1][1];
        return grid[y][x];
    }

    private void test()
    {   test_move();
        test_move_back();
        test_rewind();
        test_win();
        System.out.println("All tests passed");
    }

    private void test_move()
    {   assert(move(0,0));
        assert(step==1 && current==Oxo.x);
        assert(sequences[0][0]==0 && sequences[0][1]==0);
        assert(!move(0,0));
        assert(move(1,0));
        assert(step==2 && current==Oxo.o);
        assert(sequences[1][0]==0 && sequences[1][1]==1);
        assert(move(2,2));
        assert(step==3 && current==Oxo.x);
        assert(sequences[2][0]==2 && sequences[2][1]==2);
        rewind();
    }

    private void test_move_back()
    {   move(0,0); move_back();
        assert(step==0);
        assert(current==Oxo.o);
        move(1,0); move(2,0); move_back();
        assert(step==1);
        assert(current==Oxo.x);
        rewind();
    }

    private void test_rewind()
    {   move(0,0); move(0,1); move(0,2);
        rewind();
        assert(step==0);
        assert(current==Oxo.o);
        assert(grid[0][0]==null);
        assert(grid[0][1]==null);
        assert(grid[0][2]==null);
    }

    private void test_win()
    {   grid[0][0]=grid[0][1]=grid[0][2]=Oxo.x;
        assert(win_check());
        assert(win_position().equals("H0"));
        rewind();
        grid[0][0]=grid[1][0]=grid[2][0]=Oxo.x;
        assert(win_check());
        assert(win_position().equals("V0"));
        rewind();
        grid[1][0]=grid[1][1]=grid[1][2]=Oxo.o;
        assert(win_check());
        assert(win_position().equals("H1"));
        rewind();
        grid[0][0]=grid[1][1]=grid[2][2]=Oxo.x;
        assert(win_check());
        assert(win_position().equals("D0"));
        rewind();
        grid[0][2]=grid[1][1]=grid[2][0]=Oxo.o;
        assert(win_check());
        assert(win_position().equals("D1"));
        rewind();
    }

}
