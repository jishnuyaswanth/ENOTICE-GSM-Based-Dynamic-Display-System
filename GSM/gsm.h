#include"types.h"
void gsm_init(void);
void send_sms(u8 *num, u8* info, u8 *msg);
void delete_sms(void);
void Read_sms(u8*sms);
void Check_gsm(void);
