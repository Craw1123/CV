#define KEYPAD_PORT PORTB                    // Output Data and set default input (pull-up pull-down)
#define KEYPAD_DDR DDRB                      // Data Direction Register
#define KEYPAD_PIN PINB                      // Input Data
#define UART_Init uart_init                  // UART1_Init
#define UART_Write uart_send                 // UART1_Write // For unsigned char like 0xF0 0x0E... etc
#define UART_Write_Text uart_send_str        // UART1_Write_Text // For raw text like "column found"
#define BAUD 19200                           // BAUDRATE
#define F_CPU 10000000                       // CPU FREQUENCY
#define BAUDRATE ((F_CPU) / (BAUD * 16) - 1) // UART BAUD RATE CALC
#define endl "\r\n"                          // NEWLINE
char number_1_char[7];
char number_2_char[7];
long number_1_long;
long number_2_long;
void uart_init(void)
{
    UBRR0H = (unsigned char)(BAUDRATE >> 8); // IF BAUDRATE IS LONGER THAN 8 BITS SHIFTS
    UBRR0L = (unsigned char)BAUDRATE;        // SET BAUDRATE

    //         RXC         -   Receive Complete Flag
    //         |TXC        -   Transmit Complete Flag
    //         ||UDRE      -   USART Data Register Empty(UDR ready to receive new data)
    //         |||FE       -   Frame Error(no need)
    //         ||||DOR     -   Data Overrun Error(no need)
    //         |||||PE     -   Parity Error(no need)
    //         ||||||U2X   -   Double Transmission Speed(no need)
    //         |||||||MPCM -   Multi-Processor Communication Mode(no need)
    UCSR0A = 0b00000000;

    //         RXCIE       -   RX Complete Interrupt Enable(no need) // maybe need
    //         |TXCIE      -   TX Complete Interrupt Enable(no need)
    //         ||UDRIE     -   USART Data Register Empty Interrupt Enable(no need)
    //         |||RXEN     -   Receiver Enable
    //         ||||TXEN    -   Transmitter Enable
    //         |||||UCSZ2  -   Character Size
    //         ||||||RXB8  -   Receive Data Bit 8(no need)
    //         |||||||TXB8 -   Transmit Data Bit 8(no need)
    UCSR0B = 0b00011000;

    //         URSEL       -   USART Register Select(UBRRH/UCSRC)
    //         |UMSEL      -   USART Mode Select(sync=1, async=0)
    //         ||UPM1      -   UPM1:0 – Parity Mode
    //         |||UPM0     -   UPM1:0 – Parity Mode
    //         ||||USBS    -   Stop Bit Select (0 - 1, 1 - 2)
    //         |||||UCSZ1  -   Character Size
    //         ||||||UCSZ0 -   Character Size (UCSZ2:UCSZ0 = 011 = 8 bit)
    //         |||||||UCPOL-   Clock Polarity for sync
    UCSR0C = 0b10000110;

    // UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void uart_send(unsigned char data_input)
{
    while (!(UCSR0A) & (1 << UDRE0)) // CHECK FOR UDRE in UCSRA to become 1
        ;
    UDR0 = data_input; // SEND DATA
}

void uart_send_str(char *string_input)
{
    while (*string_input != '\0')
    {
        uart_send(*string_input);
        delay_ms(1);
        string_input++;
    }
}

unsigned char uart_receive(void)
{
    while (!(UCSR0A & (1 << RXC0))) // CHECK FOR RXC in UCSRA to become 1
        ;
    return UDR0;
}

int keypad_input(void)
{

    int keypad_int[4][4] = {1, 2, 3, -2,
                            4, 5, 6, -2,
                            7, -2, -2, -2,
                            -1, 0, -3, -2};

    unsigned char col;
    unsigned char row;
    unsigned char temp;
    int col_number;
    int row_number;

    col = 0x0F;
    row = 0xF0;
    do
    {
        // default state 1111 0000 (output/input)
        // default input 0000 1111
        // if pressed    0000 1101 <- this is what we want
        KEYPAD_DDR = 0xF0;  // first 4(1) as output, last 4(0) as input
        KEYPAD_PORT = 0x0F; // making default values of input HIGH (pull-up mode)
        // output always stays LOW
        delay_ms(50);
        col = KEYPAD_PIN & 0x0F; // if button pressed some input bits will become 0 through output
    } while (col == 0x0F);
    switch (col)
    {
    case 0x0E: // 1110 14
        col_number = 0;
        break;
    case 0x0D: // 1101 13
        col_number = 1;
        break;
    case 0x0B: // 1011 11
        col_number = 2;
        break;
    case 0x07: // 0111 7
        col_number = 3;
        break;
    }
    /*
    col_number = 0;
    temp = 0x0F - col;
    while (temp >= 2)
    {
          temp = temp / 2;
          col_number++;
    }
    */
    

    // default state 0000 1111 (input/output)
    // default input 1111 0000
    // if pressed    1101 0000 <- this is what we want
    KEYPAD_DDR = 0x0F;  // first 4(0) as input, last 4(1) as output
    KEYPAD_PORT = 0xF0; // making default values of input HIGH (pull-up mode)
    // output always stays LOW
    delay_ms(50);
    row = KEYPAD_PIN & 0xF0; // if button pressed some input bits will become 0 through output

    switch (row)
    {
    case 0xE0: // 1110
        row_number = 0;
        break;
    case 0xD0: // 1101
        row_number = 1;
        break;
    case 0xB0: // 1011
        row_number = 2;
        break;
    case 0x70: // 0111
        row_number = 3;
        break;
    }

    return keypad_int[row_number][col_number];
}

char get_char(int number)
{
     char res = number + '0';
     return res;
}

int get_int(char number)
{
    return number - '0';
}

void long_to_char(long number, char input_string[7])
{
    int j = 0;
    int k = 0;
    char buffer_char[7];
    if (number==0){
      input_string[0] = '0';
      input_string[1] = '\0';
    }
    else {
    while (number > 0)
    {
        int num = number % 10;
        buffer_char[j] = get_char(num);
        number = number / 10;
        j++;
    }

    for (k = j - 1; k >= 0; k--)
        input_string[j - k - 1] = buffer_char[k];

    input_string[j] = '\0';
    }
}

long keyboard_input_6(void)
{
    int i = 0;
    long number = 0;
    char number_char[7];

    UART_Write_Text("Enter first number using keyboard 4x4: ");
    while (1)
    {
        int buffer = keypad_input();

        if (buffer == -1) // *
        {
            if (i == 0)
            {
                UART_Write_Text(endl);
                UART_Write_Text("No number entered");
                UART_Write_Text(endl);
                UART_Write_Text("Enter first number using keyboard 4x4: ");
                delay_ms(1000);
            }
            else
            {
                break;
            }
        }
        else if (buffer==-3){
            i = 0;
            number = 0;
            UART_Write_Text(endl);
            UART_Write_Text("Number reset");
            UART_Write_Text(endl);
            UART_Write_Text("Enter first number using keyboard 4x4: ");
            delay_ms(1000);
        }
        else if (buffer == -2) // ABCD89#
        {
            UART_Write_Text(endl);
            UART_Write_Text("Incorrent symbol, try again: ");
            if (i != 0)
            {
                long_to_char(number, number_char);
                UART_Write_Text(number_char);
            }
            delay_ms(1000);
        }

        else
        {
            if (i == 6)
            {
                UART_Write_Text(endl);
                UART_Write_Text("Number limit reached, waiting for *: ");
                long_to_char(number, number_char);
                UART_Write_Text(number_char);
                delay_ms(1000);
            }
            else
            {
                number = number * 10 + buffer;
                UART_Write(get_char(buffer));
                delay_ms(1000);
                i++;
            }
        }
    }
    UART_Write_Text(endl);
    return number;
}

long uart_input_6(void)
{
    int i = 0;
    long number = 0;
    char number_char[7];

    UART_Write_Text("Enter second number using PC keyboard: ");
    while (1)
    {
        unsigned char buffer = uart_receive();

        if (buffer == '*')
        {
            if (i == 0)
            {
                UART_Write_Text(endl);
                UART_Write_Text("No number entered");
                UART_Write_Text(endl);
                UART_Write_Text("Enter second number using PC keyboard: ");
            }
            else
            {
                break;
            }
        }
        else if (buffer == '#'){
            i = 0;
            number = 0;
            UART_Write_Text(endl);
            UART_Write_Text("Number reset");
            UART_Write_Text(endl);
            UART_Write_Text("Enter second number using PC keyboard: ");
        }
        else if (!(buffer >= '0' && buffer <= '7'))
        {
            UART_Write_Text(endl);
            UART_Write_Text("Incorrent symbol, try again: ");
            if (i != 0)
            {
                long_to_char(number, number_char);
                UART_Write_Text(number_char);
            }
        }

        else
        {
            if (i == 6)
            {
                UART_Write_Text(endl);
                UART_Write_Text("Number limit reached, waiting for *: ");
                long_to_char(number, number_char);
                UART_Write_Text(number_char);
            }
            else
            {
                number = number * 10 + get_int(buffer);
                UART_Write(buffer);
                i++;
            }
        }
    }
    UART_Write_Text(endl);
    return number;
}

void main()
{
    UART_Init();
    while (1)
    {
        number_1_long = keyboard_input_6();
        long_to_char(number_1_long, number_1_char);
        number_2_long = uart_input_6();
        long_to_char(number_2_long, number_2_char);

        UART_Write_Text(number_1_char);
        if (number_1_long > number_2_long)
        {
            UART_Write_Text(" > ");

        }
        else if (number_1_long < number_2_long)
        {
            UART_Write_Text(" < ");
        }
        else if (number_1_long == number_2_long)
        {
            UART_Write_Text(" = ");
        }
        UART_Write_Text(number_2_char);

        UART_Write_Text(endl);
    }
}