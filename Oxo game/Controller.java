import java.util.*;
import javafx.application.*;
import javafx.stage.*;
import javafx.geometry.*;
import javafx.scene.*;
import javafx.scene.layout.*;
import javafx.scene.control.*;

public class Controller extends Application
{   private Board gameboard = new Board();
    private Button[] btns = new Button[9];
    private boolean Ai_switch = false;
    private boolean finished = false;
    Label footer,Aiwhisper;
    private static String[] command;

    public static void main(String[] args)
    {   command = args;
        launch(args);
    }

    public void start(Stage stage)
    {   Controller oxo = new Controller();
        GridPane grid = new GridPane();
        oxo.grid_initial(grid);

        //test mode
        if(command.length==1)
        {   if(command[0].equals("test"))
            {   oxo.test();
                System.exit(0);
            }
        }
        //normal mode
        Scene scene = new Scene(grid);
        scene.getStylesheets().add("./style.css");
        stage.setTitle("Monde du Citron!");
        stage.setScene(scene);
        stage.show();
    }

    private void test()
    {   test_oxobutton();
        test_ai_move();
        System.out.println("All tests passed");
    }

    private void test_oxobutton()
    {   Button btn = new Button("");
        Buttonpress(btn,0,0);
        assert(btn.getText().equals("O"));
        Buttonpress(btn,0,0);
        assert(btn.getText().equals("O"));
        Buttonpress(btn,0,1);
        assert(btn.getText().equals("X"));
        rewind();
    }

    private void test_ai_move()
    {   ai_move();
        assert(gameboard.step==1);
        ai_move();
        assert(gameboard.step==2);

        // AI play against AI will result in a draw
        for(int i=0;i<8;i++) ai_move();
        Ai test = new Ai(gameboard);
        assert(test.run()==null);
        rewind();
    }

    private void grid_initial(GridPane grid)
    {   int i,j;
        Button btn;
        grid.setPadding(new Insets(25,25,25,25));
        grid.setVgap(3);
        grid.setHgap(2);
        Label header = new Label("Oxo Game");
        header.setId("label_header");
        GridPane.setConstraints(header,0,0,2,1);
        grid.getChildren().add(header);

        footer = new Label("");
        footer.setId("label_footer");
        footer.setPrefWidth(390);
        footer.setPrefHeight(40);
        footer.setAlignment(Pos.CENTER);
        GridPane.setConstraints(footer,0,5,3,1);
        grid.getChildren().add(footer);
        footer_display();

        Aiwhisper = new Label("");
        Aiwhisper.setId("label_aiwhisper");
        Aiwhisper.setPrefWidth(390);
        footer.setPrefHeight(40);
        Aiwhisper.setAlignment(Pos.CENTER);
        GridPane.setConstraints(Aiwhisper,0,6,3,1);
        grid.getChildren().add(Aiwhisper);

        Button reset = ResetButton();
        GridPane.setConstraints(reset,2,4);
        grid.getChildren().add(reset);

        Button ai = AiButton();
        GridPane.setConstraints(ai,0,4,2,1);
        grid.getChildren().add(ai);

        CheckBox switchbox = switchbox();
        GridPane.setConstraints(switchbox,2,0,1,1, HPos.RIGHT, VPos.TOP);
        grid.getChildren().add(switchbox);

        // add Oxo buttons
        for(i=0;i<3;i++)
        {   for(j=1;j<4;j++)
           {   GridPane.setConstraints(btn=OxoButton(i,j-1),i,j);
               grid.getChildren().add(btn);
               btns[i*3+j-1]=btn;
           }
        }
    }

    private Button OxoButton(int x, int y)
    {   Button btn = new Button("");
        btn.setPrefHeight(130);
        btn.setPrefWidth(130);
        btn.setOnAction(e->
        {   if(!finished) Buttonpress(btn, x, y);
        });
        return btn;
    }

    private void Buttonpress(Button btn, int x, int y)
    {   boolean move = false;
        if(move=gameboard.move(x, y))
        {   if(gameboard.current==Oxo.o)
            {    btn.setText("X");
            }
            else
            {    btn.setText("O");
            }
        }
        if(gameboard.win_check())
        {   winning();
        }
        else
        {   footer_display();
            predict();
        }
        if(Ai_switch && move)
        {   ai_move();
        }
    }

    private Button ResetButton()
    {   Button reset = new Button("Reset");
        reset.setPrefWidth(130);
        reset.setId("button_reset");
        reset.setOnAction(e->
        {   rewind();
            footer_display();
        });
        return reset;
    }

    private Button AiButton()
    {   Button btn = new Button("Let Me Help You");
        btn.setPrefWidth(260);
        btn.setId("button_ai");
        btn.setOnAction(e->
        {   if(!finished)
            {   ai_move();
               if(Ai_switch)
               {   ai_move();
               }
            }
        });
       return btn;
   }

   private void ai_move()
   {   Oxo winner;
       Ai ai = new Ai(gameboard);
       winner = ai.run();
       if(gameboard.move(ai.answer_x, ai.answer_y))
       {   if(gameboard.current==Oxo.o)
           {    btns[ai.answer_x*3+ai.answer_y].setText("X");
           }
           else
           {    btns[ai.answer_x*3+ai.answer_y].setText("O");
           }
       }
       if(gameboard.win_check())
       {   winning();
       }
       else
       {   footer_display();
           predict();
       }
   }

   private CheckBox switchbox()
   {   CheckBox btn = new CheckBox("AI");
       btn.setPrefWidth(80);
       btn.setPrefHeight(40);
       btn.setId("check-box_switch");
       btn.setOnAction(e->
       {    Ai_switch = btn.isSelected();
       });
       return btn;
   }

   private void predict()
   {   Oxo winner;
       Ai ai = new Ai(gameboard);
       winner = ai.run();
       if(winner==Oxo.o)
       Aiwhisper.setText("Devil whisper: Player O can win");
       else if(winner==Oxo.x)
       Aiwhisper.setText("Devil whisper: Player X can win");
       else
       Aiwhisper.setText("Devil whisper: Best result is a draw");
   }

    private void footer_display()
    {   footer.setId("label_footer");
        if(gameboard.current==Oxo.o) footer.setText(" Player O");
        else footer.setText(" Player X");
    }

    private void winning()
    {   footer.setId("label_win");
        if(gameboard.current==Oxo.o) footer.setText("Player X has won !!!");
        else footer.setText("Player O has won !!!");
        celebrate();
        finished = true;
    }

// a simple & nasty way to ask which line of buttons to light up
    private void celebrate()
    {   int i,j;
        String result = gameboard.win_position();
        if(result.charAt(0)=='H')
        {   i = result.charAt(1)-'0';
            for(j=0;j<3;j++) btns[i+3*j].setId("button_win");
        }
        if(result.charAt(0)=='V')
        {   i = result.charAt(1)-'0';
            for(j=0;j<3;j++) btns[3*i+j].setId("button_win");
        }
        if(result.charAt(0)=='D')
        {   i = result.charAt(1)-'0';
            if(i==0) for(j=0;j<3;j++) btns[i+j*4].setId("button_win");
            else for(j=0;j<3;j++) btns[i+j*2+1].setId("button_win");
        }
    }

    private void rewind()
    {   int i,j;
        for(i=0;i<9;i++)
        {   btns[i].setText(" ");
            btns[i].setId("button");
        }
        Aiwhisper.setText("");
        gameboard.rewind();
        finished = false;
    }
}
