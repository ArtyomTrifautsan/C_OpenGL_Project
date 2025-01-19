#include "event_handler.h"

#include <stdlib.h>

#include "input_keys.h"


EventHandler create_event_handler()
{
    EventHandler event_handler;

    int keyboard_size = 64;
    event_handler.keyboard_pressed_keys = create_array(keyboard_size);
    event_handler.keyboard_released_keys = create_array(keyboard_size);
    event_handler.keyboard_repeated_keys = create_array(keyboard_size);

    int mouse_size = 16;
    event_handler.mouse_pressed_keys = create_array(mouse_size);
    event_handler.mouse_released_keys = create_array(mouse_size);
    event_handler.mouse_repeated_keys = create_array(mouse_size);

    return event_handler;
}

void clear_event_storage(EventHandler *event_handler)
{
    array_clear(&event_handler->keyboard_pressed_keys);
    array_clear(&event_handler->keyboard_released_keys);
    array_clear(&event_handler->keyboard_repeated_keys);

    array_clear(&event_handler->mouse_pressed_keys);
    array_clear(&event_handler->mouse_released_keys);
    array_clear(&event_handler->mouse_repeated_keys);
}

void free_event_handler(EventHandler *event_handler)
{
    free_array(&event_handler->keyboard_pressed_keys);
    free_array(&event_handler->keyboard_released_keys);
    free_array(&event_handler->keyboard_repeated_keys);

    free_array(&event_handler->mouse_pressed_keys);
    free_array(&event_handler->mouse_released_keys);
    free_array(&event_handler->mouse_repeated_keys);
}

void push_to_keyboard_pressed(EventHandler *event_handler, KeyboardCode key)
{
    array_push_back(&event_handler->keyboard_pressed_keys, key);
}
void push_to_keyboard_released(EventHandler *event_handler, KeyboardCode key)
{
    array_push_back(&event_handler->keyboard_released_keys, key);
}

void push_to_keyboard_repeated(EventHandler *event_handler, KeyboardCode key)
{
    array_push_back(&event_handler->keyboard_repeated_keys, key);
}

void push_to_mouse_pressed(EventHandler *event_handler, KeyboardCode key)
{
    array_push_back(&event_handler->mouse_pressed_keys, key);
}

void push_to_mouse_released(EventHandler *event_handler, KeyboardCode key)
{
    array_push_back(&event_handler->mouse_released_keys, key);
}

void push_to_mouse_repeated(EventHandler *event_handler, KeyboardCode key)
{
    array_push_back(&event_handler->mouse_repeated_keys, key);
}

bool is_keyboard_key_pressed(EventHandler *event_handler, KeyboardCode key)
{
    return is_in_array(&event_handler->keyboard_pressed_keys, key);
}

bool is_keyboard_key_released(EventHandler *event_handler, KeyboardCode key)
{
    return is_in_array(&event_handler->keyboard_released_keys, key);
}

bool is_keyboard_key_repeated(EventHandler *event_handler, KeyboardCode key)
{
    return is_in_array(&event_handler->keyboard_repeated_keys, key);
}

bool is_mouse_key_pressed(EventHandler *event_handler, MouseCode key)
{
    return is_in_array(&event_handler->mouse_pressed_keys, key);
}

bool is_mouse_key_released(EventHandler *event_handler, MouseCode key)
{
    return is_in_array(&event_handler->mouse_released_keys, key);
}

bool is_mouse_key_repeated(EventHandler *event_handler, MouseCode key)
{
    return is_in_array(&event_handler->mouse_repeated_keys, key);
}
