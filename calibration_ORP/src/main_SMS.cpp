// Include the MKRNB library
#include <Arduino.h>


// Include the MKRNB library
#include <MKRNB.h>
// #include "arduino_secrets.h" 
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[] = "";

//Read input serial
int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}

// initialize the library instance
NB nbAccess;
NB_SMS sms;

void setup() {
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("SMS Messages Sender");

  // connection state
  bool connected = false;

  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (!connected) {
    // if (nbAccess.begin(PINNUMBER) == NB_READY) {
    if (nbAccess.begin(PINNUMBER) == NB_READY) {

      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("NB initialized");
}

void loop() {

  Serial.print("Enter a mobile number: ");
  char remoteNum[20];  // telephone number to send sms
  readSerial(remoteNum);
  Serial.println(remoteNum);

  // sms text
  Serial.print("Now, enter SMS content: ");
  char txtMsg[200];
  readSerial(txtMsg);
  Serial.println("SENDING");
  Serial.println();
  Serial.println("Message:");
  Serial.println(txtMsg);

  // send the message
  sms.beginSMS(remoteNum);
  sms.print(txtMsg);
  sms.endSMS();
  Serial.println("\nCOMPLETE!\n");
}













// #include <MKRNB.h>
// // #include "arduino_secrets.h" 
// // Please enter your sensitive data in the Secret tab or arduino_secrets.h
// // PIN Number
// const char PINNUMBER[] = "";

// // initialize the library instance
// NB nbAccess;
// NB_SMS sms;
// //Read input serial
// int readSerial(char result[]) {
//   int i = 0;
//   while (1) {
//     while (Serial.available() > 0) {
//       char inChar = Serial.read();
//       if (inChar == '\n') {
//         result[i] = '\0';
//         Serial.flush();
//         return 0;
//       }
//       if (inChar != '\r') {
//         result[i] = inChar;
//         i++;
//       }
//     }
//   }
// }

// String readSerial() {

//   String text = "";

//   while (1) {

//     while (Serial.available() > 0) {

//       char inChar = Serial.read();

//       if (inChar == '\n') {

//         return text;

//       }

//       if (inChar != '\r') {

//         text += inChar;

//       }

//     }

//   }
// }

// // void setup() {


// // pin manager object

// NBPIN PINManager;

// // save input in serial by user

// String user_input = "";

// // authenticated with PIN code
// boolean auth = false;

// // serial monitor result messages

// String oktext = "OK";

// String errortext = "ERROR";

// void setup() {

//   // initialize serial communications and wait for port to open:

//   Serial.begin(9600);

//   while (!Serial) {

//     ; // wait for serial port to connect. Needed for Leonardo only

//   }

//   Serial.println("Change PIN example\n");

//   PINManager.begin();

//   // check if the SIM have pin lock

//   while (!auth) {

//     int pin_query = PINManager.isPIN();

//     if (pin_query == 1) {

//       // if SIM is locked, enter PIN code

//       Serial.print("Enter PIN code: ");

//       user_input = readSerial();

//       // check PIN code

//       if (PINManager.checkPIN(user_input) == 0) {

//         auth = true;

//         PINManager.setPINUsed(true);

//         Serial.println(oktext);

//       } else {

//         // if PIN code was incorrected

//         Serial.println("Incorrect PIN. Remember that you have 3 opportunities.");

//       }

//     } else if (pin_query == -1) {

//       // PIN code is locked, user must enter PUK code

//       Serial.println("PIN locked. Enter PUK code: ");

//       String puk = readSerial();

//       Serial.print("Now, enter a new PIN code: ");

//       user_input = readSerial();

//       // check PUK code

//       if (PINManager.checkPUK(puk, user_input) == 0) {

//         auth = true;

//         PINManager.setPINUsed(true);

//         Serial.println(oktext);

//       } else {

//         // if PUK o the new PIN are incorrect

//         Serial.println("Incorrect PUK or invalid new PIN. Try again!.");

//       }

//     } else if (pin_query == -2) {

//       // the worst case, PIN and PUK are locked

//       Serial.println("PIN and PUK locked. Use PIN2/PUK2 in a mobile phone.");

//       while (true);

//     } else {

//       // SIM does not requires authetication

//       Serial.println("No pin necessary.");

//       auth = true;

//     }

//   }

//   // start module

//   Serial.print("Checking register in NB IoT / LTE Cat M1 network...");

//   if (PINManager.checkReg() == 0) {

//     Serial.println(oktext);

//   }

//   // if you are connect by roaming

//   else if (PINManager.checkReg() == 1) {

//     Serial.println("ROAMING " + oktext);

//   } else {

//     // error connection

//     Serial.println(errortext);

//     while (true);

//   }
// // }
//   // initialize serial communications and wait for port to open:
//   Serial.begin(9600);
//   while (!Serial) {
//     ; // wait for serial port to connect. Needed for native USB port only
//   }

//   Serial.println("SMS Messages Sender");

//   // connection state
//   bool connected = false;

//   // If your SIM has PIN, pass it as a parameter of begin() in quotes
//   while (!connected) {
//     Serial.println("testing");
//     // if (nbAccess.begin(PINNUMBER) == NB_READY) {
//     if (nbAccess.begin() == NB_READY) {
//       connected = true;
//     } else {
//       Serial.println("Not connected");
//       delay(1000);
//     }
//   }

//   Serial.println("NB initialized");
// }

// void loop() {

//   Serial.print("Enter a mobile number: ");
//   char remoteNum[20];  // telephone number to send sms
//   readSerial(remoteNum);
//   Serial.println(remoteNum);

//   // sms text
//   Serial.print("Now, enter SMS content: ");
//   char txtMsg[200];
//   readSerial(txtMsg);
//   Serial.println("SENDING");
//   Serial.println();
//   Serial.println("Message:");
//   Serial.println(txtMsg);

//   // send the message
//   sms.beginSMS(remoteNum);
//   sms.print(txtMsg);
//   sms.endSMS();
//   Serial.println("\nCOMPLETE!\n");
// }










// // //////////////////////

// // /*

// //  This example enables you to change or remove the PIN number of

// //  a SIM card inserted into a MKR NB 1500 board.

// //  Circuit:

// //  * MKR NB 1500 board

// //  * Antenna

// //  * SIM card

// //  Created 12 Jun 2012

// //  by David del Peral

// // */

// // // libraries
// // #include <MKRNB.h>

// // // pin manager object

// // NBPIN PINManager;

// // // save input in serial by user

// // String user_input = "";

// // // authenticated with PIN code
// // boolean auth = false;

// // // serial monitor result messages

// // String oktext = "OK";

// // String errortext = "ERROR";

// // void setup() {

// //   // initialize serial communications and wait for port to open:

// //   Serial.begin(9600);

// //   while (!Serial) {

// //     ; // wait for serial port to connect. Needed for Leonardo only

// //   }

// //   Serial.println("Change PIN example\n");

// //   PINManager.begin();

// //   // check if the SIM have pin lock

// //   while (!auth) {

// //     int pin_query = PINManager.isPIN();

// //     if (pin_query == 1) {

// //       // if SIM is locked, enter PIN code

// //       Serial.print("Enter PIN code: ");

// //       user_input = readSerial();

// //       // check PIN code

// //       if (PINManager.checkPIN(user_input) == 0) {

// //         auth = true;

// //         PINManager.setPINUsed(true);

// //         Serial.println(oktext);

// //       } else {

// //         // if PIN code was incorrected

// //         Serial.println("Incorrect PIN. Remember that you have 3 opportunities.");

// //       }

// //     } else if (pin_query == -1) {

// //       // PIN code is locked, user must enter PUK code

// //       Serial.println("PIN locked. Enter PUK code: ");

// //       String puk = readSerial();

// //       Serial.print("Now, enter a new PIN code: ");

// //       user_input = readSerial();

// //       // check PUK code

// //       if (PINManager.checkPUK(puk, user_input) == 0) {

// //         auth = true;

// //         PINManager.setPINUsed(true);

// //         Serial.println(oktext);

// //       } else {

// //         // if PUK o the new PIN are incorrect

// //         Serial.println("Incorrect PUK or invalid new PIN. Try again!.");

// //       }

// //     } else if (pin_query == -2) {

// //       // the worst case, PIN and PUK are locked

// //       Serial.println("PIN and PUK locked. Use PIN2/PUK2 in a mobile phone.");

// //       while (true);

// //     } else {

// //       // SIM does not requires authetication

// //       Serial.println("No pin necessary.");

// //       auth = true;

// //     }

// //   }

// //   // start module

// //   Serial.print("Checking register in NB IoT / LTE Cat M1 network...");

// //   if (PINManager.checkReg() == 0) {

// //     Serial.println(oktext);

// //   }

// //   // if you are connect by roaming

// //   else if (PINManager.checkReg() == 1) {

// //     Serial.println("ROAMING " + oktext);

// //   } else {

// //     // error connection

// //     Serial.println(errortext);

// //     while (true);

// //   }
// // }

// // void loop() {

// //   // Function loop implements pin management user menu

// //   // Only if you SIM use pin lock, you can change PIN code

// //   // user_op variables save user option

// //   Serial.println("Choose an option:\n1 - On/Off PIN.");

// //   if (PINManager.getPINUsed()) {

// //     Serial.println("2 - Change PIN.");

// //   }

// //   String user_op = readSerial();

// //   if (user_op == "1") {

// //     Serial.println("Enter your PIN code:");

// //     user_input = readSerial();

// //     // activate/deactivate PIN lock

// //     PINManager.switchPIN(user_input);

// //   } else if (user_op == "2" && PINManager.getPINUsed()) {

// //     Serial.println("Enter your actual PIN code:");

// //     String oldPIN = readSerial();

// //     Serial.println("Now, enter your new PIN code:");

// //     String newPIN = readSerial();

// //     // change PIN

// //     PINManager.changePIN(oldPIN, newPIN);

// //   } else {

// //     Serial.println("Incorrect option. Try again!.");

// //   }

// //   delay(1000);
// // }

// // /*

// //   Read input serial

// //  */

// // String readSerial() {

// //   String text = "";

// //   while (1) {

// //     while (Serial.available() > 0) {

// //       char inChar = Serial.read();

// //       if (inChar == '\n') {

// //         return text;

// //       }

// //       if (inChar != '\r') {

// //         text += inChar;

// //       }

// //     }

// //   }
// // }

