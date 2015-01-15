/*
 * Pair Tag Highlighter
 *
 * highlights matching opening/closing HTML tags
 *
 * Author:  Volodymyr Kononenko aka kvm
 * Email:   vm@kononenko.ws
 *
 */

#include <geanyplugin.h>
#include "Scintilla.h"  // for the SCNotification struct
#include "SciLexer.h"

#define INDICATOR_TAGMATCH 0
#define MAX_TAG_NAME 64

/* These items are set by Geany before plugin_init() is called. */
GeanyPlugin     *geany_plugin;
GeanyData       *geany_data;
GeanyFunctions  *geany_functions;

ScintillaObject *sci;


PLUGIN_VERSION_CHECK(211)

PLUGIN_SET_INFO("Word Highlighter", "Finds and highlights matching words in editor", "1.0", 
                "Rishikesh Jha <techierishi@gmail.com>");


/* Searches matching word and.
 * highlight them all
 * 
 */

void dbgg(gchar* *gch){


FILE *f = fopen("/home/techierishi/dbgg.txt", "a");
if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}

fprintf(f, "Some text: %s\n", *gch);



fclose(f);


}

unsigned long createRGB(int r, int g, int b)
{
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}


void highlight_word(gint openingBracket, gint closingBracket)
{
    scintilla_send_message(sci, SCI_INDICSETSTYLE,
                            INDICATOR_TAGMATCH, INDIC_ROUNDBOX);
    scintilla_send_message(sci, SCI_INDICSETFORE,
                            0, createRGB(0, 208, 0)); // green #00d000
    scintilla_send_message(sci, SCI_INDICSETALPHA, INDICATOR_TAGMATCH, 60);
    scintilla_send_message(sci, SCI_INDICATORFILLRANGE,
                            openingBracket, closingBracket-openingBracket+1);
}





void run_word_highlighter(void)
{
    gint position = sci_get_current_position(sci);
    gint lineNumber = sci_get_current_line(sci);
    gint lineStart = sci_get_position_from_line(sci, lineNumber);
    gint lineEnd = sci_get_line_end_position(sci, lineNumber);

    gchar* gc  = sci_get_selection_contents(sci);

	dbgg(&gc);
    //highlight_word(openingBracket, closingBracket);

}


/* Notification handler for editor-notify */
static gboolean on_editor_notify(GObject *obj, GeanyEditor *editor,
                                SCNotification *nt, gpointer user_data)
{
    gint lexer;

    /* setting global sci variable to be available in other functions */
    sci = editor->sci;

    scintilla_send_message(sci, SCI_SETINDICATORCURRENT, INDICATOR_TAGMATCH, 0);

    lexer = sci_get_lexer(sci);
   

    /* nmhdr is a structure containing information about the event */
    switch (nt->nmhdr.code)
    {
        case SCN_UPDATEUI:
            run_word_highlighter();
            break;
    }

    /* returning FALSE to allow Geany processing the event */
    return FALSE;
}


PluginCallback plugin_callbacks[] =
{
    { "editor-notify", (GCallback) &on_editor_notify, FALSE, NULL },
    { NULL, NULL, FALSE, NULL }
};


void plugin_init(GeanyData *data)
{
}


void plugin_cleanup(void)
{
    
}
