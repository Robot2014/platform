#include "cmd_line.h"
#define CMD_HEAD  "<CMD_LINE>#"


cmd_tbl_s cmd_line[] =
{
	{
		tftp,3,1, do_tftp,
		"download file via network using TFTP protocol \n",
		"[loadAddress] [loadfilename] \n"
	},
	{
	
	},


};


int main( char *arg_v[] , char arg_c)
{
	
	while(1)
	{
		get_command();
		run_command();
	}

	return 1;
}