// generated by Fast Light User Interface Designer (fluid) version 1.0303

#include "inactive.h"

Fl_Group *the_group=(Fl_Group *)0;

Fl_Menu_Item menu_menu[] = {
 {"item", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"item", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"item", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"item", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"item", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

static void cb_active(Fl_Button*, void*) {
  the_group->activate();
}

static void cb_inactive(Fl_Button*, void*) {
  the_group->deactivate();
}

int main(int argc, char **argv) {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = new Fl_Double_Window(420, 369);
    w = o;
    { the_group = new Fl_Group(25, 25, 375, 295, "activate()/deactivate() called on this Fl_Group");
      the_group->box(FL_ENGRAVED_FRAME);
      the_group->align(Fl_Align(FL_ALIGN_TOP|FL_ALIGN_INSIDE));
      { new Fl_Button(50, 50, 105, 25, "button");
      } // Fl_Button* o
      { Fl_Light_Button* o = new Fl_Light_Button(50, 80, 105, 25, "light button");
        o->value(1);
        o->align(Fl_Align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE));
      } // Fl_Light_Button* o
      { Fl_Group* o = new Fl_Group(50, 130, 105, 125, "Child group");
        o->box(FL_DOWN_FRAME);
        { Fl_Check_Button* o = new Fl_Check_Button(50, 170, 105, 25, "red");
          o->type(102);
          o->down_box(FL_DIAMOND_DOWN_BOX);
          o->selection_color((Fl_Color)1);
          o->labelcolor((Fl_Color)1);
        } // Fl_Check_Button* o
        { Fl_Check_Button* o = new Fl_Check_Button(50, 190, 105, 25, "green");
          o->type(102);
          o->down_box(FL_DIAMOND_DOWN_BOX);
          o->selection_color((Fl_Color)2);
          o->labelcolor((Fl_Color)2);
        } // Fl_Check_Button* o
        { Fl_Check_Button* o = new Fl_Check_Button(50, 210, 105, 25, "blue");
          o->type(102);
          o->down_box(FL_DIAMOND_DOWN_BOX);
          o->selection_color((Fl_Color)4);
          o->labelcolor((Fl_Color)4);
        } // Fl_Check_Button* o
        { Fl_Check_Button* o = new Fl_Check_Button(50, 230, 105, 25, "white");
          o->type(102);
          o->down_box(FL_DIAMOND_DOWN_BOX);
          o->selection_color((Fl_Color)55);
          o->labelcolor((Fl_Color)55);
        } // Fl_Check_Button* o
        { Fl_Check_Button* o = new Fl_Check_Button(50, 130, 105, 25, "check");
          o->down_box(FL_DOWN_BOX);
        } // Fl_Check_Button* o
        { Fl_Round_Button* o = new Fl_Round_Button(50, 150, 105, 25, "round");
          o->down_box(FL_ROUND_DOWN_BOX);
        } // Fl_Round_Button* o
        o->end();
      } // Fl_Group* o
      { Fl_Slider* o = new Fl_Slider(165, 50, 24, 205, "Fl_Slider");
        o->value(0.5);
      } // Fl_Slider* o
      { Fl_Input* o = new Fl_Input(195, 50, 195, 30);
        o->static_value("Fl_Input");
      } // Fl_Input* o
      { Fl_Menu_Button* o = new Fl_Menu_Button(245, 90, 130, 30, "menu");
        o->menu(menu_menu);
      } // Fl_Menu_Button* o
      { Fl_Value_Output* o = new Fl_Value_Output(245, 130, 130, 30, "value:");
        o->maximum(10000);
        o->step(1);
        o->textfont(5);
        o->textsize(24);
        o->textcolor((Fl_Color)4);
      } // Fl_Value_Output* o
      { Fl_Box* o = new Fl_Box(245, 170, 140, 50, "Fl_Box");
        o->box(FL_EMBOSSED_FRAME);
        o->labeltype(FL_SHADOW_LABEL);
        o->labelfont(3);
        o->labelsize(38);
      } // Fl_Box* o
      { Fl_Scrollbar* o = new Fl_Scrollbar(40, 274, 180, 20, "scrollbar");
        o->type(1);
      } // Fl_Scrollbar* o
      { new Fl_Roller(235, 230, 25, 65, "roller");
      } // Fl_Roller* o
      { new Fl_Dial(275, 235, 50, 50, "dial");
      } // Fl_Dial* o
      { new Fl_Clock(335, 235, 50, 50, "clock");
      } // Fl_Clock* o
      the_group->end();
      Fl_Group::current()->resizable(the_group);
    } // Fl_Group* the_group
    { Fl_Button* o = new Fl_Button(25, 330, 185, 25, "active");
      o->type(102);
      o->value(1);
      o->callback((Fl_Callback*)cb_active);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(220, 330, 180, 25, "inactive");
      o->type(102);
      o->callback((Fl_Callback*)cb_inactive);
    } // Fl_Button* o
    o->end();
  } // Fl_Double_Window* o
  w->show(argc, argv);
  return Fl::run();
}