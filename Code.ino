#define    command_high    4
#define    command_low     0
#define    data_high       5
#define    data_low        1
#define    slave_address   0x70



/* ---------------------------------------------------- */
/*                     I2C Functions                    */
/* ---------------------------------------------------- */

void i2c_init (void){
TWSR = 0X00; // set prescaler bits to zero
TWBR = 0x47; // SCL frequency is 50K for XTAL = 8Mhz
TWCR = 0x04; // enable TWI module
}

void i2c_start(void){
TWCR = ( 1 << TWINT ) | ( 1 << TWSTA ) | ( 1 << TWEN );
while ((TWCR & ( 1 << TWINT ) ) == 0) ;
}

void i2c_write(unsigned char data){
TWDR = data ;
TWCR = (1<<TWINT) | (1<<TWEN); // enable the transmission
while ((TWCR & ( 1 << TWINT ) ) == 0); // polling e.g checking TWINT flag to check whether START condition transmitted succesfully
}

void i2c_stop(){
TWCR = ( 1 <<TWINT )|( 1 << TWEN) | ( 1<< TWSTO);
}



/* ---------------------------------------------------- */
/*                     LCD Functions                    */
/* ---------------------------------------------------- */

void lcd_init(){
 send_command (0x02); // 4bit mode
//send_command (0x28); // Initialization of 16X2 LCD in 4bit mode
send_command (0x0E); // Display ON Cursor ON
send_command (0x06); // Auto Increment cursor
send_command (0x01); // Clear display
send_command (0x80); // Cursor at home position
}

void send_command(volatile uint8_t cmd){
volatile uint8_t high_nibble = cmd & 0xf0;
volatile uint8_t low_nibble = cmd << 4;
 i2c_start(); // transmit start
 i2c_write(slave_address); // transmitt Slave address + W0 (SLA+W means to master is writing )
 i2c_write(high_nibble + command_high);
 i2c_write(high_nibble + command_low);
 i2c_write(low_nibble + command_high);
i2c_write(low_nibble + command_low);
 i2c_stop(); // transmitt stop condition
 delayMicroseconds(500);
}

void send_data(volatile uint8_t data1){
volatile uint8_t high_nibble = data1 & 0xf0;
volatile uint8_t low_nibble = data1 << 4;
 i2c_start(); // transmit start
 i2c_write(slave_address); // transmitt Slave address + W0 (SLA+W means to master is writing )
 i2c_write(high_nibble + data_high);
 i2c_write(high_nibble + data_low);
 i2c_write(low_nibble + data_high);
i2c_write(low_nibble + data_low);
 i2c_stop(); // transmitt stop condition
 delayMicroseconds(50);
}

void lcd_string (char *str){ /* Send string to LCD function */
for(int i=0;str[i]!=0;i++){ /* Send each char of string till the NULL */
send_data (str[i]); /* Call LCD data write */
 }
}



/* ---------------------------------------------------- */
/*                     Main Functions                    */
/* ---------------------------------------------------- */

int main(void){
 i2c_init(); // initialise TWI for master mode
 lcd_init(); // initialise the 16*2 LCD
 lcd_string("Hello World!");

while(1);
return 0;
}