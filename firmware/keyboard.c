#include "keyboard.h"

#include "event.h"

static uint8 _keyboard_status = 0x00;

void
keyboard_init(void)
{
	KEYBOARD_DDR = 0x00;
	event_add_polling_fct(keyboard_poll);
	_keyboard_status = KEYBOARD_PORT & KEYBOARD_PORT_MASK;
}

void
keyboard_poll(void)
{
	if( (KEYBOARD_PORT & KEYBOARD_PORT_MASK) != _keyboard_status ) {
		event_t event = {
			.code = E_NONE,
			.data = 0x00
		};
		for(uint8 i=0; i<8; i++) {
			uint8 current_key_mask = (1 << i);
			if( KEYBOARD_PORT_MASK & current_key_mask ) {
				if( (KEYBOARD_PORT & current_key_mask) && !(_keyboard_status & current_key_mask) ) {
					event.code = E_KEY_PRESSED;
					event.data = current_key_mask;
					event_push(event);
				}
				if( !(KEYBOARD_PORT & current_key_mask) && (_keyboard_status & current_key_mask) ) {
					event.code = E_KEY_PRESSED;
					event.data = current_key_mask;
					event_push(event);
				}
			}
		}
		_keyboard_status = KEYBOARD_PORT & KEYBOARD_PORT_MASK;
	}
}