
## Tool usage:
By default, the parser is looking for "input_markdown.md" for markdown input and
saves the html to "output_html.html".

Parameters/commands:
* -i  >> Defines the input file, use "-iMyInputFilename"
* -o  >> Defines the output file, use "-oMyOutputFilename"
* -h  >> Shows the help and exits
* -   >> No parameter given, shows the help

# Headers, whitespace and a litte about paragraphs
###Other headers without a space
## Spaces become removed
# I guess thats all we need

There is just one more think, a paragraph
##does not become interrupted by a headline, the headline element is wrapped into the paragraph!
And here is the third and last line of this paragraph

versus

First line (its a single paragraph)
 ## Second, this not a headline as we have a whitespace at the start of the line!
Third...

### headline is a headline element

First line (its a single paragraph)
Second...
Third...



## Quotes

> A quote
with a line break
> and a different kind of linebreak as we make use of '>' which states a new line.

> A new quote - maybe ;)
> that looks a bit cleaner.
>
> Oh, and it has a second paragraph.

a empty quote:

>

which becomes completely ignored! :)

Or what is that here:

*
*

Two stars, wrapped into one paragraph? No, its a list of two empty elements.


In comparison to

*italic*

or with a space at the line start.

 *
 *

The last ones have preceeding whitespaces! So the do not open lists and are instead placed into a paragraph.

A bit more about quotes:

> A soft line break
and here is the second line of the quote
and a third line of text.

> Versus this
> here, those are two totally indenpented lines
> and here is the hard line break too, cause we are citing and FORCE a wrap

But also:

> A new citation
 a new line
another line, all handles with soft line breaks

#Markdown Parser

##Erstelle Deinen eigenen Markdown-Parser und wandle Markdown-Code in HTML-Code um.
**Du hast noch: 22 Tage 10 Stunden 59 Minuten 15 Sekunden**

#{ABOUT}
Was ist eigentlich ein Parser?

Mit **Hilfe von Parsern wird eine Eingabe zerlegt, analysiert und daraus eine Ausgabe in einem anderen Format erstellt**. In *der Regel wird* also *eine (Text-)Eingabe in eine neue Struktur übersetzt*.
Und **so *soll es auch bei dieser ***Code Competition*** sein. Du sollst **einen Parser (=Übersetzer) schreiben**, der **Markdown-Dokumente in HTML-Quellocde übersetzt**.

##{INPUT}
* Es bietet sich die Verwendung von regulären Ausdrücken (*Regular Expressions*) an.

Deine Abgabe soll:

* Die Eingabe eines mit Markdown strukturierten Dokuments erlauben. Hierbei ist die Eingabe über einen Textbereich in einer GUI möglich, aber auch das Einlesen einer Datei (über eine GUI, oder über die Kommandozeile).
Folgende Markdown-Syntax soll berücksichtigt werden und entsprechend in HTML-Code umgewandelt werden:

Markdown-Zeichen	HMTL-Code
# 			h1-Überschrift
##		 	h2-Überschrift
*		 	Ein Stern am Anfang einer Zeile beginnt eine Auflistung (Jedes Element einer Liste wird mit einem Stern gekennzeichnet)

**fetter_text**		Ein fett geschriebener Text wird mit zwei Sternen (**) eingeleitet und wieder beendet.

*kursiver_text*		Kursiv geschriebener Text wird mit einem Stern eingeleitet und einem Stern beendet (*)

> Ein Zitat wird durch ein > am Zeilenanfang eingeleitet
> Und hier geht das Zitat in eine neue Zeile über

Leerzeile		Ein Paragraph wird durch eine Leerzeile zwischen Textblücken angegeben.


#{REVIEW}

**Worauf achten wir bei der Bewertung Deiner Abgabe?**

* Funktionalität: Lässt sich das Programm bedienen? Tut die Anwendung oder die Funktion, was sie soll? Wie umfangreich sind die Funktionen?
* Code-Qualität: Ist der Code sinnvoll strukturiert und effizient?
* Code-Lesbarkeit / Dokumentation: Lässt sich der Quellcode nachvollziehen? Ist der Code kommentiert?
* Setup: Ist das System einfach einzurichten / aufzusetzen? (z.B. mittels guter Dokumentation, Docker, Vagrant, Skripte, o.ä.)
* Welche Zusatzfeatures wurden eingebaut?

**Wie bewerten wir?**

##{POST}


# NEW CONTENT


# An exhibit of Markdown

This note demonstrates some of what [Markdown][1] is capable of doing.

*Note: Feel free to play with this page. Unlike regular notes, this doesn't automatically save itself.*

## Basic formatting

Paragraphs can be written like so. A paragraph is the basic block of Markdown. A paragraph is what text will turn into when there is no reason it should become anything else.

Paragraphs must be separated by a blank line. Basic formatting of *italics* and **bold** is supported. This *can be **nested** like* so.

## Lists

### Ordered list

1. Item 1
2. A second item
3. Number 3
4. Ⅳ

*Note: the fourth item uses the Unicode character for [Roman numeral four][2].*

### Unordered list

* An item
* Another item
* Yet another item
* And there's more...

## Paragraph modifiers

### Code block

    Code blocks are very useful for developers and other people who look at code or other things that are written in plain text. As you can see, it uses a fixed-width font.

You can also make `inline code` to add code into other things.

### Quote

> Here is a quote. What this is should be self explanatory. Quotes are automatically indented when they are used.

## Headings

There are six levels of headings. They correspond with the six levels of HTML headings. You've probably noticed them already in the page. Each level down uses one more hash character.

### Headings *can* also contain **formatting**

### They can even contain `inline code`

Of course, demonstrating what headings look like messes up the structure of the page.

I don't recommend using more than three or four levels of headings here, because, when you're smallest heading isn't too small, and you're largest heading isn't too big, and you want each size up to look noticeably larger and more important, there there are only so many sizes that you can use.

## URLs

URLs can be made in a handful of ways:

* A named link to [MarkItDown][3]. The easiest way to do these is to select what you want to make a link and hit `Ctrl+L`.
* Another named link to [MarkItDown](http://www.markitdown.net/)
* Sometimes you just want a URL like <http://www.markitdown.net/>.

## Horizontal rule

A horizontal rule is a line that goes across the middle of the page.

---

It's sometimes handy for breaking things up.

## Images

Markdown can also contain images. I'll need to add something here sometime.

## Finally

There's actually a lot more to Markdown than this. See the official [introduction][4] and [syntax][5] for more information. However, be aware that this is not using the official implementation, and this might work subtly differently in some of the little things.


  [1]: http://daringfireball.net/projects/markdown/
  [2]: http://www.fileformat.info/info/unicode/char/2163/index.htm
  [3]: http://www.markitdown.net/
  [4]: http://daringfireball.net/projects/markdown/basics
  [5]: http://daringfireball.net/projects/markdown/syntax



#### TEXT

It's very easy to make some words **bold** and other words *italic* with Markdown. You can even [link to Google!](http://google.com)

#### LISTS

Sometimes you want numbered lists:

1. One
2. Two
3. Three

Sometimes you want bullet points:

* Start a line with a star
* Profit!

Alternatively,

- Dashes work just as well
- And if you have sub points, put two spaces before the dash or star:
  - Like this
  - And this

#### HEADINGS AND quote

# Structured documents

Sometimes it's useful to have different levels of headings to structure your documents. Start lines with a `#` to create headings. Multiple `##` in a row denote smaller heading sizes.

### This is a third-tier heading

You can use one `#` all the way up to `######` six for different heading sizes.

If you'd like to quote someone, use the > character before the line:

> Coffee. The finest organic suspension ever devised... I beat the Borg with it.
> - Captain Janeway


# BROTHER JOHNS TESTDOKUMENT

# Ein Testdokument

Absätze sind die grundlegenden Blockelemente.

Ein neuer Absatz braucht mindestens eine Leerzeile.


Mehrere geht genauso und führen zum gleichen Ergebnis.

Einen Absatz darf man beliebig
umbrechen. Es bleibt ein einzelner Absatz ohne (!) Zeilenumbruch. Bonus: Harte Zeilenumbrüche innerhalb eines Absatzes (aber nicht am Absatzanfang oder -ende) macht man am Zeilenende mit einem Backslash oder mindestens zwei Leerzeichen. Ja, Leerzeichen. Vollkommen bescheuerte Idee, finde ich, aber so ists festgelegt.

Führende Leerzeichen sind ok
          und werden entfernt.

   Ausnahme: Bei der ersten Zeile eines Absatzes dürfen es max. 3 LZ sein, sonst wird ein Codeblock draus – der von der Aufgabe nicht gefordert ist. Also könnte man auf die Spezialbehandlung für erste Zeilen verzichten.

Viel Inline-Formatierung ist ja nicht gefordert: **Fett** und *kursiv*. Was kann man denn damit noch anstellen ... Wer sagt eigentlich, dass man immer nur ganze Wörter formatieren will: Donau*dampf*schifffahrts**kapitäns**mütze.

In der Aufgabe steht nichts von geschachtelter Formatierung, Aber ***fettkursiv*** oder **fett und *kursiv*** ist ja nicht sooo weit aus der Welt. Trotzdem würde ich das als Bonus ansehen.

Inline-Formatierung wird der Reihe nach geparst und dangling Sternchen bleiben simple Sternchen. Beispiel: In *C++* (kursiv) wird der Pointer mit einem Sternchen (*) markiert. Der komplette Regelsatz ist relativ komplex und steht hier: http://spec.commonmark.org/0.27/#emphasis-and-strong-emphasis

Zu den Überschriften: Da die Aufgabenstellung so vage ist, alles laut Spec.

### Laut Aufgabenstellung ist dies keine Überschrift, sondern ein normaler Absatz (da nur H1 und H2 gefordert sind).

#Auch ein normaler Absatz, da mindestens ein Leerzeichen nach dem # stehen muss.

# normale H1-Überschrift
#    genauso normale H1-Überschrift
# auch eine normale H1 mit optionalem schließenden Hash #
# auch das ist eine normale H1, die Anzahl der Schließhashes muss nicht übereinstimmen ##

## normale H2
## H2: keine Schließhashes, da nicht per Leerzeichen getrennt####
## H2: keine Schließhashes, da noch ## Text danach

Spec sagt: “The opening # character may be indented 0-3 spaces.”

 # H1
  # H1
   # H1
    # keine H1. Eigentlich ein Codeblock, aber der ist von der Aufgabe nicht gefordert. Ich würde es als normalen Absatz interpretieren.

Mehrzeilige Überschriften geht nicht. Leerzeilen vor/nach Überschriften sind optional.

Ein Absatz.
## Eine H2
Noch ein Absatz. ##

Nicht von den Hashes am Ende verwirren lassen!


## > Das hier ist eine interessant formatierte H2-Überschrift <

Und wenn wir schon bei den spitzen Klammern sind:

> Ein Zitatabsatz

Oder umgebrochen:

> Auch das ist
> ein einzelner Zitatabsatz (ohne harten Zeilenumbruch!).

Normale Zeilenumbrüche werden genauso behandelt wie im Fließtext.

> Ein einzelner Zitatabsatz
mit Umbruch.

Man kann das natürlich auch kombinieren:

> Das mit dem Zitieren
schaut auf Anhieb einfach aus.
> Isses aber nich. Denn dieser ganze Block hier
> ist ein einzelner Zitatabsatz.

Leerzeilen trennen Zitate.

> Zitat 1

> Zitat 2

Im Vergleich zu:

> Ein einzelnes Zitat.
>
> Es hat zwei Absätze.

Eines noch:

> Das Leerzeichen nach der spitzen Klammer
>ist optional.


## Listen

Lasst uns Sachen auflisten; weil man mit Listen trotz der *am Zeilenanfang*-Einschränkung sooo viel Mist bauen kann! :D

* eins
* zwei
* drei

So weit, so einfach.

* eins
* **zwei** in fett
* *drei* in kursiv
* *Nein, das ist
* nicht kursiv*. Das sind zwei Listenpunkte mit jeweils einem Stern im Text.

Folgendes

*ist laut Spec
*keine Liste, sondern ein Absatz, der zwei Sternchen im Text hat. Wichtig: Kein kursiv wegen dem Zeilenumbruch direkt vor dem zweiten Stern!

Apropos Absätze:

* langweiliger kurzer Listenpunkt
* Der hier ist ein bisschen länglicher.

  V.a. hat er einen zweiten Absatz.
* noch ein Punkt
* und noch einer

Huch!

*

Ein einzelner Stern!? Das ist eine Liste mit einem einzelnen leeren Listenpunkt.

Aber Vorsicht! Ein leerer Listenpunkt
*
unterbricht nicht einen Absatz! Das da eine Zeile höher ist ein simpler Stern.

Alles was leer ist, ist ein wunderschöner Grenzfall. Z.B.:

*
  Was ist das denn?
* Na logisch! Ein Listenpunkt mit führender Leerzeile.
* Todo: Nachlesen, was mit dem Zeilenumbruch passieren soll. Schlucken wie in normalen Absätzen? Oder bleibt der?

Ein leerer Punkt in einer Liste? Logisch:

* eins
*
* drei

Leere Zeilen sind auch was Feines. Sie trennen normalerweise Absätze. In Listen sieht das ein bisschen anders aus.

* Leerzeilen

* zwischen Listenpunkten
* sind erlaubt


* auch mehrere Leerzeilen

Das da oben ist eine einzelne Liste.

* Die Aufgabenstellung fordert den Listenpunkt *am Zeilenanfang*.
  * Das hier ist also keine geschachtelte Liste (im Gegensatz zur Spec).

Sondern? Ähm ... Ich würde sagen: eine Liste mit einem einzelnen Absatz in einem einzelnen Listenpunkt. Der Absatz enthält einen weichen Zeilenumbruch und ein dangling Sternchen.
