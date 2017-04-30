//------------------------------------------------------------------------------
// NOTICE
//------------------------------------------------------------------------------
//
// "Markdown to HTML converter" short "mdownhtml"
//
// Converts markdown to HTML, partly mixing some commonmark with
// John Grubers definition of markdown.
//
// Uses/converts the ATX-Syntax for markdown headings!
// Initially written for the www.it-talents.de coding competition
//
// Version date: 29.04.2017
//
// Author: Jan R.
// Contact: <jan AT dwrox DOT net>
//
//------------------------------------------------------------------------------


// CODE START
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


//------------------------------------------------------------------------------
// Definitions
//------------------------------------------------------------------------------
#define PARAGRAPH_LISTS       // Define if we should create "<li>" elements with paragraphed content, equals "<li><p></p></li>"
#undef PARAGRAPH_LISTS        // Comment me out to create <li><p>CONTENT</p></li> elements

//------------------------------------------------------------------------------
// Structs
//------------------------------------------------------------------------------
typedef struct html_tag {
  bool isClosed;              // Is the markdown tag already closed with html?
  bool isSibling;             // TODO: Add comment
  bool isListEnd;
  unsigned int tagType;       // Which mardkown/html tag are we dealing with?
  short formatDepth;
} html_tag;

typedef struct html_collection {
  long count;                 // The amount of tags
  struct html_tag* tags;      // The tag list
} html_collection;

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
// None present... its just one big main, splitted in 3 to 4 logical blocks

//------------------------------------------------------------------------------
// Code
//------------------------------------------------------------------------------

int main(int argc, char *argv[]) {

  char* inputName = NULL;   // "input_markdown.md";
  char* outputName = NULL;  //"output_html.html";

  bool hasInput = false;
  bool hasOutput = false;

  printf("Mdownhtml - v29.04.2017 -\nAuthor and contact: <jan@dwrox.net>\n\n");

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (strlen(argv[i]) > 1) {
        if (tolower(argv[i][1]) == 'i') {
          inputName = malloc(strlen(argv[i]) * sizeof(char));
          strcpy(inputName, &argv[i][2]);
          hasInput = true;
        } else if (tolower(argv[i][1]) == 'o') {
          outputName = malloc(strlen(argv[i]) * sizeof(char));
          strcpy(outputName, &argv[i][2]);
          hasOutput = true;
        } else if (tolower(argv[i][1]) == 'h') {
          printf("Usage:\n-i\tInput file name, use '-iMyInputFilename'\n-o\tOutput file name, use '-oMyOutputFilename'\n-h\tPrint this help\n\nBy default if nothing is provided the input file 'input_markdown.md' is used.\nAs output, the default file is called 'output_html.html'\n\n");

          free(inputName);
          free(outputName);
          return EXIT_SUCCESS;
        }
      } else {
        printf("Usage:\n-i\tInput file name, use '-iMyInputFilename'\n-o\tOutput file name, use '-oMyOutputFilename'\n-h\tPrint this help\n\nBy default if nothing is provided the input file 'input_markdown.md' is used.\nAs output, the default file is called 'output_html.html'\n\n");
        return EXIT_SUCCESS;
      }
    }
  }

  if (!hasInput) {
    inputName = malloc(19);
    strcpy(inputName, "input_markdown.md");
  }

  if (!hasOutput) {
    outputName = malloc(17);
    strcpy(outputName, "output_html.html");
  }

  // Open the input and out file and deal with errors
  FILE* inputFile = fopen(inputName, "rb");

  if (inputFile == NULL) {
    fprintf(stderr, "Cannot open input markdown file '%s' for reading.\nWithout parameters this defaults to 'input_markdown.md'.\n\nUse mdownhtml -h to show a list of commands.\n", inputName);
    free(inputName);
    free(outputName);
    return EXIT_FAILURE;
  }

  FILE* outputFile = fopen(outputName, "wb");

  if (outputFile == NULL) {
    fclose(inputFile);

    free(inputName);
    free(outputName);

    fprintf(stderr, "Cannot open output html file '%s' for writing.\nWithout parameters this defaults to 'output_html.html'.\n\nUse mdownhtml -h to show a list of commands.\n", outputName);
    return EXIT_FAILURE;
  }

  //----------------------------------------------------------------------------
  // Tag definitions and "htmlCollection" as data holder for the open markdown tags

  #ifdef PARAGRAPH_LISTS
  char tagOpenings[12][24] = {
    "<h1>", "<h2>", "<h3>", "<h4>", "<h5>", "<h6>",  "<li><p>", "<strong>", "<em>", "<p>", "<blockquote>", "<strong><em>"
  };

  char tagClosings[12][27] = {
    "</h1>", "</h2>", "</h3>", "</h4>", "</h5>", "</h6>", "</p></li>", "</strong>", "</em>", "</p>", "</blockquote>", "</em></strong>"
  };

  #else

  char tagOpenings[12][24] = {
    "<h1>", "<h2>", "<h3>", "<h4>", "<h5>", "<h6>",  "<li>", "<strong>", "<em>", "<p>", "<blockquote>", "<strong><em>"
  };

  char tagClosings[12][27] = {
    "</h1>", "</h2>", "</h3>", "</h4>", "</h5>", "</h6>", "</li>", "</strong>", "</em>", "</p>", "</blockquote>", "</em></strong>"
  };

  #endif

  struct html_collection htmlCollection = { 0, NULL };  // Our data collection for all present markdown/html tags


  //----------------------------------------------------------------------------

  char inputChar = '\0';    // The currently processed character of the input file
  char nextChar = '\0';     // In case we deal with a follow up character for the formatting
  char oldChar = '\0';      // Temporary store nextChar to check the following character, in some case
  bool lineStart = true;    // Are we at the beginning of a new line?
  bool lineEnd = false;     // Are we at the end?
  bool tagIsClosed = false; // Is the tag closed already?
  int tagType = -1;         // Which markdown/html tag are we dealing with
  int headerDepth = 0;      // How deep is a header markdown?
  int formatDepth = 0;      // How deep is the formatting level of asterisk/star characters?
  int depthDiff = 0;
  bool hasOpenParagraph = false;
  bool hasOpenQuote = false;
  bool hasOpenList = false;
  long orgPos = 0;

  printf("Start converting '%s' and saving to '%s'...\n", inputName, outputName);

  // Main loop for readout and creation
  while (true) {
    inputChar = fgetc(inputFile);
    nextChar = '\0';
    tagType = -1;
    headerDepth = 0;
    formatDepth = 0;
    depthDiff = 0;
    hasOpenParagraph = false;
    hasOpenQuote = false;
    hasOpenList = false;

    switch (inputChar) {
      case '#':
        if (lineStart) {
          // Create a header object if we are at linestart and gather the depth
          headerDepth = 1;

          while ((nextChar = fgetc(inputFile)) == '#') {
            ++headerDepth;
          }

          if (nextChar != ' ') {
            fseek(inputFile, -1, SEEK_CUR);
          }
          tagType = headerDepth - 1;

          // Previous method
          /*if (fgetc(inputFile) == '#') {
            tagType = 1;
          } else {
            tagType = 0;
            fseek(inputFile, -1, SEEK_CUR);
          }*/
        } else {
          tagType = 9;
        }

        break;
      case '*':
        // Create a text format or list item, either italic or bold or bold italic based on start
        if (lineStart) {
          // We are at linestart... could be a list item or a format or an empty list item

          nextChar = fgetc(inputFile);

          if (nextChar == ' ' || nextChar == '\t' || nextChar == '\n' || nextChar == '\r') {
            tagType = 6;
            formatDepth = 0;

            if (nextChar == '\n' || nextChar == '\r') {
              hasOpenList = false;

              if (htmlCollection.count != 0 && htmlCollection.tags[htmlCollection.count - 1].tagType == 6 && !htmlCollection.tags[htmlCollection.count - 1].isClosed) {
                orgPos = ftell(inputFile);
                while (true) {
                  nextChar = fgetc(inputFile);
                  if (nextChar != '\n' || nextChar != '\r') {
                    break;
                  }
                }

                if (nextChar == '*') {
                  hasOpenList = true;
                  fseek(inputFile, -1, SEEK_CUR);
                } else {
                  fseek(inputFile, orgPos, SEEK_SET);
                }
              }

              if (!hasOpenList) {
                if (htmlCollection.count != 0 && htmlCollection.tags[htmlCollection.count - 1].tagType == 9 && !htmlCollection.tags[htmlCollection.count - 1].isClosed) {
                  if (fgetc(inputFile) != '*') {
                    tagType = -1;
                  }

                  fseek(inputFile, -1, SEEK_CUR);
                }
              }
            }

            if (nextChar != ' ') {
              fseek(inputFile, -1, SEEK_CUR);
            }
          } else {
            // Else we have format
            fseek(inputFile, -1, SEEK_CUR);
            formatDepth = 1;
          }
        } else {
          // If we are not at the start, its italic first
          formatDepth = 1;
          tagType = 8;
        }

        if (formatDepth != 0) {
          while (fgetc(inputFile) == '*') {
            ++formatDepth;
          }

          switch (formatDepth) {
            case 1:
              // its italic
              tagType = 8;
              break;
            case 2:
              // if another star follows, its bold
              tagType = 7;
              break;
            case 3:
              // if another star follows, its bolditalic
              tagType = 11;
              break;
            default:
              break;
          }

          fseek(inputFile, -1, SEEK_CUR);
        }

        break;
      case '>':
        if (lineStart) {
          // At the line start, we deal with quote markdown
          tagType = 10;

          for (long i = htmlCollection.count - 1; i > -1; --i) {
            // Check if we already have an not closed quote, so we dont create a new tag
            if (!htmlCollection.tags[i].isClosed && htmlCollection.tags[i].tagType == 10 && !htmlCollection.tags[i].isListEnd) {
              tagType = -1;
              break;
            }
          }

          nextChar = fgetc(inputFile);

          if (nextChar != ' ') {
            fseek(inputFile, -1, SEEK_CUR);
          } else {
            if (tagType == -1) {
              // If we dont create a new tag, ignore the line start and markdown character
              lineStart = false;
              continue;
            } else {
              fputc('#', inputFile);
            }
          }
        }
        break;
      case ' ':

        // Check if we are at line end with two spaces or more spaces and insert a br if the line ends
        if (lineStart) {
          tagType = 9;
        } else {
          orgPos = ftell(inputFile);

          formatDepth = 1;

          while ((nextChar = fgetc(inputFile)) == ' ') {
            ++formatDepth;
            continue;
          }

          if (formatDepth == 2 && (nextChar == '\n' || nextChar == '\r')) {
            fputs("<br/>", outputFile);
            lineStart = true;
            lineEnd = false;
            continue;
          } else {
            fseek(inputFile, orgPos, SEEK_SET);
            fputc(' ', outputFile);
            continue;
          }
        }
        break;
      case '\n':
      case '\r':
      case EOF:
        // Set that the current line is ended
        lineEnd = true;
        break;
      default:
        if (lineStart || (htmlCollection.count != 0 && htmlCollection.tags[htmlCollection.count - 1].tagType == 10 && !htmlCollection.tags[htmlCollection.count - 1].isClosed)) {
          // If we are at a start of a new line, we assume that we should use a paragraph for every text item
          // in case the last open tag is a quote, we could wrap it into a p - tag too
          tagType = 9;

          if (htmlCollection.count != 0) {
            // If we have possible open tags, check if we really need to add a paragraph
            // in case there is a open blockquote, we create a paragraph, in case of a open paragraph we dont create a new one
            hasOpenParagraph = false;
            hasOpenQuote = false;

            for (long i = htmlCollection.count - 1; i > -1; --i) {
              if (htmlCollection.tags[i].tagType == 9 && !htmlCollection.tags[i].isClosed) {
                hasOpenParagraph = true;
                break;
              } else if (htmlCollection.tags[i].tagType == 10 && !htmlCollection.tags[i].isClosed) {
                hasOpenQuote = true;
                break;
              }
            }

            if (hasOpenParagraph && !hasOpenQuote) {
              tagType = -1;
            }
          }
        }
        break;
    }


    //--------------------------------------------------------------------------

    tagIsClosed = false;  // Is the tag closed

    // If we are at line end, or we have a follow up text format or an open tag, try to close it...
    if ((lineEnd || tagType != -1) && htmlCollection.count != 0) {
      nextChar = fgetc(inputFile);     // Which character follows? - This becomes also dynamically changed on demand
      oldChar = nextChar;              // Backup the current selection
      char pChar = '\0';
      bool isParserAtEnd = false;      // Are we at the end of the file? This is needed to finalize with the last read in data and break out of execution

      if (nextChar == EOF) {
        nextChar = '\n';               // Overwrite the EOF, so the switch case statement can deal with it
        isParserAtEnd = true;
      }

      if (nextChar == '*') {
        // This is most likely a text format or a list following, particular at the linEend
        nextChar = fgetc(inputFile);
        if (nextChar == ' ' || nextChar == '\n' || nextChar == '\r' || (nextChar == '*' && oldChar == ' ')) {
          // Its a list item
          nextChar = '*';
        } else {
          // Its most likely the end of a list item...
          hasOpenList = false;          // Do we have a list which needs to be closed now?
          for (long i = htmlCollection.count - 1; i > -1; --i) {
            if (htmlCollection.tags[i].tagType == 6 && !htmlCollection.tags[i].isClosed) {
              hasOpenList = true;       // Yes
              nextChar = 0;             // Set nextChar to zero, to add special case handling
              break;
            }
          }

          if (!hasOpenList) {
            nextChar = oldChar;         // We dont have an open list, get back to the old picked character threatment
          }
        }

        if (nextChar != ' ') {
          fseek(inputFile, -1, SEEK_CUR); // Rewind by one
        }
      } else {
        // This is not a list, so we check if we are at the end of a paragraph item which is not yet closed
        if (lineEnd) {
          for (long i = htmlCollection.count - 1; i > -1; --i) {
            if (htmlCollection.tags[i].tagType == 6 && !htmlCollection.tags[i].isClosed) {
              nextChar = 0;     // Set a special rule to handle a paragraph correctly
              break;
            }
          }
        }
      }

      for (long i = htmlCollection.count-1; i > -1; --i) {
        if (!htmlCollection.tags[i].isClosed) {
          // Check all open markdown/html tags

          if (htmlCollection.tags[i].tagType == 6) {
            // If we have a open list element
            switch (nextChar) {
              case '*':
                // Has a follow list element, close only the li element
                fputs(tagClosings[6], outputFile);                 // Add closing </li> element
                htmlCollection.tags[i].isClosed = true;            // Close the tag
                htmlCollection.tags[i].isListEnd = false;
                break;
              case '\n':
              case '\r':
              case 0:
                // Has no following list element, close the li and ul elements
                #ifdef PARAGRAPH_LISTS
                fputs("</p></li></ul>", outputFile);              // Add special ending not in closing list (with paragraph)
                #else
                fputs("</li></ul>", outputFile);                  // Add special ending not in closing list
                #endif
                htmlCollection.tags[i].isClosed = true;           // Close...
                htmlCollection.tags[i].isListEnd = true;
                break;
              default:
                break;
            }

          } else if (htmlCollection.tags[i].tagType == 9) {
            // If we have a paragraph
            switch (nextChar) {
              case '>':
                pChar = fgetc(inputFile);
                if (pChar == '\n' || pChar == '\r') {
                  fputs(tagClosings[9], outputFile);
                  htmlCollection.tags[i].isClosed = true;
                }
                fseek(inputFile, -1, SEEK_CUR);
                break;
              case '\n':
              case '\r':
                // Close the paragraph, we have a empty line in between which might introduce a new paragraph
                // Add closing p tag
                if (inputChar != '*') {
                  fputs(tagClosings[9], outputFile);
                  htmlCollection.tags[i].isClosed = true;
                }
                break;
              case 0:
                // Special threatment case, there is a newline following, close the paragraph
                if (oldChar == '\n' || oldChar == '\r') {
                  fputs(tagClosings[9], outputFile);
                  htmlCollection.tags[i].isClosed = true;
                }
                break;
              default:
                // If anything else follows and we deal with a paragraph, reset it to avoid creation of a new one
                if (tagType == 9) {
                  tagType = -1; // Reset, only affects the current dealed with tag
                }
                break;
            }

          } else if (htmlCollection.tags[i].tagType == 10) {
            // Add and close a quote tag

            if (lineEnd && nextChar != '>' && (nextChar == '\n' || nextChar == '\r')) {
              // if we are at the end of the line and next line has no connected quote markdown, close the quote
              fputs(tagClosings[10], outputFile);
              htmlCollection.tags[i].isClosed = true;
              htmlCollection.tags[i].isListEnd = true;
            }
          } else {
            if (tagType == htmlCollection.tags[i].tagType) {
              // If the processed tag type is equal to the current processed one (we go from top to bottom/backwards), close this tag
              tagIsClosed = true; // Set this, to avoid readding another closing for this tag
              htmlCollection.tags[i].isClosed = true; // Close the tag
              fputs(tagClosings[htmlCollection.tags[i].tagType], outputFile);
            } else if (htmlCollection.tags[i].tagType > 5 && htmlCollection.tags[htmlCollection.count - 1].tagType == tagType && !htmlCollection.tags[htmlCollection.count - 1].isClosed) {
              // if we dont deal with a headline, close the tag
              fputs(tagClosings[htmlCollection.tags[i].tagType], outputFile);
            } else if (lineEnd) {
              // if we deal with a headline and are at the line end, close it as a headline affects the whole line
              // close only at end
              fputs(tagClosings[htmlCollection.tags[i].tagType], outputFile);
              htmlCollection.tags[i].isClosed = true;
            }
          }
        }
      }

      if (isParserAtEnd)  {
        // EOF, break out the main loop
        break;
      } else {
        fseek(inputFile, -1, SEEK_CUR);
      }
    }

    //--------------------------------------------------------------------------
    // Here we start to check for closing of tags or handle if we have to create a new html tag

    if (tagType != -1 && !tagIsClosed ) {
      // Add tag html to outputFile

      bool doCreateTag = true;    // Create a new tag by defualt


      if (tagType == 7 || tagType == 8 || tagType == 11) {
        // Check if we have a following markdown character for text formatting
        // (bold, italic) or if a '*' only occurs as normal text
        bool addLeadingParagraph = true;

        // Check if we have any open data format
        for (long i = htmlCollection.count - 1; i > -1; --i) {
          if ((htmlCollection.tags[i].tagType == 6 || htmlCollection.tags[i].tagType == 9 || htmlCollection.tags[i].tagType == 10) && !htmlCollection.tags[i].isClosed) {
            // Wrap into a paragraph if not present for some items
            addLeadingParagraph = false;
            break;
          }
        }

        if (addLeadingParagraph) {
          htmlCollection.tags = realloc(htmlCollection.tags, sizeof(html_tag) * (htmlCollection.count + 1));
          htmlCollection.tags[htmlCollection.count].isClosed = false;
          htmlCollection.tags[htmlCollection.count].isSibling = false;
          htmlCollection.tags[htmlCollection.count].isListEnd = false;
          htmlCollection.tags[htmlCollection.count].tagType = 9;
          htmlCollection.tags[htmlCollection.count].formatDepth = 0;
          ++htmlCollection.count;
          fputs(tagOpenings[9], outputFile); // Add the "<p>"
        }

        doCreateTag = false;

        long orgPos = ftell(inputFile);   // Save the current file position
        char dataChar = '\0';             // The current character
        bool doExit = false;              // Should we exit the while loop?
        short tempFormatDepth = 0;        // Temporary store the depth on look aheads


        while (true) {
          dataChar = fgetc(inputFile);
          switch (dataChar) {
            case '\n':
            case '\r':
              // We are at the end of the line...
              doCreateTag = false;    // Dont create a text format tag
              doExit = true;          // Exit the loop!
              if (tagType == 7) {
                // If we have an bold tag, place one addional star, which otherwise would be lost
                depthDiff = 1;
              } else if (tagType == 11) {
                // If we have an italicbold tag, place two addional asterisks, which otherwise would be lost
                depthDiff = 2;
              }
              break;
            case '*':
              tempFormatDepth = 1;

              while (fgetc(inputFile) == '*') {
                ++tempFormatDepth;
              }

              if (tempFormatDepth == formatDepth) { // The asterisk match
                doCreateTag = true;
                doExit = true;
              } else {
                // The tag is smaller as the one initially provided
                // if its not, it could be another nested tag this seems to do less harm as the opposite
                if (formatDepth > tempFormatDepth) {
                  depthDiff = formatDepth - 1;
                  doExit = true;
                }
              }

              fseek(inputFile, -1, SEEK_CUR);
              break;
            default:
              break;
          }

          if (doExit || dataChar == EOF) {
            break;
          }
        }

        fseek(inputFile, orgPos, SEEK_SET); // Reset to current reading position
      }



      if (doCreateTag) {
        // Yes, we create a html tag
        bool isSibling = false;

        if (tagType == 6) {
          // If we deal with a list item
          bool hasOpener = false;

          for (long i = htmlCollection.count - 1; i > -1; --i) {
            if (htmlCollection.tags[i].tagType == 6 && !htmlCollection.tags[i].isListEnd && !htmlCollection.tags[i].isSibling) {
              hasOpener = true;
              break;
            } else if (htmlCollection.tags[i].tagType == 6 && htmlCollection.tags[i].isListEnd) {
              break;
            }
          }

          if (!hasOpener) {
            // There is no list open, create a new "<ul><li>"...
            fputs("<ul>", outputFile);
            fputs(tagOpenings[tagType], outputFile);  // Add the <li>
            isSibling = false;
          } else {
            // We have a open list, create only the "<li>"
            fputs(tagOpenings[tagType], outputFile);
            isSibling = true;
          }

        } else {
          if (lineStart && (tagType == 7 || tagType == 8 || tagType == 11)) {
            // If we are at lineStart and have bold or italic markdown or bolditalic
            bool hasOpenTag = false;
            for (long i = htmlCollection.count - 1;i > -1; --i) {
              if (htmlCollection.tags[i].tagType == 9 && !htmlCollection.tags[i].isClosed) {
                // We are inside a paragraph, dont create a new one
                hasOpenTag = true;
                break;
              }
            }

            if (!hasOpenTag) {
              // The markdown is not surrounded by a paragraph, create one and put its data in memory
              htmlCollection.tags = realloc(htmlCollection.tags, sizeof(html_tag) * (htmlCollection.count + 1));
              htmlCollection.tags[htmlCollection.count].isClosed = false;
              htmlCollection.tags[htmlCollection.count].isSibling = false;
              htmlCollection.tags[htmlCollection.count].isListEnd = false;
              htmlCollection.tags[htmlCollection.count].tagType = 9;
              htmlCollection.tags[htmlCollection.count].formatDepth = 0;
              ++htmlCollection.count;
              fputs(tagOpenings[9], outputFile); // Add the "<p>"
            }
          }

          fputs(tagOpenings[tagType], outputFile); // Add the html markdown variant for the text format
        }

        // Put the data for the new tag in memory
        htmlCollection.tags = realloc(htmlCollection.tags, sizeof(html_tag) * (htmlCollection.count + 1));
        htmlCollection.tags[htmlCollection.count].isClosed = false;
        htmlCollection.tags[htmlCollection.count].isSibling = isSibling;
        htmlCollection.tags[htmlCollection.count].isListEnd = false;
        htmlCollection.tags[htmlCollection.count].tagType = tagType;
        htmlCollection.tags[htmlCollection.count].formatDepth = formatDepth;
        ++htmlCollection.count;

      } else {
        // We dont create a new tag, this is because its a regular '*' inside the text, so we add this symbol
        tagType = -1;
      }

    }

    // Special case handling for line breaks if we have no tag starts here, tagtype -1 means, we have a regular character
    // tagtype 9, we openend up a paragraph
    if (tagType == -1 || tagType == 9) {

      // If we dont have a tag markdown character or a normal text charater with a paragraph
      bool addedNewLine = false;

      if (lineEnd) {

        // if we are at lineEnd safely check if we have a open paragraph we are inside of
        nextChar = fgetc(inputFile);

        if (nextChar == '>') {
          nextChar = fgetc(inputFile);

          if (nextChar != '\n' && nextChar != '\r') {
            for (long i = htmlCollection.count - 1; i > -1; --i) {
              if (!htmlCollection.tags[i].isClosed && (htmlCollection.tags[i].tagType == 9 || htmlCollection.tags[i].tagType == 10)) {
                // We have an open paragraph or blockquote

                // If we have either a list element, bold or italic text, set a new line

                if (htmlCollection.tags[htmlCollection.count - 1].tagType >= 7 && htmlCollection.tags[htmlCollection.count - 1].tagType <= 11) {
                  if (htmlCollection.tags[i].tagType == 10 && nextChar != '>') {
                    fputs("<br/>\n", outputFile); // set a html line ending "<br/>"

                    // Any text is not surrounded by a paragraph, create one and put its data in memory
                    htmlCollection.tags = realloc(htmlCollection.tags, sizeof(html_tag) * (htmlCollection.count + 1));
                    htmlCollection.tags[htmlCollection.count].isClosed = false;
                    htmlCollection.tags[htmlCollection.count].isSibling = false;
                    htmlCollection.tags[htmlCollection.count].isListEnd = false;
                    htmlCollection.tags[htmlCollection.count].tagType = 9;
                    htmlCollection.tags[htmlCollection.count].formatDepth = 0;
                    ++htmlCollection.count;
                    fputs(tagOpenings[9], outputFile); // Add the "<p>"
                    addedNewLine = true;
                    break;
                  } else if (htmlCollection.tags[i].tagType == 9 && nextChar == ' ') {
                    fputs("<br/>", outputFile); // set a html line ending "<br/>"
                    break;
                  }
                }
              }
            }

            fseek(inputFile, -1, SEEK_CUR);
          }

          fseek(inputFile, -1, SEEK_CUR);

        } else if (nextChar != '\n' && nextChar != '\r') {
          oldChar = nextChar;
          nextChar = fgetc(inputFile);

          if (oldChar == '*' && nextChar != ' ' && htmlCollection.count != 0 && !htmlCollection.tags[htmlCollection.count - 1].isClosed && (htmlCollection.tags[htmlCollection.count - 1].tagType == 9 || htmlCollection.tags[htmlCollection.count - 1].tagType == 10)) {
            // This checks, if we a possible format or list character which would avoid creating a line break in some cases
            fputs("<br/>", outputFile); // set a html line ending "<br/>"
          } else {
            // Test if we have open items...
            hasOpenQuote = false;
            hasOpenParagraph = false;
            hasOpenList = false;

            if (nextChar == ' ') {
              for (long i = htmlCollection.count - 1; i > -1; --i) {
                if (htmlCollection.tags[i].tagType == 10 && !htmlCollection.tags[i].isClosed) {
                  // We have an open blockquote
                  hasOpenQuote = true;
                  break;
                } else if (htmlCollection.tags[i].tagType == 9 && !htmlCollection.tags[i].isClosed) {
                  hasOpenParagraph = true;
                  break;
                } else if (htmlCollection.tags[i].tagType == 6 && !htmlCollection.tags[i].isClosed && !htmlCollection.tags[i].isListEnd) {
                  hasOpenList = true;
                  break;
                }
              }
            }

            if (!lineStart && (!hasOpenQuote && hasOpenParagraph && !hasOpenList)) {
              if (htmlCollection.count != 0 && htmlCollection.tags[htmlCollection.count - 1].tagType != 6)
              fputs("<br/>", outputFile); // set a html line ending "<br/>"
            }
          }

          fseek(inputFile, -2, SEEK_CUR);
        } else {
          fseek(inputFile, -1, SEEK_CUR);
        }

      }

      // Write out asterik characters in case we have a mismatch from formatting
      if (depthDiff != 0) {
        for (int i = 0;i < depthDiff; ++i) {
          fputc('*', outputFile);
        }
      }


      // Store any other data like regular text and such into output, char by char...
      // Escape only angle brackets with html entities, other characters stay
      if (!addedNewLine) {
        if (inputChar == '<') {
          fputs("&lt;", outputFile);
        } else if (inputChar == '>') {
          fputs("&gt;", outputFile);
        } else {
          fputc(inputChar, outputFile);
        }
      }
    }

    //--------------------------------------------------------------------------
    lineStart = false;  // Set that we are not anymore at the line start after processing one character

    if (lineEnd) {
      // If we are at the line end, set linestart active again
      lineStart = true;
      lineEnd = false;
    }

    //--------------------------------------------------------------------------
    if (inputChar == EOF) {
      // End of file character found, break the main loop
      break;
    }

  }

  //----------------------------------------------------------------------------
  // Cleanup
  fclose(inputFile);
  fclose(outputFile);

  free(inputName);
  free(outputName);
  free(htmlCollection.tags);

  //----------------------------------------------------------------------------
  printf("Finished parsing conversion.\n");
  return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
