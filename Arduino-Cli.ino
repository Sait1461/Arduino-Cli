//This Script was designed to run with the linux command 'screen' in something 
//simmilar to 'screen /dev/ttyACM0', replacing 'ttyACM0' with your board's Port.
const String Name="Console> ";
const String Cursor="⦁";

void Banner() {  //STARTUP MESSAGE
  Serial.print("\033c");
  Serial.println("Wait for the typed letters to appear before submitting.");
  Serial.println("Type \"help\" for a list of commands.");
  Serial.print(Name + Cursor +"\r");
}
void setup() {
  Serial.begin(115200);
  Banner();
}

//VARIABLES ABOUT THE TYPED WORDS
String Input="";
String incomingString="";
int length=0;
bool command = false;
bool ELSe=false;
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
  if (Serial.available() > 0) {  //If theres anything being typed at all
    incomingString = Serial.readString();
    if (incomingString.indexOf(3) >= 0) { //CTRL-C
      Serial.println("\033[2K" + Name + Input + "^C\r");
      Serial.print("\r");
      Input="";
    } else if (incomingString.indexOf(13) >= 0) {  //ENTER
      command=false;
      Serial.println("\033[2K" + Name + Input + "\r"); /*To remove the cursor from previous line*/
      if (Input.length()==0) {  /*If input IS empty*/
        Serial.print("\r");
      } else {  /*If input ISN'T empty*/
      //HELP
      iteration=0;
      while (iteration < Input.length() && Input.charAt(iteration) == HELp.charAt(iteration)) {iteration=iteration + 1;}/*Checks for length and order*/
      if (iteration == HELp.length() && (! Input.charAt(iteration) || Input.indexOf(32) == HELp.length())) {  /*If input IS 'clear' OR 'clear '*/
        Help();
        command=true;
      }
      //CLEAR
      iteration=0;
      while (iteration < Input.length() && Input.charAt(iteration) == CLEAr.charAt(iteration)) {iteration=iteration + 1;}/*Checks for length and order*/
      if (iteration == CLEAr.length() && (! Input.charAt(iteration) || Input.indexOf(32) == CLEAr.length())) {  /*If input IS 'clear' OR 'clear '*/
        Clear();
        command=true;
      }
      //REBOOT
      iteration=0;
      while (iteration < Input.length() && Input.charAt(iteration) == REBOOt.charAt(iteration)) {iteration=iteration + 1;}/*Checks for length and order*/
      if (iteration == REBOOt.length() && (! Input.charAt(iteration) || Input.indexOf(32) == REBOOt.length())) {  /*If input IS 'reboot' OR 'reboot '*/
        resetFunc();
        command=true;
      }
      //TIME
      iteration=0;
      while (iteration < Input.length() && Input.charAt(iteration) == TIMe.charAt(iteration)) {iteration=iteration + 1;}/*Checks for length and order*/
      if (iteration == TIMe.length() && (! Input.charAt(iteration) || Input.indexOf(32) == TIMe.length())) {  /*If input IS 'reboot' OR 'reboot '*/
        Time();
        command=true;
      }
      //INVALID COMMAND
      if ( ! command) {  /*If input ISN'T a command*/
        Serial.println( "\"" + Input + "\" Is not a command");
      }
      Input = "";  /*Reset the input after every enter*/
      }
    } else if (incomingString.indexOf(127) >= 0) {  //BACKSPACE
      ELSe=false;
      iteration=0;
      while (iteration < incomingString.length() && ! ELSe) {  /*Detect if NOT ONLY backspaces in string*/
        if (incomingString.charAt(iteration) != 127) {ELSe=true;}
        iteration=iteration + 1;
      }
      iteration=0;
      while (incomingString.indexOf(127) >= 0 && ELSe) {  /*Check where to delete*/
        if (incomingString.indexOf(127) >= 0) {  /*If there ARE backspaces*/
          while (iteration == 0 && (incomingString.charAt(iteration) == 127)){  /*Checks if string starts with backspaces*/
            if (Input.length() != 0){  /*If there's room to delete, do so*/
              Input.remove(Input.length() - 1);
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
      if (ELSe) {  /*An update if there WEREN'T ONLY backspaces*/
        Input= Input + incomingString;
      }

      if ( ! ELSe) {  /*If its ONLY backspaces*/
        length = incomingString.length();
        if (length > Input.length() ) {  /*If you try to delete more times than there's letters*/
          length=Input.length();
        }
        Input.remove(Input.length() - length);
      }
    } else {  //BASIC INPUT
      Input=Input + incomingString;
    }
    //DETECT & REMOVE INVALID CHARS (SHOUDLN'T BE ANY)
    while (Input.indexOf("\x1B[A") >= 0 || Input.indexOf("\x1B[B") >= 0 || Input.indexOf("\x1B[C") >= 0 || Input.indexOf("\x1B[D") >= 0 || Input.indexOf(127) >= 0 || Input.indexOf(9) >= 0) {
      if (Input.indexOf(127) >= 0) {Input.remove(Input.indexOf(127), 1);}  /*Enter*/
      if (Input.indexOf("\x1B[A") >= 0) {Input.remove(Input.indexOf("\x1B[A"), 3);}  /*Up*/
      if (Input.indexOf("\x1B[B") >= 0) {Input.remove(Input.indexOf("\x1B[B"), 3);}  /*Down*/
      if (Input.indexOf("\x1B[C") >= 0) {Input.remove(Input.indexOf("\x1B[C"), 3);}  /*Right*/
      if (Input.indexOf("\x1B[D") >= 0) {Input.remove(Input.indexOf("\x1B[D"), 3);}  /*Left*/
      if (Input.indexOf(9) >= 0) {Input.remove(Input.indexOf(9), 1);}  /*Tab*/
    }
    Serial.print("\033[2K" + Name + Input + Cursor + "\r");
  }
}
