#ifndef OLYMPUS_HEADER_IMAGES_14_H
#define OLYMPUS_HEADER_IMAGES_14_H

struct ImageHolder{
	ALLEGRO_BITMAP* ManaNumSym[16];
	ALLEGRO_BITMAP* ManaColorSym[5];
	ALLEGRO_BITMAP* wallpaper;
	ALLEGRO_BITMAP* card_back;
	ALLEGRO_BITMAP* card_template[7];
	ALLEGRO_BITMAP* tapsymbol;
	ALLEGRO_BITMAP* basiclands;
	ALLEGRO_BITMAP* watermarks[7];
	ALLEGRO_BITMAP* pt_box[7];
	ALLEGRO_BITMAP* loyalty;

	ImageHolder();
	~ImageHolder();
};

#endif //OLYMPUS_HEADER_IMAGES_14_H