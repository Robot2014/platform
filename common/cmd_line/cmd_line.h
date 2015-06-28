#ifndef CMD_LINE_H
#define CMD_LINE_H

#define CFG_MAXARGS 16
#define CFG_CBSIZE   256  /* Console I/O Buffer Size	*/

typedef ytustruct cmd_tbl_s {
	char	*name;			/* Command Name			*/
	int		maxargs;		/* maximum number of arguments	*/
	int		repeatable;		/* autorepeat allowed?		*/
							/* Implementation function	*/
	int		(*cmd)(struct cmd_tbl_s *, int, int, char *[]);
	char	*usage;			/* Usage message	(short)	*/
#ifdef	CFG_LONGHELP
	char		*help;		/* Help  message	(long)	*/
#endif
#ifdef CONFIG_AUTO_COMPLETE
							/* do auto completion on the arguments */
	int		(*complete)(int argc, char *argv[], char last_char, int maxv, char *cmdv[]);
#endif
}cmd_tbl_t;

int run_command (const char *cmd, int flag);

#endif