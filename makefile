CC = g++
CFLAGS = -Wall -Werror -g
LFLAGS = -lallegro -lallegro_font -lallegro_image -lallegro_primitives -lallegro_ttf
IDIR = HFiles
ODIR = obj
CDIR = CFiles

_ALLDEPS = olympus_main.h 1general.h 2cards.h 3modifs.h 4game.h 5board.h 6abilities.h 7options.h 8resolvables.h 9abstractIo.h 10triggers.h 11abstractUI.h head1_constants.h head2_mana.h head3_readfiles.h

ALLDEPS = $(patsubst %,$(IDIR)/%,$(_ALLDEPS))

_OBJ = abilities.o cards.o castopts.o combat.o constants.o createopts.o default_ui.o describe.o disp.o identifiers.o io_allegro.o iterate.o main.o mana.o modifiers.o output_game_allegro.o permanents.o phases.o ragnarok.o readfiles.o readwords.o regularExp.o rules_text.o takeopts.o triggers.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(CDIR)/%.cpp $(ALLDEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

olympus: $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
