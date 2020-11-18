#ifndef OLYMPUS_CLASSES_MODIFS_8_H
#define OLYMPUS_CLASSES_MODIFS_8_H

struct Targeter{
private:
	Target* content;
public:
	bool valid;
	char colorIdentity;

	Targeter(): valid(true) {god.gdebug(DBG_TARGETING) << " Creating a Targeter\n"; };
	~Targeter();
	void setTarget(Target* tgt);
	Target* getTarget(){return content; };
};

struct ModListNode{
	Targeter* content;
	struct ModListNode* prev;
	struct ModListNode* next;

	ModListNode(Targeter* trgtr, ModListNode* nex): content(trgtr), prev(0), next(nex) {
		god.gdebug(DBG_TARGETING) << "  ModListNode created\n";
	};
	~ModListNode();
};

#endif //OLYMPUS_CLASSES_MODIFS_8_H