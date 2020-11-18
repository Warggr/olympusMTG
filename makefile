CC = g++
X = 
CFLAGS = -Wall -Werror -g
LFLAGS = -lallegro -lallegro_font -lallegro_image -lallegro_primitives -lallegro_ttf
IDIR = HFiles
ODIR = obj
CDIR = CFiles

_ALLDEPS = classes.h classes_abilities.h classes_board.h classes_cards.h classes_game.h classes_general.h classes_modifs.h classes_options.h classes_resolvables.h header_mana.h header_readfiles.h header_triggers.h header_images.h
ALLDEPS = $(patsubst %,$(IDIR)/%,$(_ALLDEPS))

_OBJ = main.o cards.o io_allegro.o takeopts.o castopts.o ragnarok.o permanents.o readfiles.o phases.o createopts.o output_game_allegro.o mana.o abilities.o modifiers.o iterate.o rules_text.o combat.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(CDIR)/%.cpp $(ALLDEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

olympus: $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
