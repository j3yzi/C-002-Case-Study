#include "../headers/state.h"
#include <stdio.h>

// A centralized array to hold all application states.
static bool app_states[APP_STATE_COUNT];

/**
 * @brief Initializes the state manager, setting all states to their default values (false).
 */
void stateManagerInit(void) {
    for (int i = 0; i < APP_STATE_COUNT; i++) {
        app_states[i] = false;
    }
}

/**
 * @brief Sets the boolean value for a given state key.
 * @param key The state to modify (from the AppStateKey enum).
 * @param value The new boolean value (true or false).
 */
void stateManagerSetBool(AppStateKey key, bool value) {
    if (key >= 0 && key < APP_STATE_COUNT) {
        app_states[key] = value;
    } else {
        fprintf(stderr, "Error: Invalid AppStateKey used in stateManagerSetBool.\\n");
    }
}

/**
 * @brief Gets the boolean value for a given state key.
 * @param key The state to retrieve (from the AppStateKey enum).
 * @return The current boolean value of the state, or false if the key is invalid.
 */
bool stateManagerGetBool(AppStateKey key) {
    if (key >= 0 && key < APP_STATE_COUNT) {
        return app_states[key];
    }
    
    fprintf(stderr, "Error: Invalid AppStateKey used in stateManagerGetBool.\\n");
    return false;
}
