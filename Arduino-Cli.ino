//This Script was designed to run with the linux command 'screen' in something 
//simmilar to 'screen /dev/ttyACM0', replacing 'ttyACM0' with your board's Port.
const String Name="Console> ";
const String Cursor="⦁";

void Banner() {  //STARTUP MESSAGE
  Serial.print("\033c");
  Serial.println("Type \"help\" for a list of commands.");
  Serial.print(Name + Cursor +"\r");
}
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(0);
  Banner();
}

//VARIABLES ABOUT THE TYPED WORDS
String Input="";
String incomingString="";
bool command = false;
//VARIABLES FOR PROCESSES
int iteration=0;
//COMMANDS
const String Commands[] = {"help","clear","reboot","time"};

void Help() { //HELP
  Serial.println("");
  Serial.println("help\tDisplays this text.");
  Serial.println("clear\tClears the screen.");
  Serial.println("reboot\tResets the board.");
  Serial.println("time\tDisplay the time since boot.");
  Serial.println("");
}
void(* Reboot) (void) = 0;  //REBOOT
void Clear() {Serial.print("\033c");}  //CLEAR
void Time(){  //TIME
  if (Input.length() > Commands[4].length() && Input.charAt(Input.indexOf("-") + 1) == 109) {//Minutes '-m'
    Serial.print(millis() / 60000);Serial.println("m");
  } else if (Input.length() > Commands[4].length() && Input.charAt(Input.indexOf("-") + 1) == 77) {//Minutes with float '-M'
    Serial.print(millis() / 60000.0);Serial.println("m");
  } else {
    Serial.print(millis() / 1000);Serial.println("s");
  }
}

void All(){
  if (iteration == 0) {
    Help();
  } else if (iteration == 1) {
    Clear();
  } else if (iteration == 2) {
    Reboot();
  } else if (iteration == 3) {
    Time();
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
        //CHECK IF INPUT IS ANY COMMAND
        iteration=0;
        while (iteration < sizeof(Commands)/sizeof(Commands[0]) && ! command) {
          int i=0;
          while (i < Input.length() && Input.charAt(i) == Commands[iteration].charAt(i)) {i++;}/*Checks for length and order*/
          if (i == Commands[iteration].length() && (! Input.charAt(i) || Input.indexOf(32) == Commands[iteration].length())) {  /*If input IS '[COMMAND]]' OR '[COMMAND] '*/
          command=true;
          All();
          }
          iteration++;
        }
        //INVALID COMMAND
        if ( ! command) {  /*If input ISN'T a command*/
          Serial.println( "\"" + Input + "\" Is not a command");
        }
        Input = "";  /*Reset the input after every enter*/
      }
    } else if (incomingString.indexOf(127) >= 0) {  //BACKSPACE
      if (incomingString.length() > Input.length() ) {  /*If you try to delete more times than there's letters*/
        Input.remove(0,Input.length());
      } else {
        Input.remove(Input.length() - incomingString.length());
      }
    } else {  //BASIC INPUT
      Input=Input + incomingString;
    }
    //DETECT & REMOVE INVALID CHARS
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
