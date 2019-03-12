import java.util.*;
class Ai
{   public int answer_x;
    public int answer_y;

    private int current_x;
    private int current_y;
    private int initial_step;
    private Board board;

    Ai(Board gameboard)
    {   board = gameboard;
    }

    public static void main(String[] args)
    {   Board gameboard = new Board();
        Ai ai = new Ai(gameboard);
        ai.test();
    }

    public Oxo run()
    {   Oxo winner;
        initial_step = board.step;
        winner=explore();
        if(winner!=board.current && winner!=null)
        {   compromise();
        }
        return winner;
    }

// a very delicate recursion way to predict(determin) the winner/draw
// on the assumption that both players' moves are optimised
// and give a best next move for the current player if there is one

    public Oxo explore()
    {   int i,j;
        Oxo winner, player = board.current;
        boolean draw = false;

        if (converge()) return board.current;
        if (board.step==9) return null;

        for(i=0;i<3;i++)
        {   for(j=0;j<3;j++)
            {   if(board.move(i,j))
                {   winner=explore();
                    if(winner==player)
                    {   record_solution(board.step-1,initial_step,i,j);
                        board.move_back();
                        return player;
                    }
                    if(winner==null)
                    {   record_localdraw(board.step-1,initial_step,i,j);
                        draw = true;
                    }
                    board.move_back();
                }
            }
        }
        if(draw)
        {   record_solution(board.step, initial_step, current_x,current_y);
            return null;
        }
        else if (player==Oxo.o) return Oxo.x;
        else return Oxo.o;
    }

    public boolean converge()
    {   int i,j;
        for(i=0;i<3;i++)
        {   for(j=0;j<3;j++)
            {   if(board.move(i,j))
                {   if(board.win_check())
                    {   record_solution(board.step-1,initial_step,i,j);
                        board.move_back();
                        return true;
                    }
                    board.move_back();
                }
            }
        }
        return false;
    }

// record answers on the first exploration call
    private void record_solution(int step, int step0, int i, int j)
    {   if(step!=step0) return;
        answer_x = i;
        answer_y = j;
    }

// record local draw solution
    private void record_localdraw(int step, int step0, int i, int j)
    {   if(step!=step0) return;
        current_x = i;
        current_y = j;
    }

// randomly choose a move. Used when a lose is potentially inevitable.
    private void compromise()
    {   Random rand = new Random();
        int i,j;
        do
        {   i = rand.nextInt(3);
            j = rand.nextInt(3);
        } while(!board.move(i,j));
        board.move_back();
        answer_x = i;
        answer_y = j;
    }

    private void test()
    {    test_overall();
         test_converge();
         test_compromise();
         test_record();
         System.out.println("All tests passed");
    }

    private void test_overall()
    {   assert(run()==null);
        board.move(0,0);
        assert(run()==null);
        board.move(0,1);
        assert(run()==Oxo.o);
        board.move(1,0);

        // a case dived into compromise
        assert(run()==Oxo.o);
        assert(board.move(answer_x,answer_y));
        board.move_back();

        board.move(1,1);
        assert(run()==Oxo.o);
        assert(answer_x==2 && answer_y==0);
        board.rewind();
        board.move(1,1);
        board.move(1,0);
        assert(run()==Oxo.o);
        board.rewind();
        board.move(0,0);
        board.move(1,1);
        board.move(1,2);
        board.move(0,2);
        board.move(0,1);
        assert(run()==Oxo.x);
        assert(answer_x==2 && answer_y==0);
        board.rewind();

        //a case that is a draw
        board.move(2,2);
        board.move(1,1);
        board.move(2,1);
        board.move(2,0);
        board.move(0,2);
        board.move(1,2);
        board.move(1,0);
        board.move(0,1);
        board.move(0,0);
        assert(run()==null);
        assert(explore()==null);
        board.rewind();
    }

    private void test_converge()
    {   board.move(0,0);
        board.move(0,1);
        board.move(1,1);
        board.move(1,0);
        assert(converge());
        board.rewind();
        board.move(0,0);
        board.move(0,1);
        board.move(1,0);
        board.move(1,1);
        assert(converge());
    }

    private void test_compromise()
    {   int i;
        board.move(0,0);
        board.move(1,0);
        board.move(1,1);
        for(i=0;i<50;i++)
        {   compromise();
            assert(0<=answer_x && answer_x<=2);
            assert(0<=answer_y && answer_y<=2);
            assert(board.move(answer_x,answer_y));
            board.move_back();
        }
        board.rewind();
    }

    private void test_record()
    {   initial_step = board.step;
        current_x = 0; current_y = 0;
        answer_x = 0; answer_y = 0;
        board.move(0,0);
        record_solution(board.step, initial_step, 1, 1);
        assert(answer_x!=1 && answer_y!=1);
        record_localdraw(board.step, initial_step, 1, 1);
        assert(current_x!=1 && current_y!=1);
        board.move_back();
        record_solution(board.step, initial_step, 1, 1);
        assert(answer_x==1 && answer_y==1);
        record_localdraw(board.step, initial_step, 1, 1);
        assert(current_x==1 && current_y==1);
        board.rewind();
    }

}
