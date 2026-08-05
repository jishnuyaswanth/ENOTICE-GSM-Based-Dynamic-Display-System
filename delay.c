// Generates an approximate delay in microseconds
void delay_us(unsigned int tdly)
{
    tdly *= 12;            // Convert the delay count into CPU instruction cycles (assuming 12 MHz clock)

    while(tdly--);         // Busy-wait loop until the delay counter reaches zero
}

// Generates an approximate delay in milliseconds
void delay_ms(unsigned int tdly)
{
    tdly *= 12000;         // Convert milliseconds into CPU instruction cycles

    while(tdly--);         // Busy-wait loop to generate the required millisecond delay
}

// Generates an approximate delay in seconds
void delay_s(unsigned int tdly)
{
    tdly *= 12000000;      // Convert seconds into CPU instruction cycles

    while(tdly--);         // Busy-wait loop until the required second delay is completed
}