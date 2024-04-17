#pragma once


typedef enum {
    MSG_UPDATE_TARGET,
    MSG_PURCHASE_UNIT,
    // ... other user actions
    MSG_GAME_STATE_UPDATE,
    // other server updates
} MessageType;

typedef struct {
    MessageType type;
} MessageHeader;

typedef struct {
    MessageHeader header;
    int unitID;
    float targetX, targetY;
} UpdateTargetCommand;

typedef struct {
    MessageHeader header;
    int unitType;
    int quantity;
} PurchaseUnitMessage;

typedef struct {
    MessageHeader header;
    // Fields for game state updates
} GameStateUpdateMessage;
