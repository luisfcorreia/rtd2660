/*
this code was shamelessly stolen from https://gist.github.com/kraftb/8c0bbcc35a9778608d74
*/

// I2C to USB Adapter using Arduino
// by Bernhard Kraft <kraftb@think-open.at>

/**
 * This sketch can get loaded onto an Arduino to use it as USB to I2C Adapter.
 * It uses the Wire library. So take a look at the documentation of the Wire
 * libarary about the pins being used as SDA/SCL. For most Arduino boards this
 * will be analog input pin 4 for SDA and analog input pin 5 for SCL.
 *
 * On the USB side the default serial link of the Arduino is used. A protocol
 * is defined which allows to perform xfer operations on the I2C bus. The
 * protocol defines messages which get sent from the PC to the Arduino and
 * which get answered by a reply from the Arduino. So the communication is
 * bounded in blocks.
 *
 * It is clear that there needs to be a way to synchronize the PC and the
 * Arduino. For this purpose the <ESC> character (0x1B, .27) is used as RESET
 * character. Whenever it is received by the Arduino it goes into a defined
 * state.
 *
 * Having only this convention would disallow the <ESC> character in payload
 * data. So another character is used to quote/escape the <ESC> character. This
 * character is the "\" character (0x5C, .92). To send a literal <ESC> value
 * the sequence "0x5C 0xB1" has to get sent (0xB1 is the <ESC> character 0x1B
 * with its nibbles exchanged). To send a literal "\" character the sequence
 * "0x5C 0xC5" has to get sent. For all other combinations of "0x5C" with
 * another character an error is generated (except for "0x5C 0x1B" which is an
 * error being signaled after the "\").
 *
 * When an error is encountered the Arduino sends an <ESC> character (0x1B, not
 * the escape sequence) and stops further processing of serial data until it
 * gets reset by a <ESC> character from the PC.
 *
 * The exception mentioned above for the sequence "0x5C 0x1B" results from
 * the fact, that 0x1B is the RESET character. So it is possible to cause
 * a RESET when a quote character has already been sent.
 */

#include <Wire.h>

/**
 * These function signatures are necessary so the file can get compiled
 * with the commandline arduino-mk package (apt-get install arduino-mk)
 */
void handleReceivedData(byte data);
void escapeSendData(byte data);
void initAdapter();
void handleCommand(byte command);
void escapeSendData(byte data);
void handleReceivedSequence(byte data);
void handleData(byte data);
void handleWireRead();
void handleIdent();

/**
 * Can't access error register of TWI/Wire library. Thus no errors
 * can get recognized for Wire.requestFrom()
 */
// extern uint8_t twi_error;


#define CMD_I2C_ADDRESS				'A'
#define CMD_I2C_LENGTH				'L'
#define CMD_I2C_WRITE_RESTART		'w'
#define CMD_I2C_WRITE				'W'
#define CMD_I2C_READ_RESTART		'r'
#define CMD_I2C_READ					'R'
#define CMD_GET_STATE				'S'
#define CMD_GET_ERROR				'E'
#define CMD_GET_IDENT				'I'
#define CMD_GET_ADDRESS				'a'
#define CMD_GET_LENGTH				'l'

#define STATE_INIT		0x00
#define STATE_ERROR		0x01
#define STATE_ADDRESS	0x02
#define STATE_LENGTH		0x03
#define STATE_WRITE		0x05

#define CHAR_RESET		0x1B		// It is somehow misleading that <ESC> is used for RESET
#define CHAR_ESCAPE		0x5C		// And "\" is the escape character.

#define CHAR_ESCAPED_RESET		0xB1
#define CHAR_ESCAPED_ESCAPE	0xC5

#define ERROR_NONE				'N'
#define ERROR_UNESCAPE			'U'
#define ERROR_LENGTH				'L'
#define ERROR_READ				'R'
#define ERROR_WRITEDATA			'W'
#define ERROR_SENDDATA			'S'


byte state = STATE_INIT;
byte address = 0;
byte length = 0;
byte error = 0;
boolean restart = false;
char data = 0;
boolean escape = false;

String ident = "Arduino I2C-to-USB 1.0"; 

void setup() {
	// initialize the serial communication:
	Serial.begin(9600);
        Wire.begin();
	initAdapter();
}

void initAdapter() {
	// End an eventually ongoing transmission
	Wire.endTransmission();
	
	state = STATE_INIT;
	address = 0;
	length = 0;
	error = ERROR_NONE;
	restart = false;
	data = 0;
	escape = false;
}

void loop() {
	while (!Serial) {
		// wait for serial port to connect. Needed for Leonardo only
		// The state will be "INIT" upon connecting the serial.
		initAdapter();
	}

	if (Serial.available()) {

		if (state == STATE_ERROR) {
			// Signal the PC an error
			Serial.write(CHAR_RESET);
		}

		// Read data from serial port
		data = Serial.read();

		if (data == CHAR_RESET) {
			// When the RESET character has been received cause a reset
			initAdapter();
		} else {
			// Every other character gets passed to "handleReceivedData"
			// which will take care about unescaping.
			handleReceivedSequence(data);
		}

		if (state == STATE_ERROR) {
			// Signal the PC an error
			Serial.write(CHAR_RESET);
		}
	}
}


/**
 * This function handles a passed data byte according to the current state
 *
 * @param byte data: The received data byte
 * @return void;
 */
void handleData(byte data) {
	if (state == STATE_INIT) {
		// The first received byte designates the command
		handleCommand(data);
	} else if (state == STATE_ADDRESS) {
		// In state ADDRESS the passed byte denotes the address upon
		// which further commands will act.
		address = data;
		state = STATE_INIT;
	} else if (state == STATE_LENGTH) {
		// The LENGTH command defines the number of bytes which
		// should get read/written
		if (data > BUFFER_LENGTH) {
			state = STATE_ERROR;
			error = ERROR_LENGTH;
		} else {
			length = data;
			state = STATE_INIT;
		}
	} else if (state == STATE_WRITE) {
		// When in WRITE state the passed value is a data byte which should
		// get sent. Pass on as many bytes as specified by a previous LENGTH
		// command. Then send it out on the I2C port.
		if (length) {
			if (Wire.write(data) == 0) {
				state = STATE_ERROR;
				error = ERROR_WRITEDATA;
				return;
			}
			length--;
		}
		if (length == 0) {
			if (Wire.endTransmission(restart ? false : true) != 0) {
				state = STATE_ERROR;
				error = ERROR_SENDDATA;
				return;
			}
			restart = false;
			state = STATE_INIT;
		}
	}
}

/**
 * This function handles a passed command
 *
 * @param byte command: The command which should get handled
 * @return void
 */
void handleCommand(byte command) {
	switch (command) {

		case CMD_I2C_ADDRESS:
			state = STATE_ADDRESS;
		break;

		case CMD_I2C_LENGTH:
			state = STATE_LENGTH;
		break;

		case CMD_I2C_WRITE_RESTART:
			restart = true;
		case CMD_I2C_WRITE:
			Wire.beginTransmission(address);
			state = STATE_WRITE;
		break;

		case CMD_I2C_READ_RESTART:
			restart = true;
		case CMD_I2C_READ:
			handleWireRead();
		break;

		case CMD_GET_ADDRESS:
			escapeSendData(address);
		break;

		case CMD_GET_LENGTH:
			escapeSendData(length);
		break;

		case CMD_GET_STATE:
			escapeSendData(state);
		break;

		case CMD_GET_ERROR:
			escapeSendData(error);
		break;

		case CMD_GET_IDENT:
			handleIdent();
		break;

	}
}

void handleIdent() {
	int len = ident.length();
	char buf[len+1];
	ident.toCharArray(buf, len+1);
	escapeSendData(len);
	// We can use "Serial.write" here because we know the IDENT string
	// doesn't contain any characters which would have to get escaped.
	Serial.write((uint8_t*)buf, len);
}

void handleWireRead() {
	Wire.requestFrom((uint8_t)address, (uint8_t)length, (uint8_t)(restart ? 0 : 1));
	restart = false;

	byte a = Wire.available();
	escapeSendData(a);
	if (a != 0) {
		byte r = 0;
		for (byte i = 0; i < a; i++) {
			r = Wire.read();
			escapeSendData(r);
		}
	}
	if (Wire.available() != 0) {
		state = STATE_ERROR;
	} else {
		state = STATE_INIT;
	}
}

/**
 * This function handles the plain received data bytes.
 * If it receives the <ESC> character it resets the state machine to INIT state.
 * It handles the "\" escape sequence and calls "handleData" for the unescaped
 * data having been received.
 *
 * @param byte data: The received data byte
 * @return void
 */
void handleReceivedSequence(byte data) {    
	if (escape) {
		escape = false;
		switch (data) {
			case CHAR_ESCAPED_ESCAPE:  // Will cause a "\" (ESCAPE) to get added to the buffer
				handleData(CHAR_ESCAPE);
			break;

			case CHAR_ESCAPED_RESET:  // Will cause a <ESC> (RESET) to get added to the buffer
				handleData(CHAR_RESET);
			break;

			default:
				// Every other character causes an error while being in an escape sequence
				state = STATE_ERROR;
				error = ERROR_UNESCAPE;
			break;
		}
	} else {
		if (data == CHAR_ESCAPE) {
			escape = true;
		} else {
			handleData(data);
		}
	}
}

/**
 * This function sends the passed byte. It escapes special characters.
 *
 * @param byte data: The data byte which should get sent.
 * @return void
 */
void escapeSendData(byte data) {
	if (data == CHAR_ESCAPE) {
		Serial.write(CHAR_ESCAPE);
		Serial.write(CHAR_ESCAPED_ESCAPE);
	} else if (data == CHAR_RESET) {
		Serial.write(CHAR_ESCAPE);
		Serial.write(CHAR_ESCAPED_RESET);
	} else {
		Serial.write(data);
	}
}
