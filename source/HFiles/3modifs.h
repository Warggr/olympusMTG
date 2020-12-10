#ifndef OLYMPUS_CLASSES_MODIFS_8_H
#define OLYMPUS_CLASSES_MODIFS_8_H

struct Targeter{
private:
	Target* content;
	std::string name; //will be remembered even after the target dies
public:
	bool valid;
	char colorIdentity; //-1 means it's an "internal" targeter, such as the one linking an ability to its origin.
	//you can't, of course, have protection from 'internal' targeters

	Targeter(Target* tar): valid(true) {setTarget(tar); };
	Targeter(): valid(true) {god.gdebug(DBG_TARGETING) << " Creating a Targeter\n"; };
	~Targeter(); //removes references to itself from its victim
	void setTarget(Target* tgt);
	Target* getTarget() const {if(valid) return content; else return 0; };
	std::string get_name() const {return name; };
};

#endif //OLYMPUS_CLASSES_MODIFS_8_H