#ifndef AIC21_CLIENT_CPP_ATTACK_H
#define AIC21_CLIENT_CPP_ATTACK_H


class Attack {

public:
    Attack(int attackerColumn, int attackerRow, int defenderColumn, int defenderRow, bool isAttackerEnemy);
    int getAttackerColumn() const;
    int getAttackerRow() const;
    int getDefenderColumn() const;
    int getDefenderRow() const;
    int isAttackerEnemy() const;

private:
    int attacker_column_;
    int attacker_row_;
    int defender_column_;
    int defender_row_;
    bool is_attacker_enemy;
};


#endif //AIC21_CLIENT_CPP_ATTACK_H
