//12-may-2021 9:16:00
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//

#include <GL/gl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#if (defined _WIN32 || defined _WIN64)
    #define OEMRESOURCE
    #define AM_WINDOWS
    #include <windows.h>
#else
    #define AM_LINUX
    #include <X11/Xatom.h>
    #include <X11/Xlib.h>
    #include <GL/glx.h>
#endif

//----------------------------------------------------------------------------//
//                                END INCLUDES                                //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                               TYPES AND DEFS                               //
//----------------------------------------------------------------------------//

#define am_malloc(size) malloc(size)
#define am_free(mem) free((void*)mem)
#define am_realloc(mem, size) realloc(mem, size)
#define am_calloc(mem, size) calloc(mem, size)

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int  int32;
typedef unsigned int  uint32;
typedef signed long int int64;
typedef unsigned long int uint64;
typedef float float32;
typedef double float64;
typedef enum {false, true} bool;
typedef enum {FAILURE, SUCCESS, IN_PROGRESS} am_result;

//----------------------------------------------------------------------------//
//                             END TYPES AND DEFS                             //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                               DYNAMIC  ARRAY                               //
//----------------------------------------------------------------------------//

typedef struct am_dyn_array {
    uint8 *data;
    uint32 length;
    uint32 alloc_length;
    uint32 element_size;
} am_dyn_array;

void am_dyn_array_init(am_dyn_array *array, uint32 element_size);
bool am_dyn_array_resize(am_dyn_array *array, uint32 newsize);
bool am_dyn_array_push(am_dyn_array *array, void *data, uint32 length);
bool am_dyn_array_insert(am_dyn_array *array, void *data, uint32 length, uint32 index);
bool am_dyn_array_pop(am_dyn_array *array, uint32 length);
bool am_dyn_array_remove(am_dyn_array *array, uint32 index, uint32 length);
void am_dyn_array_cleanup(am_dyn_array *array);

#define am_dyn_array_create(name, type) am_dyn_array name; am_dyn_array_init(&name, (uint32)sizeof(type))
#define am_dyn_array_data_length(array, length) ((array)->element_size * (length))
#define am_dyn_array_data_pos(array, index) ((array)->data + am_dyn_array_data_length((array), (index)))
#define am_dyn_array_data_retrieve(array, type, index) (((type*) (void*) (array)->data)[(index)])

//----------------------------------------------------------------------------//
//                             END DYNAMIC  ARRAY                             //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                                    MATH                                    //
//----------------------------------------------------------------------------//

typedef struct am_vec2i {
    int32 x;
    int32 y;
} am_vec2i;

//----------------------------------------------------------------------------//
//                                  END MATH                                  //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                                  PLATFORM                                  //
//----------------------------------------------------------------------------//

#define AM_WINDOW_ROOT_PARENT DefaultRootWindow(am_engine_get_subsystem(platform)->display)

typedef enum am_key_map {
    AM_KEYCODE_INVALID,
    AM_KEYCODE_ESCAPE,
    AM_KEYCODE_F1,
    AM_KEYCODE_F2,
    AM_KEYCODE_F3,
    AM_KEYCODE_F4,
    AM_KEYCODE_F5,
    AM_KEYCODE_F6,
    AM_KEYCODE_F7,
    AM_KEYCODE_F8,
    AM_KEYCODE_F9,
    AM_KEYCODE_F10,
    AM_KEYCODE_F11,
    AM_KEYCODE_F12,
    AM_KEYCODE_F13,
    AM_KEYCODE_F14,
    AM_KEYCODE_F15,
    AM_KEYCODE_F16,
    AM_KEYCODE_F17,
    AM_KEYCODE_F18,
    AM_KEYCODE_F19,
    AM_KEYCODE_F20,
    AM_KEYCODE_F21,
    AM_KEYCODE_F22,
    AM_KEYCODE_F23,
    AM_KEYCODE_F24,
    AM_KEYCODE_F25,
    AM_KEYCODE_PRINT_SCREEN,
    AM_KEYCODE_SCROLL_LOCK,
    AM_KEYCODE_PAUSE,
    AM_KEYCODE_ACCENT_GRAVE,
    AM_KEYCODE_1,
    AM_KEYCODE_2,
    AM_KEYCODE_3,
    AM_KEYCODE_4,
    AM_KEYCODE_5,
    AM_KEYCODE_6,
    AM_KEYCODE_7,
    AM_KEYCODE_8,
    AM_KEYCODE_9,
    AM_KEYCODE_0,
    AM_KEYCODE_MINUS,
    AM_KEYCODE_EQUAL,
    AM_KEYCODE_BACKSPACE,
    AM_KEYCODE_INSERT,
    AM_KEYCODE_HOME,
    AM_KEYCODE_PAGE_UP,
    AM_KEYCODE_NUMPAD_NUM,
    AM_KEYCODE_NUMPAD_DIVIDE,
    AM_KEYCODE_NUMPAD_MULTIPLY,
    AM_KEYCODE_NUMPAD_SUBTRACT,
    AM_KEYCODE_TAB,
    AM_KEYCODE_Q,
    AM_KEYCODE_W,
    AM_KEYCODE_E,
    AM_KEYCODE_R,
    AM_KEYCODE_T,
    AM_KEYCODE_Y,
    AM_KEYCODE_U,
    AM_KEYCODE_I,
    AM_KEYCODE_O,
    AM_KEYCODE_P,
    AM_KEYCODE_LEFT_SQUARE_BRACKET,
    AM_KEYCODE_RIGHT_SQUARE_BRACKET,
    AM_KEYCODE_BACKSLASH,
    AM_KEYCODE_DELETE,
    AM_KEYCODE_END,
    AM_KEYCODE_PAGE_DOWN,
    AM_KEYCODE_NUMPAD_7,
    AM_KEYCODE_NUMPAD_8,
    AM_KEYCODE_NUMPAD_9,
    AM_KEYCODE_CAPS_LOCK,
    AM_KEYCODE_A,
    AM_KEYCODE_S,
    AM_KEYCODE_D,
    AM_KEYCODE_F,
    AM_KEYCODE_G,
    AM_KEYCODE_H,
    AM_KEYCODE_J,
    AM_KEYCODE_K,
    AM_KEYCODE_L,
    AM_KEYCODE_SEMICOLON,
    AM_KEYCODE_APOSTROPHE,
    AM_KEYCODE_ENTER,
    AM_KEYCODE_NUMPAD_4,
    AM_KEYCODE_NUMPAD_5,
    AM_KEYCODE_NUMPAD_6,
    AM_KEYCODE_NUMPAD_ADD,
    AM_KEYCODE_LEFT_SHIFT,
    AM_KEYCODE_Z,
    AM_KEYCODE_X,
    AM_KEYCODE_C,
    AM_KEYCODE_V,
    AM_KEYCODE_B,
    AM_KEYCODE_N,
    AM_KEYCODE_M,
    AM_KEYCODE_COMMA,
    AM_KEYCODE_PERIOD,
    AM_KEYCODE_SLASH,
    AM_KEYCODE_RIGHT_SHIFT,
    AM_KEYCODE_UP_ARROW,
    AM_KEYCODE_NUMPAD_1,
    AM_KEYCODE_NUMPAD_2,
    AM_KEYCODE_NUMPAD_3,
    AM_KEYCODE_LEFT_CONTROL,
    AM_KEYCODE_LEFT_META,
    AM_KEYCODE_LEFT_ALT,
    AM_KEYCODE_SPACE, 
    AM_KEYCODE_RIGHT_ALT,
    AM_KEYCODE_RIGHT_META,
    AM_KEYCODE_MENU,
    AM_KEYCODE_RIGHT_CONTROL,
    AM_KEYCODE_LEFT_ARROW,
    AM_KEYCODE_DOWN_ARROW,
    AM_KEYCODE_RIGHT_ARROW, 
    AM_KEYCODE_NUMPAD_0,
    AM_KEYCODE_NUMPAD_DECIMAL,
    AM_KEYCODE_NUMPAD_EQUAL,
    AM_KEYCODE_NUMPAD_ENTER,
    AM_KEYCODE_COUNT
} am_key_map;

typedef enum am_mouse_map {
    AM_MOUSE_BUTTON_INVALID,
    AM_MOUSE_BUTTON_LEFT,
    AM_MOUSE_BUTTON_RIGHT,
    AM_MOUSE_BUTTON_MIDDLE,
    AM_MOUSE_BUTTON_COUNT
} am_mouse_map;

//TODO: Need window events as well, ex resize, focus, etc
typedef enum am_events {
    AM_EVENT_INVALID,
    AM_EVENT_KEY_PRESS,
    AM_EVENT_KEY_RELEASE,
    AM_EVENT_MOUSE_MOTION,
    AM_EVENT_MOUSE_BUTTON_PRESS,
    AM_EVENT_MOUSE_BUTTON_RELEASE,
    AM_EVENT_MOUSE_ENTER,
    AM_EVENT_MOUSE_LEAVE,
    AM_EVENT_MOUSE_SCROLL_UP,
    AM_EVENT_MOUSE_SCROLL_DOWN,
    AM_EVENT_WINDOW_MOTION,
    AM_EVENT_WINDOW_RESIZE,
    AM_EVENT_COUNT
} am_events;

typedef struct am_window_info {
    uint64 parent;
    const char* window_title;
    uint32 window_width;
    uint32 window_height; 
    am_vec2i window_position; 
    bool is_fullscreen; //Useless for child windows
    bool is_resizable;
} am_window_info;

//TODO: Linux only, implement windows functinality
typedef struct am_window {
    uint64 handle;
    int32 internal_id;
    am_window_info info;
} am_window;

typedef struct am_platform_callbacks {
    void (*am_platform_key_callback)(uint64, am_key_map, am_events);
    void (*am_platform_mouse_button_callback)(uint64, am_mouse_map, am_events);
    void (*am_platform_mouse_motion_callback)(uint64, int32, int32, am_events);
    void (*am_platform_mouse_scroll_callback)(uint64, am_events);
    void (*am_platform_window_motion_callback)(uint64, int32, int32, am_events);
    void (*am_platform_window_resize_callback)(uint64, int32, int32, am_events);
} am_platform_callbacks;

typedef struct am_platform_input {
    am_key_map keycodes[256]; //LUT
    bool keyboard_map[AM_KEYCODE_COUNT]; 
    bool prev_keyboard_map[AM_KEYCODE_COUNT];
    bool mouse_map[AM_MOUSE_BUTTON_COUNT];
    bool prev_mouse_map[AM_MOUSE_BUTTON_COUNT];
    int32 wheel_delta;
    am_vec2i mouse_position;
    bool mouse_moved;
} am_platform_input;

typedef struct am_platform_time {
    //TODO: Complete this as things come up
    uint64 elapsed;
} am_platform_time;

typedef struct am_platform {
    #if defined(AM_LINUX)
        Display *display;
    #endif
    am_dyn_array windows; //of am_window*
    am_platform_input input;
    am_platform_time time;
    am_platform_callbacks callbacks;
} am_platform;


//Platform fns
am_key_map am_platform_translate_keysym(KeySym *key_syms, int32 width);
am_mouse_map am_platform_translate_button(uint32 button);
am_platform *am_platform_create();
void am_platform_update(am_platform *platform);
void am_platform_terminate(am_platform *platform);

//Keyboard input
void am_platform_key_press(am_key_map key); 
void am_platform_key_release(am_key_map key); 
bool am_platform_key_pressed(am_key_map key); 
bool am_platform_key_down(am_key_map key);
bool am_platform_key_released(am_key_map key); 
bool am_platform_key_up(am_key_map key);

//Mouse input
void am_platform_mouse_button_press(am_mouse_map button); 
void am_platform_mouse_button_release(am_mouse_map button); 
bool am_platform_mouse_button_pressed(am_mouse_map button); 
bool am_platform_mouse_button_down(am_mouse_map button); 
bool am_platform_mouse_button_released(am_mouse_map button); 
bool am_platform_mouse_button_up(am_mouse_map button);
void am_platform_mouse_position(int32 *x, int32 *y); 
am_vec2i am_platform_mouse_position_vec();
int32 am_platform_mouse_wheel_delta();
bool am_platform_mouse_moved();

//Platform default callbacks
void am_platform_key_callback_default(uint64 window_handle, am_key_map key, am_events event);
void am_platform_mouse_button_callback_default(uint64 window_handle, am_mouse_map button, am_events event);
void am_platform_mouse_motion_callback_default(uint64 window_handle, int32 x, int32 y, am_events event);
void am_platform_mouse_scroll_callback_default(uint64 window_handle, am_events event);
void am_platform_window_motion_callback_default(uint64 window_handle, int32 x, int32 y, am_events event);
void am_platform_window_resize_callback_default(uint64 window_handle, int32 width, int32 height, am_events event);

#define am_platform_set_key_callback(platform, callback) platform->callbacks.am_platform_key_callback = callback
#define am_platform_set_mouse_button_callback(platform, callback) platform->callbacks.am_platform_mouse_button_callback = callback
#define am_platform_set_mouse_motion_callback(platform, callback) platform->callbacks.am_platform_mouse_motion_callback = callback
#define am_platform_set_mouse_scroll_callback(platform, callback) platform->callbacks.am_platform_mouse_scroll_callback = callback  
#define am_platform_set_window_motion_callback(platform, callback) platform->callbacks.am_platform_window_motion_callback = callback                           
#define am_platform_set_window_resize_callback(platform, callback) platform->callbacks.am_platform_window_resize_callback = callback  

//Window fns
am_window *am_platform_window_create(am_window_info window_info);
am_window *am_platform_window_lookup(uint64 handle); 
int32 am_platform_window_index_lookup(uint64 handle);
void am_platform_window_terminate(am_window *window);

//TODO:REMOVE; TEMPORARY
am_platform *test_platform;

//----------------------------------------------------------------------------//
//                                END PLATFORM                                //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                                   ENGINE                                   //
//----------------------------------------------------------------------------//

typedef struct am_engine_info {
    void (*init)();
    void (*update)();
    void (*shutdown)();
    const char* window_title;
    uint32 window_width;
    uint32 window_height; 
    bool is_running;
    bool vsync_enabled;
    bool fullscreen_enabled;
} am_engine_info;

typedef struct am_engine {
    am_engine_info engine_info;
    am_platform *platform;
    //am_audio audio; TODO: Implement
    //am_pfngl pfngl; TODO: Implement
} am_engine;

//TODO:Implement
am_engine *am_engine_get_instance();

//TODO:CHANGE THIS
//#define am_engine_get_subsystem(sys) am_engine_get_instance()->sys
#define am_engine_get_subsystem(sys) test_platform

//----------------------------------------------------------------------------//
//                                 END ENGINE                                 //
//----------------------------------------------------------------------------//

/*

ENGINE
   |
   - PLATFORM
   |    |
   |    - INPUT
   |    - TIME
   |    - WINDOW ARRAY
   |    - CALLBACKS
   |
   - AUDIO
   |
   - PFNGL* //Maybe not needed here
   |   |
   |   - Pointers to all OpenGL functions needed
   |
   - USER DATA POINTER
*/

//----------------------------------------------------------------------------//
//                             DYNAMIC  ARRAY IMPL                            //
//----------------------------------------------------------------------------//

void am_dyn_array_init(am_dyn_array *array, uint32 element_size) {
    array->data = NULL;
    array->length = 0;
    array->alloc_length = 0;
    array->element_size = element_size;
};

bool am_dyn_array_resize(am_dyn_array *array, uint32 new_length) {
    if (new_length > array->alloc_length) {
        void *buffer = NULL;
        buffer = realloc(array->data, (new_length + (array->alloc_length >> 1u))*am_dyn_array_data_length(array, 1));
        if (buffer) {
            array->alloc_length = new_length + (array->alloc_length >> 1u);
            array->data = (uint8*) buffer;
        } else return false;
    };
    return true;
};

bool am_dyn_array_push(am_dyn_array *array, void *data, uint32 length) {
    if (length >=0) { //Useless check, just keeping the same function type for prettiness
        am_dyn_array_resize(array, array->length + length);
        memcpy(am_dyn_array_data_pos(array, array->length), data, am_dyn_array_data_length(array, length));
        array->length += length;
        return true;
    };
    return false;
};

bool am_dyn_array_insert(am_dyn_array *array, void *data, uint32 length, uint32 index) {
    if (index <= array->length) {
        am_dyn_array_resize(array, array->length + length);
        memcpy(am_dyn_array_data_pos(array, index+length), am_dyn_array_data_pos(array, index), am_dyn_array_data_length(array, length));
        memcpy(am_dyn_array_data_pos(array, index), data, am_dyn_array_data_length(array, length));
        array->length += length;
        return true;
    };
    return false;
};

bool am_dyn_array_pop(am_dyn_array *array, uint32 length) {
    if (length <= array->length) {
        array->length -= length;
        return true;
    };
    return false;
};

bool am_dyn_array_remove(am_dyn_array *array, uint32 index, uint32 length) {
    if (index + length > array->length) return false;
    memcpy(am_dyn_array_data_pos(array, index), am_dyn_array_data_pos(array, index+length), am_dyn_array_data_length(array, array->length - length));
    array->length -= length;
    return true;
};

void am_dyn_array_cleanup(am_dyn_array *array) {
    array->length = 0;
    array->element_size = 0;
    array->alloc_length = 0;
    free((void*)array->data);
    array->data = NULL;
};

//----------------------------------------------------------------------------//
//                           END DYNAMIC  ARRAY IMPL                          //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                                PLATFORM IMPL                               //
//----------------------------------------------------------------------------//

am_key_map am_platform_translate_keysym(KeySym *key_syms, int32 width) {
    if (width > 1) {
        switch (key_syms[1]) {
            case XK_KP_0:           return AM_KEYCODE_NUMPAD_0;
            case XK_KP_1:           return AM_KEYCODE_NUMPAD_1;
            case XK_KP_2:           return AM_KEYCODE_NUMPAD_2;
            case XK_KP_3:           return AM_KEYCODE_NUMPAD_3;
            case XK_KP_4:           return AM_KEYCODE_NUMPAD_4;
            case XK_KP_5:           return AM_KEYCODE_NUMPAD_5;
            case XK_KP_6:           return AM_KEYCODE_NUMPAD_6;
            case XK_KP_7:           return AM_KEYCODE_NUMPAD_7;
            case XK_KP_8:           return AM_KEYCODE_NUMPAD_8;
            case XK_KP_9:           return AM_KEYCODE_NUMPAD_9;
            case XK_KP_Separator:
            case XK_KP_Decimal:     return AM_KEYCODE_NUMPAD_DECIMAL;
            case XK_KP_Equal:       return AM_KEYCODE_NUMPAD_EQUAL;
            case XK_KP_Enter:       return AM_KEYCODE_NUMPAD_ENTER;
            default:                break;
        };
    };

    switch (key_syms[0]) {
        case XK_Escape:         return AM_KEYCODE_ESCAPE;
        case XK_Tab:            return AM_KEYCODE_TAB;
        case XK_Shift_L:        return AM_KEYCODE_LEFT_SHIFT;
        case XK_Shift_R:        return AM_KEYCODE_RIGHT_SHIFT;
        case XK_Control_L:      return AM_KEYCODE_LEFT_CONTROL;
        case XK_Control_R:      return AM_KEYCODE_RIGHT_CONTROL;
        case XK_Alt_L:          return AM_KEYCODE_LEFT_ALT;
        case XK_Alt_R:          return AM_KEYCODE_RIGHT_ALT;
        case XK_Super_L:        return AM_KEYCODE_LEFT_META;
        case XK_Super_R:        return AM_KEYCODE_RIGHT_META;
        case XK_Menu:           return AM_KEYCODE_MENU;
        case XK_Num_Lock:       return AM_KEYCODE_NUMPAD_NUM;
        case XK_Caps_Lock:      return AM_KEYCODE_CAPS_LOCK;
        case XK_Print:          return AM_KEYCODE_PRINT_SCREEN;
        case XK_Scroll_Lock:    return AM_KEYCODE_SCROLL_LOCK;
        case XK_Pause:          return AM_KEYCODE_PAUSE;
        case XK_Delete:         return AM_KEYCODE_DELETE;
        case XK_BackSpace:      return AM_KEYCODE_BACKSPACE;
        case XK_Return:         return AM_KEYCODE_ENTER;
        case XK_Home:           return AM_KEYCODE_HOME;
        case XK_End:            return AM_KEYCODE_END;
        case XK_Page_Up:        return AM_KEYCODE_PAGE_UP;
        case XK_Page_Down:      return AM_KEYCODE_PAGE_DOWN;
        case XK_Insert:         return AM_KEYCODE_INSERT;
        case XK_Left:           return AM_KEYCODE_LEFT_ARROW;
        case XK_Right:          return AM_KEYCODE_RIGHT_ARROW;
        case XK_Down:           return AM_KEYCODE_DOWN_ARROW;
        case XK_Up:             return AM_KEYCODE_UP_ARROW;
        case XK_F1:             return AM_KEYCODE_F1;
        case XK_F2:             return AM_KEYCODE_F2;
        case XK_F3:             return AM_KEYCODE_F3;
        case XK_F4:             return AM_KEYCODE_F4;
        case XK_F5:             return AM_KEYCODE_F5;
        case XK_F6:             return AM_KEYCODE_F6;
        case XK_F7:             return AM_KEYCODE_F7;
        case XK_F8:             return AM_KEYCODE_F8;
        case XK_F9:             return AM_KEYCODE_F9;
        case XK_F10:            return AM_KEYCODE_F10;
        case XK_F11:            return AM_KEYCODE_F11;
        case XK_F12:            return AM_KEYCODE_F12;
        case XK_F13:            return AM_KEYCODE_F13;
        case XK_F14:            return AM_KEYCODE_F14;
        case XK_F15:            return AM_KEYCODE_F15;
        case XK_F16:            return AM_KEYCODE_F16;
        case XK_F17:            return AM_KEYCODE_F17;
        case XK_F18:            return AM_KEYCODE_F18;
        case XK_F19:            return AM_KEYCODE_F19;
        case XK_F20:            return AM_KEYCODE_F20;
        case XK_F21:            return AM_KEYCODE_F21;
        case XK_F22:            return AM_KEYCODE_F22;
        case XK_F23:            return AM_KEYCODE_F23;
        case XK_F24:            return AM_KEYCODE_F24;
        case XK_F25:            return AM_KEYCODE_F25;
        case XK_KP_Divide:      return AM_KEYCODE_NUMPAD_DIVIDE;
        case XK_KP_Multiply:    return AM_KEYCODE_NUMPAD_MULTIPLY;
        case XK_KP_Subtract:    return AM_KEYCODE_NUMPAD_SUBTRACT;
        case XK_KP_Add:         return AM_KEYCODE_NUMPAD_ADD;
        case XK_KP_Insert:      return AM_KEYCODE_NUMPAD_0;
        case XK_KP_End:         return AM_KEYCODE_NUMPAD_1;
        case XK_KP_Down:        return AM_KEYCODE_NUMPAD_2;
        case XK_KP_Page_Down:   return AM_KEYCODE_NUMPAD_3;
        case XK_KP_Left:        return AM_KEYCODE_NUMPAD_4;
        case XK_KP_Right:       return AM_KEYCODE_NUMPAD_6;
        case XK_KP_Home:        return AM_KEYCODE_NUMPAD_7;
        case XK_KP_Up:          return AM_KEYCODE_NUMPAD_8;
        case XK_KP_Page_Up:     return AM_KEYCODE_NUMPAD_9;
        case XK_KP_Delete:      return AM_KEYCODE_NUMPAD_DECIMAL;
        case XK_KP_Equal:       return AM_KEYCODE_NUMPAD_EQUAL;
        case XK_KP_Enter:       return AM_KEYCODE_NUMPAD_ENTER;
        case XK_a:              return AM_KEYCODE_A;
        case XK_b:              return AM_KEYCODE_B;
        case XK_c:              return AM_KEYCODE_C;
        case XK_d:              return AM_KEYCODE_D;
        case XK_e:              return AM_KEYCODE_E;
        case XK_f:              return AM_KEYCODE_F;
        case XK_g:              return AM_KEYCODE_G;
        case XK_h:              return AM_KEYCODE_H;
        case XK_i:              return AM_KEYCODE_I;
        case XK_j:              return AM_KEYCODE_J;
        case XK_k:              return AM_KEYCODE_K;
        case XK_l:              return AM_KEYCODE_L;
        case XK_m:              return AM_KEYCODE_M;
        case XK_n:              return AM_KEYCODE_N;
        case XK_o:              return AM_KEYCODE_O;
        case XK_p:              return AM_KEYCODE_P;
        case XK_q:              return AM_KEYCODE_Q;
        case XK_r:              return AM_KEYCODE_R;
        case XK_s:              return AM_KEYCODE_S;
        case XK_t:              return AM_KEYCODE_T;
        case XK_u:              return AM_KEYCODE_U;
        case XK_v:              return AM_KEYCODE_V;
        case XK_w:              return AM_KEYCODE_W;
        case XK_x:              return AM_KEYCODE_X;
        case XK_y:              return AM_KEYCODE_Y;
        case XK_z:              return AM_KEYCODE_Z;
        case XK_1:              return AM_KEYCODE_1;
        case XK_2:              return AM_KEYCODE_2;
        case XK_3:              return AM_KEYCODE_3;
        case XK_4:              return AM_KEYCODE_4;
        case XK_5:              return AM_KEYCODE_5;
        case XK_6:              return AM_KEYCODE_6;
        case XK_7:              return AM_KEYCODE_7;
        case XK_8:              return AM_KEYCODE_8;
        case XK_9:              return AM_KEYCODE_9;
        case XK_0:              return AM_KEYCODE_0;
        case XK_space:          return AM_KEYCODE_SPACE;
        case XK_minus:          return AM_KEYCODE_MINUS;
        case XK_equal:          return AM_KEYCODE_EQUAL;
        case XK_bracketleft:    return AM_KEYCODE_LEFT_SQUARE_BRACKET;
        case XK_bracketright:   return AM_KEYCODE_RIGHT_SQUARE_BRACKET;
        case XK_backslash:      return AM_KEYCODE_BACKSLASH;
        case XK_semicolon:      return AM_KEYCODE_SEMICOLON;
        case XK_apostrophe:     return AM_KEYCODE_APOSTROPHE;
        case XK_grave:          return AM_KEYCODE_ACCENT_GRAVE;
        case XK_comma:          return AM_KEYCODE_COMMA;
        case XK_period:         return AM_KEYCODE_PERIOD;
        case XK_slash:          return AM_KEYCODE_SLASH;
        default:                return AM_KEYCODE_INVALID;
    };
};

am_mouse_map am_platform_translate_button(uint32 button) {
    switch (button) {
        case 1: return AM_MOUSE_BUTTON_LEFT;
        case 2: return AM_MOUSE_BUTTON_MIDDLE;
        case 3: return AM_MOUSE_BUTTON_RIGHT;
    };
    return AM_MOUSE_BUTTON_INVALID;
};

//TODO:Init time related stuff when there's more
am_platform *am_platform_create() {
    am_platform *platform = (am_platform*)am_malloc(sizeof(am_platform));
    assert(platform != NULL);
    am_dyn_array_init(&platform->windows, sizeof(am_window*));
    
    platform->display = XOpenDisplay(NULL);
    memset(platform->input.keycodes, -1, sizeof(platform->input.keycodes));
    int32 min, max;
    XDisplayKeycodes(platform->display, &min, &max);
    int32 width;
    KeySym *key_syms = XGetKeyboardMapping(platform->display, min, max - min + 1, &width);
    for (int32 i = min; i < max; i++)  platform->input.keycodes[i] = am_platform_translate_keysym(&key_syms[(i-min)*width], width);
    XFree(key_syms);

    platform->input.wheel_delta = 0;
    am_vec2i temp = {0,0};
    platform->input.mouse_position = temp;

    memset(platform->input.keyboard_map, 0, sizeof(platform->input.keyboard_map));
    memset(platform->input.prev_keyboard_map, 0, sizeof(platform->input.prev_keyboard_map));
    memset(platform->input.mouse_map, 0, sizeof(platform->input.mouse_map));
    memset(platform->input.mouse_map, 0, sizeof(platform->input.prev_mouse_map));

    //TODO:Init more time related stuff if needed
    platform->time.elapsed = 0;

    am_platform_set_key_callback(platform, am_platform_key_callback_default);
    am_platform_set_mouse_button_callback(platform, am_platform_mouse_button_callback_default);
    am_platform_set_mouse_motion_callback(platform, am_platform_mouse_motion_callback_default);
    am_platform_set_mouse_scroll_callback(platform, am_platform_mouse_scroll_callback_default);
    am_platform_set_window_motion_callback(platform, am_platform_window_motion_callback_default);
    am_platform_set_window_resize_callback(platform, am_platform_window_resize_callback_default);
    return platform;
};

//TODO:update time
void am_platform_update(am_platform *platform) {
    //TODO:Update time
    memcpy(platform->input.prev_mouse_map, platform->input.mouse_map, sizeof(platform->input.mouse_map));
    //for (int i = 0; i < AM_MOUSE_BUTTON_COUNT; i++) platform->input.prev_mouse_map[i] = platform->input.mouse_map[i];
    memcpy(platform->input.prev_keyboard_map, platform->input.keyboard_map, sizeof(platform->input.keyboard_map));
    //for (int i = 0; i < AM_KEYCODE_COUNT; i++) platform->input.prev_keyboard_map[i] = platform->input.keyboard_map[i];
    platform->input.wheel_delta = 0;
    platform->input.mouse_moved = false;

    XEvent xevent;
    while (XPending(platform->display)) {
        XNextEvent(platform->display, &xevent);
        am_window* current_window = am_platform_window_lookup(xevent.xany.window);
        switch (xevent.type) {
            case KeyPress: {
                am_key_map key = platform->input.keycodes[xevent.xkey.keycode];
                platform->callbacks.am_platform_key_callback(xevent.xany.window, key, AM_EVENT_KEY_PRESS);  
                break;
            };
            case KeyRelease: {
                am_key_map key = platform->input.keycodes[xevent.xkey.keycode];
                platform->callbacks.am_platform_key_callback(xevent.xany.window, key, AM_EVENT_KEY_RELEASE);  
                break;
            };
            case ButtonPress: {
                am_mouse_map button = am_platform_translate_button(xevent.xbutton.button);
                if (button == AM_MOUSE_BUTTON_INVALID) {
                    if (xevent.xbutton.button == 4) {
                        platform->callbacks.am_platform_mouse_scroll_callback(xevent.xany.window, AM_EVENT_MOUSE_SCROLL_UP);
                        break;
                    };
                    if (xevent.xbutton.button == 5) {
                        platform->callbacks.am_platform_mouse_scroll_callback(xevent.xany.window, AM_EVENT_MOUSE_SCROLL_DOWN);
                        break;
                    };
                };
                platform->callbacks.am_platform_mouse_button_callback(xevent.xany.window, button, AM_EVENT_MOUSE_BUTTON_PRESS);
                break;
            };
            case ButtonRelease: {
                am_mouse_map button = am_platform_translate_button(xevent.xbutton.button);
                platform->callbacks.am_platform_mouse_button_callback(xevent.xany.window, button, AM_EVENT_MOUSE_BUTTON_RELEASE);
                break;
            };
            case MotionNotify: {
                platform->callbacks.am_platform_mouse_motion_callback(xevent.xany.window, xevent.xbutton.x, xevent.xbutton.y, AM_EVENT_MOUSE_MOTION);
                break;
            };
            case ConfigureNotify: {
                if (xevent.xconfigure.width != current_window->info.window_width || xevent.xconfigure.height != current_window->info.window_height) {
                    platform->callbacks.am_platform_window_resize_callback(xevent.xany.window, xevent.xconfigure.width, xevent.xconfigure.height, AM_EVENT_WINDOW_RESIZE);
                };
                if (xevent.xconfigure.x != current_window->info.window_position.x || xevent.xconfigure.y != current_window->info.window_position.y) {
                    platform->callbacks.am_platform_window_motion_callback(xevent.xany.window, xevent.xconfigure.x, xevent.xconfigure.y, AM_EVENT_WINDOW_MOTION);
                };
                break;
            };
            case ClientMessage: {
                if (xevent.xclient.data.l[0] = XInternAtom(platform->display, "WM_DELETE_WINDOW", false)) {
                    XUnmapWindow(platform->display, xevent.xany.window);
                    XDestroyWindow(platform->display, xevent.xany.window);
                };
                break;
            };
        };
    };
};

void am_platform_terminate(am_platform *platform) {
    am_free(platform->windows.data);
    am_free(platform);
};

void am_platform_key_press(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return;
    am_platform *platform = am_engine_get_subsystem(platform);
    platform->input.keyboard_map[key] = true;
};

void am_platform_key_release(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return;
    am_platform *platform = am_engine_get_subsystem(platform);
    platform->input.keyboard_map[key] = false; 
}; 

bool am_platform_key_pressed(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.keyboard_map[key] && !platform->input.prev_keyboard_map[key]; 
}; 

bool am_platform_key_down(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.keyboard_map[key] && platform->input.prev_keyboard_map[key];
}; 

bool am_platform_key_released(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return !platform->input.keyboard_map[key] && platform->input.prev_keyboard_map[key]; 
}; 

bool am_platform_key_up(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return !platform->input.keyboard_map[key];
};

void am_platform_mouse_button_press(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return;
    am_platform *platform = am_engine_get_subsystem(platform);
    platform->input.mouse_map[button] = true;
}; 

void am_platform_mouse_button_release(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return;
    am_platform *platform = am_engine_get_subsystem(platform);
    platform->input.mouse_map[button] = false;

};

bool am_platform_mouse_button_pressed(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.mouse_map[button] && !platform->input.prev_mouse_map[button];
};

bool am_platform_mouse_button_down(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.mouse_map[button] && platform->input.prev_mouse_map[button];
};

bool am_platform_mouse_button_released(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return !platform->input.mouse_map[button] && platform->input.prev_mouse_map[button];
};

bool am_platform_mouse_button_up(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return !platform->input.mouse_map[button];  
};

void am_platform_mouse_position(int32 *x, int32 *y) {
    am_platform *platform = am_engine_get_subsystem(platform);
    *x = platform->input.mouse_position.x;
    *y = platform->input.mouse_position.y;
};

am_vec2i am_platform_mouse_position_vec() {
    am_platform *platform = am_engine_get_subsystem(platform);
    am_vec2i position;
    position.x = platform->input.mouse_position.x;
    position.y = platform->input.mouse_position.y;
    return position;
}; 

int32 am_platform_mouse_wheel_delta() {
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.wheel_delta;
};

bool am_platform_mouse_moved() {
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.mouse_moved;
};

void am_platform_key_callback_default(uint64 window_handle, am_key_map key, am_events event) {
    am_platform *platform = am_engine_get_subsystem(platform);
    switch (event) {
        case AM_EVENT_KEY_PRESS: {
            platform->input.keyboard_map[key] = true;
            break;
        };
        case AM_EVENT_KEY_RELEASE: {
            platform->input.keyboard_map[key] = false;
            break;
        };
    };
};

void am_platform_mouse_button_callback_default(uint64 window_handle, am_mouse_map button, am_events event) {
    am_platform *platform = am_engine_get_subsystem(platform);
    switch (event) {
        case AM_EVENT_MOUSE_BUTTON_PRESS: {
            platform->input.mouse_map[button] = true;
            break;
        };
        case AM_EVENT_MOUSE_BUTTON_RELEASE: {
            platform->input.mouse_map[button] = false;
            break;
        };
    };
};

void am_platform_mouse_motion_callback_default(uint64 window_handle, int32 x, int32 y, am_events event) {
    am_platform *platform = am_engine_get_subsystem(platform);
    platform->input.mouse_moved = true;
    platform->input.mouse_position.x = x;
    platform->input.mouse_position.y = y;
};

void am_platform_mouse_scroll_callback_default(uint64 window_handle, am_events event) {
    am_platform *platform = am_engine_get_subsystem(platform);
    switch (event) {
        case AM_EVENT_MOUSE_SCROLL_UP: {
            ++platform->input.wheel_delta;
            break;
        };
        case AM_EVENT_MOUSE_SCROLL_DOWN: {
            --platform->input.wheel_delta;
            break;
        };
    };
};

void am_platform_window_motion_callback_default(uint64 window_handle, int32 x, int32 y, am_events event) {
    am_platform *platform = am_engine_get_subsystem(platofrm);
    am_window *window = am_platform_window_lookup(window_handle);
    window->info.window_position.x = x;
    window->info.window_position.y = y;
};

void am_platform_window_resize_callback_default(uint64 window_handle, int32 width, int32 height, am_events event) {
    am_platform *platform = am_engine_get_subsystem(platofrm);
    am_window *window = am_platform_window_lookup(window_handle);
    window->info.window_height = height;
    window->info.window_width = width; 
};

//TODO: Freeing colormap breaks the window
am_window *am_platform_window_create(am_window_info window_info) {
    am_window *new_window = (am_window*)am_malloc(sizeof(am_window)); 
    assert(new_window != NULL);
    new_window->info = window_info;
    am_platform *platform = am_engine_get_subsystem(platform);
    XSetWindowAttributes window_attributes;
    int32 attribs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    XVisualInfo *visual_info = glXChooseVisual(platform->display, 0, attribs);
    Colormap color_map = XCreateColormap(platform->display, window_info.parent, visual_info->visual, AllocNone);
    window_attributes.colormap = color_map;
    window_attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask;
    uint64 window = (uint64)XCreateWindow(platform->display, window_info.parent, window_info.window_position.x, window_info.window_position.y, window_info.window_width, window_info.window_height, 0, visual_info->depth, InputOutput, visual_info->visual, CWColormap | CWEventMask, &window_attributes);
    assert(window != BadAlloc && window != BadColor && window != BadCursor && window != BadMatch && window != BadPixmap && window != BadValue && window != BadWindow);
    new_window->handle = window;
    
    Atom wm_delete = XInternAtom(platform->display, "WM_DELETE_WINDOW", true);
    XSetWMProtocols(platform->display, (Window)new_window->handle, &wm_delete, 1);
    XStoreName(platform->display, (Window)new_window->handle, new_window->info.window_title);
    XMapWindow(platform->display, (Window)new_window->handle);
    XFree(visual_info);
    //TODO:FREEING THIS BREAKS THE WINDOW
    //XFreeColormap(platform->display, color_map);

    if (new_window->info.is_fullscreen && new_window->info.parent == AM_WINDOW_ROOT_PARENT) {
        Atom wm_state = XInternAtom(platform->display, "_NET_WM_STATE", false);
        Atom wm_fs = XInternAtom(platform->display, "_NET_WM_STATE_FULLSCREEN", false);
        XEvent xevent = {0};
        xevent.type = ClientMessage;
        xevent.xclient.window = new_window->handle;
        xevent.xclient.message_type = wm_state;
        xevent.xclient.format = 32;
        xevent.xclient.data.l[0] = true;
        xevent.xclient.data.l[1] = wm_fs;
        xevent.xclient.data.l[2] = 0;
        xevent.xclient.data.l[3] = 0;
        XSendEvent(platform->display, new_window->info.parent, false, SubstructureRedirectMask | SubstructureNotifyMask, &xevent);
        XFlush(platform->display);
    };

    new_window->internal_id = (int32)platform->windows.length;
    am_dyn_array_push(&platform->windows, &new_window, 1);
    return new_window;
};

am_window *am_platform_window_lookup(uint64 handle) {
    am_platform *platform = am_engine_get_subsystem(platform);
    for (int32 i = 0; i < platform->windows.length; i++)
        if (am_dyn_array_data_retrieve(&platform->windows, am_window*, i)->handle == handle) 
            return am_dyn_array_data_retrieve(&platform->windows, am_window*, i);
    return NULL;
};

int32 am_platform_window_index_lookup(uint64 handle) {
    am_platform *platform = am_engine_get_subsystem(platform);
    for (int32 i = 0; i < platform->windows.length; i++)
        if (am_dyn_array_data_retrieve(&platform->windows, am_window*, i)->handle == handle)
            return i;
    return -1;
};

void am_platform_window_terminate(am_window *window) {
    am_platform *platform = am_engine_get_subsystem(platform);
    XEvent xevent;
    xevent.xclient.type = ClientMessage;
    xevent.xclient.window = (Window)window->handle;
    xevent.xclient.message_type = XInternAtom(platform->display, "WM_PROTOCOLS", true);
    xevent.xclient.format = 32;
    xevent.xclient.data.l[0] = XInternAtom(platform->display, "WM_DELETE_WINDOW", false);
    xevent.xclient.data.l[1] = CurrentTime;
    XSendEvent(platform->display, window->handle, false, NoEventMask, &xevent);

    am_dyn_array_remove(&platform->windows, am_platform_window_index_lookup(window->handle), 1);  
};

//----------------------------------------------------------------------------//
//                              END PLATFORM IMPL                             //
//----------------------------------------------------------------------------//

int main() {
    test_platform = am_platform_create(); 

    printf("pfcr\n");
    am_window_info test = {
        .window_height = 500,
        .window_width = 400,
        .window_title = "Win1",
        .window_position = {0,0},
        .parent = AM_WINDOW_ROOT_PARENT,
        .is_fullscreen = false
    };
    am_window *wind = am_platform_window_create(test);
    XSetWindowBackground(test_platform->display, wind->handle, 0x00FF00);

    am_window_info test2 = {
        .window_height = 100,
        .window_width = 100,
        .window_title = "Win2",
        .window_position = {50,70},
        .parent = wind->handle,
        .is_fullscreen = false
    };
    am_window *wind2 = am_platform_window_create(test2);
    XSetWindowBackground(test_platform->display, wind2->handle, 0xFF0000);

    am_window_info test3 = {
        .window_height = 10,
        .window_width = 10,
        .window_title = "Win3",
        .window_position = {3,3},
        .parent = wind2->handle,
        .is_fullscreen = false
    };
    am_window *wind3 = am_platform_window_create(test3);
    XSetWindowBackground(test_platform->display, wind3->handle, 0x0000FF);

    XEvent event;
    uint64 t = 0;
    int32 mx, my;
    while (true) {
        t++;
        am_platform_update(test_platform);
        if (am_platform_key_pressed(AM_KEYCODE_SPACE)) am_platform_window_terminate(wind3);
        if (am_platform_key_down(AM_KEYCODE_W)) printf("w %d\n",t);
        if (am_platform_mouse_moved()) {
            am_platform_mouse_position(&mx, &my);
            printf("Mouse pos: %d %d\n", mx, my);
        };
        if (am_platform_mouse_wheel_delta() != 0) printf("Scrolled\n");
    };

    am_platform_terminate(test_platform);
    return 0;
}