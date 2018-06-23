#include "conclusions.h"
#include "messages.h"

conclusion_t tt_conclusions[] = {
	{
		.condition = 30,
		.name = "turtle",
		.art = {
			"                  ",
			"  _____     ____  ",
			" /      \\  |  o | ",
			"|        |/ ___\\| ",
			"|_________/       ",
			"|_|_| |_|_|       ",
		},
		.message1 = msg_conc_1_message1,
		.message2 = msg_conc_1_message2,
		.message3 = msg_conc_1_message3,
	},
	{
		.condition = 40,
		.name = "dino",
		.art = {
			"           _____  ",
			"          | o   | ",
			" _       /    __| ",
			"\\ \\     /   _/    ",
			"\\  \\__/    |\\     ",
			" \\        /       ",
		},
		.message1 = msg_conc_2_message1,
		.message2 = msg_conc_2_message2,
		.message3 = msg_conc_2_message3,
	},
	{
		.condition = 60,
		.name = "octopus",
		.art = {
			"       ",
			" ,---. ",
			"( @ @ )",
			" ).-.( ",
			"'/|||\\`",
			"  '|`  ",
		},
		.message1 = msg_conc_3_message1,
		.message2 = msg_conc_3_message2,
		.message3 = msg_conc_3_message3,
	},
	{
		.condition = -1,
		.name = "octopus",
		.art = {
			"       ",
			" ,---. ",
			"( @ @ )",
			" ).-.( ",
			"'/|||\\`",
			"  '|`  ",
		},
		.message1 = msg_conc_4_message1,
		.message2 = msg_conc_4_message2,
		.message3 = msg_conc_4_message3,
	},
};

const int tt_conclusions_count = sizeof(tt_conclusions) / sizeof(tt_conclusions[0]);
