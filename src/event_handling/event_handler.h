#pragma once

#include "input_keys.h"
#include "../data_structures/queue/queue.h"
#include "../data_structures/array/array.h"
#include <stdbool.h>


typedef struct {
    Array keyboard_pressed_keys;
    Array keyboard_released_keys;
    Array keyboard_repeated_keys;

    Array mouse_pressed_keys;
    Array mouse_released_keys;
    Array mouse_repeated_keys;
} EventHandler;


EventHandler create_event_handler();
void clear_event_storage(EventHandler *event_handler);
void free_event_handler(EventHandler *event_handler);

void push_to_keyboard_pressed(EventHandler *event_handler, KeyboardCode key);
void push_to_keyboard_released(EventHandler *event_handler, KeyboardCode key);
void push_to_keyboard_repeated(EventHandler *event_handler, KeyboardCode key);

void push_to_mouse_pressed(EventHandler *event_handler, KeyboardCode key);
void push_to_mouse_released(EventHandler *event_handler, KeyboardCode key);
void push_to_mouse_repeated(EventHandler *event_handler, KeyboardCode key);

bool is_keyboard_key_pressed(EventHandler *event_handler, KeyboardCode key);
bool is_keyboard_key_released(EventHandler *event_handler, KeyboardCode key);
bool is_keyboard_key_repeated(EventHandler *event_handler, KeyboardCode key);

bool is_mouse_key_pressed(EventHandler *event_handler, MouseCode key);
bool is_mouse_key_released(EventHandler *event_handler, MouseCode key);
bool is_mouse_key_repeated(EventHandler *event_handler, MouseCode key);
