#ifndef _KEY_INPUT_H_
#define _KEY_INPUT_H_


typedef struct{
	Uint8 ArrowKeyRight;
	Uint8 ArrowKeyLeft;
	Uint8 ArrowKeyUp;
	Uint8 ArrowKeyDown;
	Uint8 Q;
	Uint8 W;
	Uint8 E;
	Uint8 R;
	Uint8 Spacebar;
	Uint8 Enter;
	Uint8 CTRL;
	Uint8 BackSpace;
	Uint8 MouseButtonUp;
	Uint8 MouseButtonDown;
	Uint8 MouseScrollWheelUp;
	Uint8 MouseScrollWheelDown;

void handleInput(int (*gameState)());

}KeyData;
void InitKeyData();
void CloseKeyData();
KeyData* GetKeyData();
void handleInput(int (*gameState)());
int StateGame();

#endif 