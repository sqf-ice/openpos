inch
     depth of root window:    %d plane%s
    number of colormaps:    minimum %d, maximum %d
         default number of colormap cells:    %d
        preallocated pixels:    black %lu, white %lu
   options:    backing-store %s, save-unders %s
   largest cursor:    unlimited    current input event mask:    0x%lx
       available colormap entries:    %d per subfield
         available colormap entries:    %d
      red, green, blue masks:    0x%lx, 0x%lx, 0x%lx
         significant bits in color specification:    %d bits
        ========== Xserver properties info end ==========
      0AC      AC     AC      AC     �@C     @AC     ffffff9@Title, Summary: |%s|%s| help_text leftMargin scrollHorizontal scrollVertical showGrip help_form help_window xdvik: Help This is xdvik, version  in the left window.
 - Athena toolkit
 - paper: a4, units cm
 - Iconv support compiled in
 - gf file support enabled
 [ Ctr-[
 Ctr-]
      number is given).
 Space key
 Del key
 Up-arrow
      Scrolls page up.
 Down-arrow
      Scrolls page down.
 Left-arrow
      Scrolls page left.
 Right-arrow
      Scrolls page right.
 T
 <, Ctrl-Home >, Ctrl-End ^
 Home
      flag is set.
 End
      `keep' flag is set.
 Ctrl-f      in the DVI file.
 Ctrl-g
 Ctrl-l
 Ctrl-o      is opened instead.
 Ctrl-p Ctrl-r or Clear
 Ctrl-s
 G
 k
      -keep option.
 M
 P
      `T' keystroke).
 S
      section `Modes').

 V
      -gsalpha option.
 the menu buttons,
 Alt-Ctrl-+ Alt-Ctrl--      value).
 Hyperlinks	Navigating links
 Mouse-1
 Mouse-2
 B
 F
     1: underline links,
     2: color links,
 Buttons 1-3
 Ctrl-Button1
      information on this).
 the magnifier() action.
 Magnifier Mode
        tick marks.
 Text Selection Mode
        Mouse-2.
        iconv error messages.
 Ruler Mode
        statusline.
        mouse.
               by default).
               output.
 jumps to the next match.
 dialog).
    sub string.
    e.g.: (a|b)|c.
    of a line, respectively.
 methods:
   to mark a range of pages.
      0m: unmark all pages,
 and examples.
 do the same.
 Ctrl-Mouse1
 Ctrl-v
 Ctrl-x
       Help resource label `%s' doesn't contain a tab character - ignoring it. ../../../texk/xdvik/gui/help-window.c   #override 
<Key>q:close-topic-window(%p)
<Key>Escape:close-topic-window(%p)
<Key>Return:close-topic-window(%p)  Help resource text `%s' doesn't contain a newline character.    Shouldn't happen: Could not find topic `%s' in help list!
Please report this as a bug to:
   http://sourceforge.net/tracker/?group_id=23164&atid=377580
        Introduction	About this version of xdvi
        22.84.16.
The program's homepage is located at:
        http://sourceforge.net/projects/xdvi
   where you can find updates, report bugs and submit feature requests.
   Xdvi has many command-line options, too numerous to be listed here;
    see the man page for a full description.
       The most important key bindings are listed in the help sections shown
  Note: Unless a key binding also has an uppercase version,
      all bindings are case-insensitive.

    The major parts of Xdvik are licensed under the X Consortium license.
  Parts (encoding.c) are licensed under the GNU General Public License.
  Xdvik uses the following libraries:
    - The kpathsea library, licensed in part under the GNU General Public
    License, in part under the GNU Library General Public License.
       - t1lib, licensed in parts under the GNU Library General Public License,
         in parts under the X Consortium license.
     There is NO WARRANTY of anything.
      Built using these configure options:
   - anti-aliasing (grey) enabled
 - T1lib (direct rendering of PS fonts) enabled
 - Langinfo support compiled in
 Page Motion	Moving around in the document
           Moves back one item in the page history. With a prefix
         argument n, move back n history items.

        Moves forward one item in the page history. With a prefix
      argument n, move forward n history items.

             Deletes current item in the page history and move
      to the history item before the deleted one. With a prefix
      argument n, delete n previous history items.
           to the history item after the deleted one. With a prefix
       argument n, delete n next history items.
  n or f or Return or LineFeed or PgDn         Moves to the next page (or to the nth next page if a
           Moves down or to the next page.    p or b or Ctrl-h or BackSpace or PgUp        Moves to the previous page (or back n pages).
          Moves up on the page or to the previous page.           Moves page up two thirds of a window-full.
             Moves page down two thirds of a window-full.
           Moves page left two thirds of a window-full.
           Moves page right two thirds of a window-full.
    