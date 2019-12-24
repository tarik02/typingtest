#ifndef TT_LOCALE_MESSAGES_H
#define TT_LOCALE_MESSAGES_H

#define MESSAGE_DECLARE(name) extern const char name[]; extern const int name ## _size;
#define MESSAGE_IMPLEMENT(name, value) const char name[] = value; const int name ## _size = sizeof(value) / sizeof(value[0]) - 1;

MESSAGE_DECLARE(msg_title);
MESSAGE_DECLARE(msg_seconds);
MESSAGE_DECLARE(msg_words_per_minute);
MESSAGE_DECLARE(msg_chars_per_minute);
MESSAGE_DECLARE(msg_percent_accuracy);
MESSAGE_DECLARE(msg_any_key_continue);
MESSAGE_DECLARE(msg_wait_seconds);

MESSAGE_DECLARE(msg_conc_1_message1);
MESSAGE_DECLARE(msg_conc_1_message2);
MESSAGE_DECLARE(msg_conc_1_message3);

MESSAGE_DECLARE(msg_conc_2_message1);
MESSAGE_DECLARE(msg_conc_2_message2);
MESSAGE_DECLARE(msg_conc_2_message3);

MESSAGE_DECLARE(msg_conc_3_message1);
MESSAGE_DECLARE(msg_conc_3_message2);
MESSAGE_DECLARE(msg_conc_3_message3);

MESSAGE_DECLARE(msg_conc_4_message1);
MESSAGE_DECLARE(msg_conc_4_message2);
MESSAGE_DECLARE(msg_conc_4_message3);

#endif
