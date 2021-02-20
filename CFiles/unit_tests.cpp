#ifdef F_TESTS
char Option::next_tag = 'a';

void verify_chain_integrity(Option** myoptions){
	int i; Option* iter = 0;
	for(i = 0; i < NBMYOPTS && myoptions[i] == 0; i++);
	if(i == NBMYOPTS) return;
#ifdef F_VERBOSE_CREATEOPTS
	std::cout << "Checking chain:";
	disp(myoptions[i], 0);
	std::cout << "\n";
#endif
	if(myoptions[i]->prev != 0){
		god.call_ragnarok();
		std::cout << "Error: first element of chain ";
#ifdef F_VERBOSE_CREATEOPTS
		std::cout << "( <" << myoptions[i]->prev->tag << "(" << myoptions[i]->tag << ")...)";
#endif
		std::cout << " has prev pointer\n";
		exit(1);
	}
	for(iter = myoptions[i]; iter->next != 0; iter = iter->next){
		if(iter->next->prev != iter){
			god.call_ragnarok();
			std::cout << "Error: inconsistency in the chain ";
#ifdef F_VERBOSE_CREATEOPTS
			if(iter->prev) std::cout << "<" << iter->prev->tag << "("; else std::cout << "<0(";
			std::cout << iter->tag << ")";
			if(iter->next) std::cout << iter->next->tag << ">"; else std::cout << "0>";
#endif
			std::cout << "\n";
			exit(1);
		}
		if(iter->exists == false){
			god.call_ragnarok();
			std::cout << "Error: deleted element still in the chain\n";
			exit(1);
		}
		if(iter == iter->next){
			god.call_ragnarok();
			std::cout << "Error: circular reference\n";
			exit(1);
		}
	}
	if(iter != myoptions[NBMYOPTS]){
		god.call_ragnarok();
		std::cout << "Error: last element does not point to last element\n";
		exit(1);
	}
	for(int i = 0; i<=NBMYOPTS; i++){
		if(myoptions[i] != 0 && myoptions[i]->exists == false){
			god.call_ragnarok();
			std::cout << "Error: deleted element (in " << i << ") still pointed by parent\n";
			exit(1);
		}
	}
	std::cout << "Chain integrity OK\n";
}
#endif
