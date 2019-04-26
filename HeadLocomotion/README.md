# Head and Locomotion

Controls both the head motor and locomotion on the same Arduino.

*For temporary use.*

## Properties

| Type | Request | Response |
| :---: | :--- | :--- |
| LM | 1 byte direction (1 forward, 0 backward), 1 byte speed (0 to 255, 50 deadzone) | None |
| RM | 1 byte direction (1 forward, 0 backward), 1 byte speed (0 to 255, 50 deadzone) | None |
| BM | LM then RM | None |
| HM | 1 byte direction, 2 byte big endian duration (milliseconds) | None |
| STOP | None | None |
