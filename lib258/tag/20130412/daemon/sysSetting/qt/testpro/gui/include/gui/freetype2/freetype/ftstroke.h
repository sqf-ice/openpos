r
           and enables this mode, respectively.  See also the the
         Toggles between several modes of displaying postscript specials:
       Display specials, display specials with their bounding box
             (if available), and display bounding boxes only (if available).
        The prefix arguments 1, 2 and 0 also allow you to select one of
     these states directly.
            Toggles expert mode (in which           the page list and the statusline do not appear).
       `1x' toggles the display of the statusline at the bottom of the window.
        `2x' toggles the scrollbars,
           `3x' toggles the page list and menu buttons.
           Makes the display of the page larger (zooms in).
       Makes the display of the page smaller (zooms out).
             Makes the fonts darker (by adding to the gamma value).
         Makes the fonts lighter (by subtracting from the gamma
    Whenever the mouse is positioned on a link, the cursor changes
 to a `hand' shape and the target of the link is displayed
      in the statusline at the bottom of the window.
 The following keybindings are pre-configured:
  	Follow the link at the cursor position.
       	If the link target is not a DVI file, try to launch
   	an application to view the file.
      	Open a new xdvi window displaying the link
    	at the cursor position if the link is a DVI file;
     	else, try to launch an application to view the file.
  	Go back to the previous hyperlink in the history.
     	Go forward to the next hyperlink in the history.
      By default, the hyperlinks are displayed in the colors 
        `linkColor' and `visitedLinkColor' (for visited links) and 
    underlined in the same colors. This can be customized 
 by setting the resource or command-line option `linkstyle' 
    to a value between 0 and 3, which have the following meaning:
      0: no highlighting of links,
           3: color and underline links.

     Mouse Buttons	Actions bound to the mouse buttons
       The mouse buttons can be customized just like the keys;
        however the bindings cannot be intermixed (since
       a mouse event always requires the cursor location
      to be present, which a key event doesn't).
     The default bindings are as follows:

       Pops up magnifier windows of different sizes.
          When the mouse is over a hyperlink, the link overrides
         the magnifier. In that case, Button 1 jumps to the link
        in the current xdvi window, Button 2 opens the link target
             in a new instance of xdvi.
             In `Ruler Mode', Button1 shows/drags the ruler instead;
        in `Text Selection Mode', Button1 can be used to select
        a rectangular region of text from the DVI file.
   Shift-Button1 to Shift-Button3
      Drag the page in each direction (Button 1), vertically
         only (Button 2) or horizontally only (Button 3).
       Invoke a reverse search for the text on the cursor
             location (see the section SOURCE SPECIALS for more
        The buttons 4 and 5 (wheel up and down for wheel mice)
 scroll the page up and down respectively, or jump to the
       next/previous page when the mouse is over the page list.        In the page list, Button 2 toggles the mark a page (see
        section `Marking Pages'); moving the mouse while holding
       Button 2 lets you toggle a range of pages.
     Mouse Modes	Magnifier Mode, Ruler Mode and Text Selection Mode
 The keystroke Ctrl-m [switch-mode()] switches between
  three different bindings for Mouse-1, which can also be
        activated via the Modes menu (in Motif, this is a submenu
      of the Options menu).  The default mode at startup can be
      customized via the X resource `mouseMode' or the command-line
  option `-mousemode'.  The default startup mode is Magnifier Mode.
      Note: The modes are activated by changing the magnifier()
      action. Switching the mode will not work if Mouse-1 has
        been customized to an action sequence that does not contain
           In this mode, the mouse buttons 1 to 5 will pop up a
           ``magnifying glass'' that shows an unshrunken image of
         the page (i.e. an image at the resolution determined by
        the option/X resource pixels or mfmode) at varying sizes.
              When the magnifier is moved, small ruler-like tick marks
               are displayed at the edges of the magnifier (unless
            the X resource delayRulers is set to false, in which case
              the tick marks are always displayed).
          The unit of the marks is determined by the X resource
          `tickUnits' (mm by default). This unit can be changed at
               runtime via the action `switch-magnifier-units()', by
          default bound to the keystroke `t' (see the description
        of that key, and of `switch-magnifier-units()' for more
        details on the units available).  The length of the tick
               marks can be changed via the X resource `tickLength'
           (4 by default). A zero or negative value suppresses the
        This mode allows you to select a rectangular region of
         text in the DVI file by holding down Mouse-1 and moving
        the mouse. The text is put into the X primary selection
        so that it can be pasted into other X applications with
        If xdvi has been compiled with locale, nl_langinfo() and
               iconv support, the selected text is converted into the
         character set of the current locale (see the output of
         `locale -a' for a list of locale settings available on
         your system).  If nl_langinfo() is not available, but
          iconv is, you can specify the input encoding for iconv
         via the X resource `textEncoding' (see the output of
           `iconv -l' for a list of valid encodings). If iconv support
            is not available, only the encodings ISO-8859-1 and UTF-8
              are supported (these names are case-insensitive).
              Note that UTF-8 is the only encoding that can render all
               characters (e.g. mathematical symbols). If ISO-8859-1 is
               active, characters that cannot be displayed are replaced
               by `' followed by the hexadecimal character code.  If a
        character is not recognized at all, it is replaced by
          `?'.  For other encodings, such characters may trigger
         If you want to extract larger portions of text, you
            can also save selected pages or the entire file in
             text format from the `File > Save as ...'  menu.
               This mode provides a simple way of measuring distances
         on the page.  When this mode is activated, the mouse
           cursor changes into a thin cross, and a larger, cross-
         shaped ruler is drawn in the highlight color at the
            mouse location. The ruler doesn't have units attached
          to it; instead, the current distance between the ruler
         and the mouse cursor is continously printed to the
             When activating Ruler Mode, the ruler is at first
              attached to the mouse and can be moved around.  It can
         then be positioned at a fixed place by clicking Mouse-1.
               After that, the mouse cursor can be moved to measure the
               horizontal (dx), vertical (dy) and direct (shortest)
           (dr) distance between the ruler center point and the
           Clicking Mouse-1 again will move the ruler to the
              current mouse position, and holding down Mouse-1 will
          drag the ruler around.
         In Ruler Mode, the following special keybindings extend
        or replace the default bindings:
               o      [ruler-snap-origin()] Snap the ruler back to
                   the origin coordinate (0,0).
            t      [overrides switch-magnifier-units()] Toggle
                    between various ruler units,  which can be
                     specified by the X resource tickUnits (`mm'
             P      [overrides declare-page-number()] Print the
                    distances shown in the statusline to standard
    String Search	Searching for strings in the DVI file
    The keystroke Ctrl-f or the menu entry File->Find ...
  opens a dialog window to search for a text string or a
 regular expression in the DVI file. The keystroke Ctrl-g
       The search term is converted from the character set specified
  by the current locale into UTF-8. (See the output of `locale -a'
       for a list of locale settings available on your system).
       Internally, the text in the DVI file is represented in
 UTF-8 encoding (you can view the text by saving the DVI
        file to a text file in UTF-8 encoding via the `File -> Save As ...'
    Ideographic characters from CJKV fonts are treated specially:
  All white space (spaces and newlines) before and after such
    characters is ignored in the search string and in the DVI file.
        To match a newline character, use `\n' in the search string;
   to match the string `\n', use `\\n'.
   If the checkbox Regular Expression is activated, the
   string is teated as a regular expression in extended POSIX
     format, with the following properties:
    a? matches a zero or one times
         a* matches a zero or more times
        a+ matches a one or more times. Note that * and + are
          greedy, i.e. they match the longest possible
           a{n} matches a exactly n times
         a{n,m} matches a at least n and no more than m times
           a|b matches a or b. Brackets can be used for grouping,
         The string matched by the nth group can be referenced
          by \n, e.g. \1 refers to the first match.
      The characters ^ and $ match the beginning and the end
         [abc] matches any of the letters a, b, c, and [a-z]
    matches all characters from a to z.
    The patterns . and [...] without an explicit newline
           don't match a newline character.
       Each item in a regular expression can also be one of
           the following POSIX character classes:
         [[:alnum:]] [[:alpha:]] [[:blank:]] [[:cntrl:]] [[:digit:]]
    [[:graph:]] [[:lower:]] [[:print:]] [[:space:]] [[:upper:]]
    These can be negated by inserting a ^ symbol after the
         first bracket: [^[:alpha:]]
    For more details on POSIX regular expressions, see
     e.g. the IEEE Std 1003.1 available online from:
        http://www.opengroup.org/onlinepubs/007904975/basedefs/xbd_chap09.html
         As a non-standard extension, the following Perl-like
           abbreviations can be used instead of the POSIX classes:
           Symbol Meaning                       POSIX Class
         \w   an alphanumeric character     [[:alnum:]]
         \W   a non-alphanumeric character  [^[:alnum:]]
                \d   a digit character             [[:digit:]]
         \D   a non-digit character         [^[:digit:]]
                \s   a whitespace character        [[:space:]]
         \S   a non-whitespace character    [^[:space:]]
           The following characters are special symbols; they
     need to be escaped with \ in order to match them
       literally: ( ) [ ] . * ? + ^ $ \.
   The dialog also provides checkboxes to search backwards,
       to match in a case-sensitive manner (the default is to
 ignore case, i.e. a search string Test will match both
 the strings test and TEST in the DVI file) and to ignore
       newlines and hyphens in the DVI file.
  Printing and Saving	Marking, printing and saving pages
 The `Save' and `Print' dialogs allow you to save or print all,
 pages, a range of pages, or all marked pages from a DVI file.
  Note that the page numbers for the `From ... to ...' range
     refer to physical pages, not TeX pages (compare the option
     `Use TeX Page Numbers' and the `T' keystroke).
 To mark a page or a range of pages, use one of the folllowing
  - Click on the page in the page list with Mouse Button 2 to mark
         a single page, or drag the mouse while holding down Button 2
 - Use one of the following key combinations:
        m:  toggle the mark of the current page,
       1m  toggle the marks of all odd pages,
         2m  toggle the marks of all even pages,
        Ctrl-n: toggle mark of current page, then move one page forward,
       Ctrl-u: move one page back, then toggle mark of that page.
        If the X resource or command line option `paper' has been used,
        its value is inserted into the `Dvips Options' field of the printing
   dialog so that the appropriate options can be passed to dvips.
 This doesn't happen if the paper size has been specified explicitly
    in the DVI file (e.g. by using the LaTeX `geometry' package).
  Note that  not all of the paper options used by xdvi
   may be understood by dvips; dvips will ignore the option
       in that case, and will use its default paper setting.
  Source Specials	Navigating between the TeX and the DVI file
    Some TeX implementations have an option to automatically
       include so-called `source specials' into a DVI file. These
     contain the line number and the filename of the TeX source
     and make it possible to go from a DVI file to the
      (roughly) corresponding place in the TeX source and back
       (this is also called `reverse search' and `forward search').
   On the TeX side, you need a TeX version that supports the `-src'
       option (e.g. teTeX >= 2.0) or a macro package like srcltx.sty
  to insert the specials into the DVI file.
      Source special mode can be customized for various editors
      by using the command line option "-editor" or one of the
       environment variables "XEDITOR", "VISUAL" or "EDITOR".
 See the xdvi man page on the "-editor" option for details
      Forward search can be performed by a program (i.e. your editor)
        invoking xdvi with the "-sourceposition" option like this:
     xdvi -sourceposition "<line> <filename>" <main file>
   If there is already an instance of xdvi running that displays
  <main file>, it will try to open the page specified by
 <line> and <filename> an highlight this location on the page.
  Else, a new instance of xdvi will be started that will try to
       [source-special()] Invoke the editor (the value
        of the "editor" resource ) to display the line in the
          TeX file corresponding to special at cursor position.
          [show-source-specials()]  Show bounding boxes for every
        source special on the current page, and print the strings
      contained in these specials to  stderr. With prefix 1,
         show every bounding box on the page (for debugging purposes).
          [source-what-special()]  Display information about the
         source special next to the cursor, similar to
          "source-special()", but without actually invoking
      the editor (for debugging purposes).
      Unrecognized tickUnits [%s]: defaulting to TeX points [pt]      Ruler tick interval adjusted to represent %.2f%s
       Ruler tick interval represents 1%s
     Ruler/Point: %d,%d, dx: %.*f %s, dy: %.*f %s, dr: %.*f %s       Ruler/Point: %d,%d, dx: %.*f %s, dy: %.*f %s, dt: %.*f %s       Ruler: %d,%d, Point: %d,%d, dx: %.*f %s, dy: %.*f %s, dr: %.*f %s       �z�GR@      R@R���Q@�ϊ׽�?�z�GRA      @      >@;ڨ��P@�͋ϊ�@ףp=
@SEP PUSH RADIO CHECK item != NULL cmp != NULL Widget for menu `%s' is null!     ../../../texk/xdvik/gui/menu.c  %*c-->%s; type=%d; mnemonic=%c; accel=%s; submenu=%p; w=%lu; action: %p Shouldn't happen: items == NULL!        Missing Mnemonic in button info `%s'    Xdvik doesn't support format characters in button labels; skipping button "%s"  Invalid action "%s" for button "%s" (skipping this line).       Invalid type "%s" for button "%s" (skipping this line). item in set_menu_info musn't be NULL!   comparison function musn't be NULL!     Wrong number of items (%lu) in translations line:
"%.*s" (skipping this line).
 Menu description (first item) mustn't be empty:
"%.*s" (skipping this line).
   ../../../texk/xdvik/gui/message-window.c        Invalid widget index in cancel_action() Invalid widget index in ok_action()     Wrong number of parameters in callback  too few elements in my_msg_map  type < (sizeof my_msg_map / sizeof my_msg_map[0])       ---------- helptext ----------
 
---------- end of helptext ----------
 <Message>WM_PROTOCOLS: WM_popdown(%d)   <Key>q:close-popup-cancel(%d)
<Key>Return:close-popup-cancel(%d)
<Key>Escape:close-popup-cancel(%d)
    <Key>Return:close-popup(%d)%s(%p)       <Key>Return:close-popup(%d)%s() Couldn't allocate enough colors - expect low display quality.   Greyscaling is running in copy mode; this will cause overstrike characters to appear incorrectly, and may result in poor display quality.  Possible fixes are:
- Use the ``-thorough'' command-line option.
- Quit some other color-hungry applications (e.g. Netscape).
- Use the ``-install'' command-line option.
See the section ``GREYSCALING AND COLORMAPS'' in the xdvi manual page for more details. idx >= 0 && idx < MAX_POPUPS *num_params == 1 
%s:
 message_popup message_paned message_text displayCaret wrap message_box skipAdjust fromHoriz horizDistance Xdvi Question Xdvi Help Xdvi Info Xdvi Warning Xdvi Error close-popup-cancel WM_popdown close-popup                          #�I     1�I     ;�I     E�I     R�I     ,  �  �  d   �   �   rowSpacing * %*d     %*d   Page number out of range %c %*d   viewport defaultColumns verticalList longest forceColumns reportCallback i < (int)page_info.index_size got: button-1 for `%d'
     ../../../texk/xdvik/gui/pagesel.c       end < (int)page_info.index_size page >= 0 && page < (int)page_info.index_size   index >= 0 && index < (int)page_info.index_size i <= (int)page_info.index_size  total_pages <= (int)page_info.index_size        idx < (int)page_info.index_size allocating list with %d pages
 pages_all pages_marked pages_range printer_text to_printer to_file xdvik: Save File save_popup xdvik: Print File print_popup of %d physical pages of %d to %d pages allowResize save_or_print_form dummy_print_to_label_form Print to:  print_to print_to_printer_form fromVert Printer:  state radioData highlightThickness dataCompression print_to_file_form radioGroup PS File:  file_text dummy_save_as_form Save as:  save_as save_to_file_form Format:  format_selection_label format_selection_popup menuName format_selection_button format_selection_pulldown_%d File name:  Browse ... file_button dvips_options_form Dvips Options: dvips_options_name dvips_options_text dummy_pages_form Pages: range_lab page_range_form All  range_marked_form Marked  range_from_to_form From:  range_from_text range_to_lab range_to_text range_of_lab Print print_button Save save_button *num_params > 0 *params != NULL Pointer string value: |%s| No active DVI file info->finfo->in_fp != NULL *.ps *.pdf *.txt No filename specified! Replace doesn't exist any more seems to be corrupted Print Copy Save Copy portrait  -t   -t landscape PRINTER lpr  -P OUTFILE: |%s|
 PDF Plain Text (ISO_8859-1) Plain Text (UTF-8) xaw_print_or_save WM_cancel xdvik: Save to file Save to file: xdvik: Print to file Print to file:      <Message>WM_PROTOCOLS: WM_cancel(%p)
<Key>Escape:WM_cancel(%p)
<Key>q:WM_cancel(%p)     <EnterWindow>:highlight(Always)
<LeaveWindow>:unhighlight()
<Btn1Down>,<Btn1Up>:set()notify()   <Key>Return:xaw_print_or_save(%p)       <Btn1Down>,<Btn1Up>:set()notify()       ../../../texk/xdvik/gui/print-dialog.c  client_data in cb_popdown musn't be NULL!       params in wm_cancel must be > 0!     