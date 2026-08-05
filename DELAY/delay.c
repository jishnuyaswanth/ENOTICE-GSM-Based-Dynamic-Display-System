//delay for micro seconds
void delay_us(unsigned int tdly)
{
        tdly*=12;
        while(tdly--);
}

//delay for milli seconds
void delay_ms(unsigned int tdly)
{
        tdly*=12000;
        while(tdly--);
}

//delay for seconds
void delay_s(unsigned int tdly)
{
        tdly*=12000000;

        while(tdly--);

}
