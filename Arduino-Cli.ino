//This Script was designed to run with the linux command 'screen' in something 
//simmilar to 'screen /dev/ttyACM0', replacing 'ttyACM0' with your board's Port.

const String Name="Console> ";
const String Cursor="⦁";

void Startup() {  //STARTUP MESSAGE
  Serial.print("\033c");
  Serial.println("Wait for the typed letters to appear before submitting.");
  Serial.println("Type \"help\" for a list of commands.");
  Serial.print(Name + Cursor +"\r");
}

void setup() {
  Serial.begin(115200);
  Startup();
}

//VARIABLES ABOUT THE TYPED WORDS
String Input="";
String incomingString="";
int length=0;
int command = 0;
int ELSe=0;
int iLength=0;

//COMMANDS
String HELp="help";
String CLEAr="clear";
String REBOOt="reboot";
String TIMe="time";

//VARIABLES FOR PROCESSES
int i=0;
int iteration=0;


void Help() { //HELP
  Serial.println("");
  Serial.println("help\tDisplays this text.");
  Serial.println("clear\tClears the screen.");
  Serial.println("reboot\tResets the board.");
  Serial.println("time\tDisplay the time since boot.");
  Serial.println("");
}

void(* resetFunc) (void) = 0;  //REBOOT
void Clear() {Serial.print("\033c");}  //CLEAR
void Time(){  //TIME
  //Minutes '-m'
  if (Input.length() > TIMe.length() && Input.charAt(Input.indexOf("-") + 1) == 109) {
    Serial.println(millis() / 60000);
  }else{
    Serial.println(millis() / 1000);
  }
}


void loop() {
  if (Serial.available() > 0) {
    command = 0;
    ELSe=0;
    incomingString = Serial.readString();
    if (incomingString.indexOf(13) >= 0) {  //ENTER
      Serial.println("\033[2K" + Name + Input + "\r"); /*To remove the cursor from previous line*/
      if (Input.length()==0) {  /*If input IS empty*/
        Serial.print("\r");
      } else {  /*If input ISN'T empty*/
        //HELP
        iteration=0;
        while (iteration < Input.length() && Input.charAt(iteration) == HELp.charAt(iteration)) {  /*Checks for length and order*/
          iteration=iteration + 1;
        }
        if (iteration == HELp.length() && (! Input.charAt(iteration) || Input.indexOf(32) == HELp.length())) {  /*If input IS 'clear' OR 'clear '*/
          Help();
          command=1;
        }
        //CLEAR
        iteration=0;
        while (iteration < Input.length() && Input.charAt(iteration) == CLEAr.charAt(iteration)) {  /*Checks for length and order*/
          iteration=iteration + 1;
        }
        if (iteration == CLEAr.length() && (! Input.charAt(iteration) || Input.indexOf(32) == CLEAr.length())) {  /*If input IS 'clear' OR 'clear '*/
          Clear();
          command=1;
        }
        //REBOOT
        iteration=0;
        while (iteration < Input.length() && Input.charAt(iteration) == REBOOt.charAt(iteration)) {  /*Checks for length and order*/
          iteration=iteration + 1;
        }
        if (iteration == REBOOt.length() && (! Input.charAt(iteration) || Input.indexOf(32) == REBOOt.length())) {  /*If input IS 'reboot' OR 'reboot '*/
          resetFunc();
          command=1;
        }
        //TIME
        iteration=0;
        while (iteration < Input.length() && Input.charAt(iteration) == TIMe.charAt(iteration)) {  /*Checks for length and order*/
          iteration=iteration + 1;
        }
        if (iteration == TIMe.length() && (! Input.charAt(iteration) || Input.indexOf(32) == TIMe.length())) {  /*If input IS 'reboot' OR 'reboot '*/
          Time();
          command=1;
        }

        //INVALID COMMAND
        if (command != 1) {  /*If input ISN'T a command*/
          Serial.println( "\"" + Input + "\" Is not a command");
        }
          Input = "";  /*Reset the input after every enter*/
      }
    } else if (incomingString.indexOf(127) >= 0) {  //BACKSPACE
      iteration=0;
      while (iteration < incomingString.length() && ELSe==0) {  /*Detect if NOT ONLY backspaces in string*/
        if (incomingString.charAt(iteration) != 127) {ELSe=1;}
        iteration=iteration + 1;
      }
      iteration=0;
      while (incomingString.indexOf(127) >= 0 && ELSe==1) {  /*Check where to delete*/
        if (incomingString.indexOf(127) >= 0) {  /*If there ARE backspaces*/
          while (iteration == 0 && (incomingString.charAt(iteration) == 127)){  /*Checks if string starts with backspaces*/
            iLength=Input.length();
            if (iLength != 0){  /*If there's room to delete, do so*/
              Input.remove(iLength - 1);
            } else {Serial.print("\a");}
            incomingString.remove(0,1);
          }
          /*This only runs AFTER checking the starter backspaces*/
          if (incomingString.indexOf(127) >= 0) {
            incomingString.remove(incomingString.indexOf(127) - 1, 2);
          }
        }
        iteration=iteration + 1;
      }
      if (ELSe==1) {  /*An update if there WEREN'T ONLY backspaces*/
        Input= Input + incomingString;
      }

      if (ELSe==0) {  /*If its ONLY backspaces*/
        length = incomingString.length();
        iLength=Input.length();
        if (length > iLength ) {  /*If you try to delete more times than there's letters*/
          length=iLength;
        }
        Input.remove(iLength - length);
      }
    } else {  //BASIC INPUT
      Input=Input + incomingString;
    }

    iteration=0;
    while (Input.indexOf("\x1B[A") >= 0 || Input.indexOf("\x1B[B") >= 0 || Input.indexOf("\x1B[C") >= 0 || Input.indexOf("\x1B[D") >= 0 || Input.indexOf(127) >= 0 || Input.indexOf(9) >= 0) {
      if (Input.indexOf(127) >= 0) {Input.remove(Input.indexOf(127), 1);}
      if (Input.indexOf("\x1B[A") >= 0) {Input.remove(Input.indexOf("\x1B[A"), 3);}
      if (Input.indexOf("\x1B[B") >= 0) {Input.remove(Input.indexOf("\x1B[B"), 3);}
      if (Input.indexOf("\x1B[C") >= 0) {Input.remove(Input.indexOf("\x1B[C"), 3);}
      if (Input.indexOf("\x1B[D") >= 0) {Input.remove(Input.indexOf("\x1B[D"), 3);}
      if (Input.indexOf(9) >= 0) {Input.remove(Input.indexOf(9), 1);}
      iteration=iteration + 1;
    }
    Serial.print("\033[2K" + Name + Input + Cursor + "\r");
  }
}
