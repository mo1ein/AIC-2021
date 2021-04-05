#include "Attack.h"

Attack::Attack(int attackerColumn, int attackerRow, int defenderColumn, int defenderRow, bool isAttackerEnemy) {
    attacker_column_ = attackerColumn;
    attacker_row_ = attackerRow;
    defender_column_ = defenderColumn;
    defender_row_ = defenderRow;
    is_attacker_enemy = isAttackerEnemy;
}

int Attack::getAttackerColumn() const {
    return attacker_column_;
}

int Attack::getAttackerRow() const {
    return attacker_row_;
}

int Attack::getDefenderColumn() const {
    return defender_column_;
}

int Attack::getDefenderRow() const {
    return defender_row_;
}

int Attack::isAttackerEnemy() const {
    return is_attacker_enemy;
}
