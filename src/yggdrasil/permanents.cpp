iterator<Permanent, false> BoardN::pbegin() {return !(mylands.empty()) ? mylands.pbegin() :
                                                    !(myartos.empty()) ? myartos.pbegin() :
                                                    !(mysuperfriends.empty()) ? mysuperfriends.pbegin() :
                                                    !(mycreas.empty()) ? mycreas.pbegin() :
                                                    iterator<Permanent, false>();
}

iterator<Permanent, false> BoardN::pend() const {
    return
        !(mycreas.empty()) ? mycreas.pend() :
        !(mysuperfriends.empty()) ? mysuperfriends.pend() :
        !(myartos.empty()) ? myartos.pend() :
        !(mylands.empty()) ? mylands.pend() :
        iterator<Permanent, false>();
}

iterator<Permanent, true> BoardN::cpbegin() const {return !(mylands.empty()) ? mylands.cpbegin() :
                                                          !(myartos.empty()) ? myartos.cpbegin() :
                                                          !(mysuperfriends.empty()) ? mysuperfriends.cpbegin() :
                                                          !(mycreas.empty()) ? mycreas.cpbegin() :
                                                          iterator<Permanent, true>();
}

iterator<Permanent, true> BoardN::cpend() const {return !(mycreas.empty()) ? mycreas.cpend() :
                                                        !(mysuperfriends.empty()) ? mysuperfriends.cpend() :
                                                        !(myartos.empty()) ? myartos.cpend() :
                                                        !(mylands.empty()) ? mylands.cpend() :
                                                        iterator<Permanent, true>();
}

void BoardN::insert(Card *to_add, Player *pl) {
    switch(to_add->get_type().underlying){
        case card_type::flagged: mylands.construct_pure_child(to_add, pl); break;
        case : myartos.construct_pure_child(to_add, pl); break;
        case 3: mysuperfriends.construct_pure_child(to_add, pl); break;
        case 4: mycreas.construct_pure_child(to_add, pl); break;
        default: std::cout << "Internal error: trying to add non-permanent to battlefield." << std::endl; exit(1);
    }
}

