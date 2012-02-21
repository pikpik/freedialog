#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dialog.h"

#define TRUE	1
#define FALSE	0

int      clear_d = 0;	               /* Should we clear screen? */

unsigned char    *rcfile = NULL;       /* Create an rc file? */

char    *hline_d = NULL;	       /* Help line */
char    *hfile = NULL;	               /* Help file */
unsigned char    *title = NULL;	       /* Title for dialog box */

int      height = -1;                  /* Height of dialog box */
int      width = -1;                   /* Width of dialog box */

extern char *__progname;

struct option long_options[] =
{
	{CLEAR,     no_argument,       NULL, CLEAR_OPT},
	{CREATE_RC, required_argument, NULL, CREATE_RC_OPT},
	{TITLE,     required_argument, NULL, TITLE_OPT},
	{HLINE,     required_argument, NULL, HLINE_OPT},
	{HFILE,     required_argument, NULL, HFILE_OPT},
	{YESNO,     no_argument,       NULL, YESNO_OPT},
	{MSGBOX,    no_argument,       NULL, MSGBOX_OPT},
	{INFOBOX,   no_argument,       NULL, INFOBOX_OPT},
	{INPUTBOX,  no_argument,       NULL, INPUTBOX_OPT},
	{TEXTBOX,   no_argument,       NULL, TEXTBOX_OPT},
	{MENU,      no_argument,       NULL, MENU_OPT},
	{PRGBOX,    no_argument,       NULL, PRGBOX_OPT},
	{CHECKLIST, no_argument,       NULL, CHECKLIST_OPT},
	{RADIOLIST, no_argument,       NULL, RADIOLIST_OPT},
	{FTREE,     no_argument,       NULL, FTREE_OPT},
	{TREE,      no_argument,       NULL, TREE_OPT},

	{NULL,      no_argument,       NULL, 0}
};

void usage(void);

int 
main(argc, argv)
	int    argc;
	char **argv;
{
	char   result[MAX_INPUT], *result2;
	char  *i, *j;
	int    ch, cmd, r;
	
	result[0] = 0;
	init_dialog();
	
	cmd = r = 0;
	while ((ch = getopt_long(argc, argv, "", long_options, (int *)NULL)) != -1) {
		switch (ch) {
		case CLEAR_OPT:
			clear_d = TRUE;
			break;
		case CREATE_RC_OPT:
			rcfile = (unsigned char *)optarg;
			break;
		case TITLE_OPT:
			title = (unsigned char *)optarg;
			break;
		case HLINE_OPT:
			hline_d = optarg;
			break;
		case HFILE_OPT:
			hfile = optarg;
			break;
		case YESNO_OPT:
		case MSGBOX_OPT:
		case INFOBOX_OPT:
		case INPUTBOX_OPT:
		case TEXTBOX_OPT:
		case MENU_OPT:
		case PRGBOX_OPT:
		case CHECKLIST_OPT:
		case RADIOLIST_OPT:
		case FTREE_OPT:
		case TREE_OPT:
			cmd = ch;
			goto breakout;
			break;
		default:
			usage();
		}
	}
	
breakout:
	argc -= optind;
	argv += optind;
	
	/* Ndialog doesn't support this yet. */

	/*if (rcfile != NULL) {
		dialog_create_rc(rcfile);
		return 0;
	}*/
	
	/* use_helpfile(hfile); */ /* Not in ndialog. */
	use_helpline(hline_d);
	
	switch (cmd) {
	case YESNO_OPT:
		if (argc != 3)
			usage();
		r = dialog_yesno(title, (unsigned char *)argv[0], 
				 strtol(argv[1], (char **)NULL, 10), 
				 strtol(argv[2], (char **)NULL, 10));
		break;
	case MSGBOX_OPT:
		if (argc != 3)
			usage();
		r = dialog_msgbox(title, (unsigned char *)argv[0],
				  strtol(argv[1], (char **)NULL, 10), 
				  strtol(argv[2], (char **)NULL, 10), 1);
		break;
	case INFOBOX_OPT:
		if (argc != 3)
			usage();
		r = dialog_msgbox(title, (unsigned char *)argv[0],
				  strtol(argv[1], (char **)NULL, 10), 
				  strtol(argv[2], (char **)NULL, 10), 0);
		break;
	case INPUTBOX_OPT:
		if (argc != 3 && argc != 4)
			usage();
		if (argc == 4)
			strncpy(result, argv[3], MAX_INPUT);
		r = dialog_inputbox(title,(unsigned char *) argv[0],
				    strtol(argv[1], (char **)NULL, 10), 
				    strtol(argv[2], (char **)NULL, 10), 
				    (unsigned char *)result);
		break;
	case TEXTBOX_OPT:
		if (argc != 3)
			usage();
		/*
		r = dialog_textbox(title, (unsigned char *)argv[0], 
				   strtol(argv[1], (char **)NULL, 10), 
				   strtol(argv[2], (char **)NULL, 10));
		*/	/* Not in ndialog. */
		break;
	case MENU_OPT:
		if ((argc % 2) != 0 || argc < 6)
			usage();
		r = dialog_menu(title, (unsigned char *)argv[0],
				strtol(argv[1], (char **)NULL, 10),
				strtol(argv[2], (char **)NULL, 10),
				strtol(argv[3], (char **)NULL, 10), (argc - 4) / 2,
				&argv[4], (unsigned char *)result, NULL, NULL);
		break;
	case PRGBOX_OPT:
		if (argc != 3)
			usage();
		/*
		r = dialog_prgbox(title, (unsigned char *)argv[0],
				  strtol(argv[1], (char **)NULL, 10),
				  strtol(argv[2], (char **)NULL, 10),
				  TRUE, TRUE);
		*/	/* Not in ndialog. */
		break;
	case CHECKLIST_OPT:
		if ((argc % 3) != 1 || argc < 7)
			usage();
		r = dialog_checklist(title, (unsigned char *)argv[0],
				     strtol(argv[1], (char **)NULL, 10),
				     strtol(argv[2], (char **)NULL, 10),
				     strtol(argv[3], (char **)NULL, 10), 
				     (argc - 4) / 3, &argv[4], 
				     (unsigned char *)result);
		break;
	case RADIOLIST_OPT:
		if ((argc % 3) != 1 || argc < 7)
			usage();
		r = dialog_radiolist(title, (unsigned char *)argv[0],
				     strtol(argv[1], (char **)NULL, 10),
				     strtol(argv[2], (char **)NULL, 10),
				     strtol(argv[3], (char **)NULL, 10), 
				     (argc - 4) / 3, &argv[4], 
				     (unsigned char *)result);
		break;
	case FTREE_OPT:
		if (argc < 6)
			usage();
		/*
		r = dialog_ftree((unsigned char *)argv[0], *argv[1], 
				 title, (unsigned char *) argv[2],
				 strtol(argv[3], (char **)NULL, 10),
				 strtol(argv[4], (char **)NULL, 10),
				 strtol(argv[5], (char **)NULL, 10),
				 (unsigned char **)&result2);
		*/	/* Not in ndialog. */
		break;
	case TREE_OPT:
		if (argc < 6)
			usage();
		/*
		r = dialog_tree((unsigned char **)&argv[5], argc - 5, 
				*argv[0], title, 
				(unsigned char *)argv[1],
				strtol(argv[2], (char **)NULL, 10),
				strtol(argv[3], (char **)NULL, 10),
				strtol(argv[4], (char **)NULL, 10),
				(unsigned char **)&result2);
		*/	/* Not in ndialog. */
		break;
	default:
		usage();
	}
	dialog_update();
	
	
	switch (cmd) {
	case CHECKLIST_OPT:
		j = result - 1;
		while ((i = index(++j, '\n')) != NULL) {
			*i = 0;
			fprintf(stderr, "\"%s\"%s", j, index(&i[1], '\n') == NULL ? "" : " ");
			j = i;
		}
		break;
	case INPUTBOX_OPT:
		if (r == FALSE)
			fprintf(stderr, "%s\n", result);
		break;
	case FTREE_OPT:
	case TREE_OPT:
		if (r == FALSE)
			fprintf(stderr, "%s\n", result2);
		break;
	default:
		if (result[0] != 0) 
			fprintf(stderr, "%s\n", result);
		break;
	}
	
	if (clear_d == TRUE) {
		dialog_clear();
		dialog_update();
	}
	
	end_dialog();
	
	return r;
	
}

void
usage(void)
{
	
	end_dialog();
	fprintf(stderr, "\
usage: %s --clear\n\
       %s [--clear] [--hfile file] [--hline line] [--title title] --[dialog] options ...\n\
       %s --create-rc\n", __progname, __progname, __progname);
	exit(2);
}
