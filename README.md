# Stop-and-Wait Reliable Protocol (RDT 2.5)

This project implements a reliable data transfer protocol using Stop-and-Wait (RDT 2.5) over UDP sockets in C. It simulates an unreliable network and handles packet loss with timeout-based retransmissions.

## Files
- `sender.c` - Implements the sender FSM.
- `receiver.c` - Implements the receiver FSM.

## How to Run
1. Compile both files using `gcc`.
2. Run `receiver` first, then `sender`.

## Future Work
- Add support for packet corruption
- Implement GBN/SR or extend to TCP

