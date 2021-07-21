class BoardN: public AbstractN {
public:
    DefaultCollectionTN<Land> mylands;
    DefaultCollectionTN<Artifact> myartos;
    DefaultCollectionTN<Planeswalker> mysuperfriends;
    DefaultCollectionTN<Creature> mycreas;
    StatedCollectionTN<Creature>* myattackers {0};

    BoardN(): mylands(this), myartos(this), mysuperfriends(this), mycreas(this), myattackers(0){};
    ~BoardN();

    void insert(Card* to_add, Player* pl);
    iterator<Permanent, false> pbegin();
    iterator<Permanent, false> pend() const;
    iterator<Permanent, true> cpbegin() const;
    iterator<Permanent, true> cpend() const;
};
