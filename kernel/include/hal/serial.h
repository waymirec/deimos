#ifndef DEIMOS_SERIAL_H
#define DEIMOS_SERIAL_H

#define SERIAL_COM1_BASE                    0x3F8
#define SERIAL_COM2_BASE                    0x2F8

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_line(unsigned short com);
int serial_is_transmit_empty(unsigned int com);
void serial_write(unsigned int com, char chr);
int serial_received(unsigned int com);
char serial_read(unsigned int com);

#endif //DEIMOS_SERIAL_H
